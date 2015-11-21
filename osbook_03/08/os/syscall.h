#ifndef _KOZOS_SYSCALL_H_INCLUDED_
#define _KOZOS_SYSCALL_H_INCLUDED_

#include "defines.h"

/* システム・コール番号の定義 */
typedef enum {
  KZ_SYSCALL_TYPE_RUN = 0,
  KZ_SYSCALL_TYPE_EXIT,
} kz_syscall_type_t;

/* システム・コール呼び出し時のパラメータ格納域の定義 */
typedef struct {
  union {
    struct {
      kz_func_t func;
      char *name;
      int stacksize;
      int argc;
      char **argv;
      kz_thread_id_t ret;
    } run;
    struct {
      int dummy;
    } exit;
  } un;
} kz_syscall_param_t;

#endif
