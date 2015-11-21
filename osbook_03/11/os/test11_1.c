#include "defines.h"
#include "kozos.h"
#include "lib.h"

int test11_1_main(int argc, char *argv[])
{
  char *p;
  int size;

  puts("test11_1 started.\n");

  /* 静的領域をメッセージで受信 */
  puts("test11_1 recv in.\n");
  kz_recv(MSGBOX_ID_MSGBOX1, &size, &p); /* 受信 */
  puts("test11_1 recv out.\n");
  puts(p);

  /* 動的に獲得した領域をメッセージで受信 */
  puts("test11_1 recv in.\n");
  kz_recv(MSGBOX_ID_MSGBOX1, &size, &p); /* 受信 */
  puts("test11_1 recv out.\n");
  puts(p);
  kz_kmfree(p); /* メモリ解放 */

  /* 静的領域をメッセージで送信 */
  puts("test11_1 send in.\n");
  kz_send(MSGBOX_ID_MSGBOX2, 15, "static memory\n"); /* 送信 */
  puts("test11_1 send out.\n");

  /* 動的に獲得した領域をメッセージで送信 */
  p = kz_kmalloc(18); /* メモリ獲得 */
  strcpy(p, "allocated memory\n");
  puts("test11_1 send in.\n");
  kz_send(MSGBOX_ID_MSGBOX2, 18, p); /* 送信 */
  puts("test11_1 send out.\n");
  /* メモリ解放は受信側で行うので，ここでは不要 */

  puts("test11_1 exit.\n");

  return 0;
}
