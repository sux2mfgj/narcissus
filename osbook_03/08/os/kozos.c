#include "defines.h"
#include "kozos.h"
#include "intr.h"
#include "interrupt.h"
#include "syscall.h"
#include "lib.h"

#define THREAD_NUM 6
#define THREAD_NAME_SIZE 15

/* スレッド・コンテキスト */
typedef struct _kz_context {
  uint32 sp; /* スタック・ポインタ */
} kz_context;

/* タスク・コントロール・ブロック(TCB) */
typedef struct _kz_thread {
  struct _kz_thread *next;
  char name[THREAD_NAME_SIZE + 1]; /* スレッド名 */
  char *stack;    /* スタック */

  struct { /* スレッドのスタート・アップ(thread_init())に渡すパラメータ */
    kz_func_t func; /* スレッドのメイン関数 */
    int argc;       /* スレッドのメイン関数に渡す argc */
    char **argv;    /* スレッドのメイン関数に渡す argv */
  } init;

  struct { /* システム・コール用バッファ */
    kz_syscall_type_t type;
    kz_syscall_param_t *param;
  } syscall;

  kz_context context; /* コンテキスト情報 */
} kz_thread;

/* スレッドのレディー・キュー */
static struct {
  kz_thread *head;
  kz_thread *tail;
} readyque;

static kz_thread *current; /* カレント・スレッド */
static kz_thread threads[THREAD_NUM]; /* タスク・コントロール・ブロック */
static kz_handler_t handlers[SOFTVEC_TYPE_NUM]; /* 割込みハンドラ */

void dispatch(kz_context *context);

/* カレント・スレッドをレディー・キューから抜き出す */
static int getcurrent(void)
{
  if (current == NULL) {
    return -1;
  }

  /* カレント・スレッドは必ず先頭にあるはずなので，先頭から抜き出す */
  readyque.head = current->next;
  if (readyque.head == NULL) {
    readyque.tail = NULL;
  }
  current->next = NULL;

  return 0;
}

/* カレント・スレッドをレディー・キューに繋げる */
static int putcurrent(void)
{
  if (current == NULL) {
    return -1;
  }

  /* レディー・キューの末尾に接続する */
  if (readyque.tail) {
    readyque.tail->next = current;
  } else {
    readyque.head = current;
  }
  readyque.tail = current;

  return 0;
}

static void thread_end(void)
{
  kz_exit();
}

/* スレッドのスタート・アップ */
static void thread_init(kz_thread *thp)
{
  /* スレッドのメイン関数を呼び出す */
  thp->init.func(thp->init.argc, thp->init.argv);
  thread_end();
}

/* システム・コールの処理(kz_run():スレッドの起動) */
static kz_thread_id_t thread_run(kz_func_t func, char *name,
				 int stacksize, int argc, char *argv[])
{
  int i;
  kz_thread *thp;
  uint32 *sp;
  extern char userstack; /* リンカ・スクリプトで定義されるスタック領域 */
  static char *thread_stack = &userstack;

  /* 空いているタスク・コントロール・ブロックを検索 */
  for (i = 0; i < THREAD_NUM; i++) {
    thp = &threads[i];
    if (!thp->init.func) /* 見つかった */
      break;
  }
  if (i == THREAD_NUM) /* 見つからなかった */
    return -1;

  memset(thp, 0, sizeof(*thp));

  /* タスク・コントロール・ブロック(TCB)の設定 */
  strcpy(thp->name, name);
  thp->next     = NULL;

  thp->init.func = func;
  thp->init.argc = argc;
  thp->init.argv = argv;

  /* スタック領域を獲得 */
  memset(thread_stack, 0, stacksize);
  thread_stack += stacksize;

  thp->stack = thread_stack; /* スタックを設定 */

  /* スタックの初期化 */
  sp = (uint32 *)thp->stack;
  *(--sp) = (uint32)thread_end;

  /*
   * プログラム・カウンタを設定する．
   */
  *(--sp) = (uint32)thread_init;

  *(--sp) = 0; /* ER6 */
  *(--sp) = 0; /* ER5 */
  *(--sp) = 0; /* ER4 */
  *(--sp) = 0; /* ER3 */
  *(--sp) = 0; /* ER2 */
  *(--sp) = 0; /* ER1 */

  /* スレッドのスタート・アップ(thread_init())に渡す引数 */
  *(--sp) = (uint32)thp;  /* ER0 */

  /* スレッドのコンテキストを設定 */
  thp->context.sp = (uint32)sp;

  /* システム・コールを呼び出したスレッドをレディー・キューに戻す */
  putcurrent();

  /* 新規作成したスレッドを，レディー・キューに接続する */
  current = thp;
  putcurrent();

  return (kz_thread_id_t)current;
}

/* システム・コールの処理(kz_exit():スレッドの終了) */
static int thread_exit(void)
{
  /*
   * 本来ならスタックも解放して再利用できるようにすべきだが省略．
   * このため，スレッドを頻繁に生成・消去するようなことは現状でできない．
   */
  puts(current->name);
  puts(" EXIT.\n");
  memset(current, 0, sizeof(*current));
  return 0;
}

/* 割込みハンドラの登録 */
static int setintr(softvec_type_t type, kz_handler_t handler)
{
  static void thread_intr(softvec_type_t type, unsigned long sp);

  /*
   * 割込みを受け付けるために，ソフトウエア・割込みベクタに
   * OSの割込み処理の入口となる関数を登録する．
   */
  softvec_setintr(type, thread_intr);

  handlers[type] = handler; /* OS側から呼び出す割込みハンドラを登録 */

  return 0;
}

static void call_functions(kz_syscall_type_t type, kz_syscall_param_t *p)
{
  /* システム・コールの実行中にcurrentが書き換わるので注意 */
  switch (type) {
  case KZ_SYSCALL_TYPE_RUN: /* kz_run() */
    p->un.run.ret = thread_run(p->un.run.func, p->un.run.name,
			       p->un.run.stacksize,
			       p->un.run.argc, p->un.run.argv);
    break;
  case KZ_SYSCALL_TYPE_EXIT: /* kz_exit() */
    /* TCBが消去されるので，戻り値を書き込んではいけない */
    thread_exit();
    break;
  default:
    break;
  }
}

/* システム・コールの処理 */
static void syscall_proc(kz_syscall_type_t type, kz_syscall_param_t *p)
{
  /*
   * システム・コールを呼び出したスレッドをレディー・キューから
   * 外した状態で処理関数を呼び出す．このためシステム・コールを
   * 呼び出したスレッドをそのまま動作継続させたい場合には，
   * 処理関数の内部で putcurrent() を行う必要がある．
   */
  getcurrent();
  call_functions(type, p);
}

/* スレッドのスケジューリング */
static void schedule(void)
{
  if (!readyque.head) /* 見つからなかった */
    kz_sysdown();

  current = readyque.head; /* カレント・スレッドに設定する */
}

static void syscall_intr(void)
{
  syscall_proc(current->syscall.type, current->syscall.param);
}

static void softerr_intr(void)
{
  puts(current->name);
  puts(" DOWN.\n");
  getcurrent(); /* レディーキューから外す */
  thread_exit(); /* スレッド終了する */
}

/* 割込み処理の入口関数 */
static void thread_intr(softvec_type_t type, unsigned long sp)
{
  /* カレント・スレッドのコンテキストを保存する */
  current->context.sp = sp;

  /*
   * 割込みごとの処理を実行する．
   * SOFTVEC_TYPE_SYSCALL, SOFTVEC_TYPE_SOFTERR の場合は
   * syscall_intr(), softerr_intr() がハンドラに登録されているので，
   * それらが実行される．
   */
  if (handlers[type])
    handlers[type]();

  schedule(); /* スレッドのスケジューリング */

  /*
   * スレッドのディスパッチ
   * (dispatch()関数の本体はstartup.sにあり，アセンブラで記述されている)
   */
  dispatch(&current->context);
  /* ここには返ってこない */
}

void kz_start(kz_func_t func, char *name, int stacksize,
	      int argc, char *argv[])
{
  /*
   * 以降で呼び出すスレッド関連のライブラリ関数の内部で current を
   * 見ている場合があるので，current を NULL に初期化しておく．
   */
  current = NULL;

  readyque.head = readyque.tail = NULL;
  memset(threads,  0, sizeof(threads));
  memset(handlers, 0, sizeof(handlers));

  /* 割込みハンドラの登録 */
  setintr(SOFTVEC_TYPE_SYSCALL, syscall_intr); /* システム・コール */
  setintr(SOFTVEC_TYPE_SOFTERR, softerr_intr); /* ダウン要因発生 */

  /* システム・コール発行不可なので直接関数を呼び出してスレッド作成する */
  current = (kz_thread *)thread_run(func, name, stacksize, argc, argv);

  /* 最初のスレッドを起動 */
  dispatch(&current->context);

  /* ここには返ってこない */
}

void kz_sysdown(void)
{
  puts("system error!\n");
  while (1)
    ;
}

/* システム・コール呼び出し用ライブラリ関数 */
void kz_syscall(kz_syscall_type_t type, kz_syscall_param_t *param)
{
  current->syscall.type  = type;
  current->syscall.param = param;
  asm volatile ("trapa #0"); /* トラップ割込み発行 */
}
