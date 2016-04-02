#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERROR_INFO __FILE__, __func__, __LINE__
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_error(M, ...) fprintf(stderr, "[ERROR] (%s:%s:%d: errno: %s) " M "\n", ERROR_INFO, clean_errno(), ##__VA_ARGS__)
#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%s:%d: errno: %s) " M "\n", ERROR_INFO, clean_errno(), ##__VA_ARGS__)
#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%s:%d) " M "\n", ERROR_INFO, ##__VA_ARGS__)

#define SUCCESS 0
#define ERROR -1
#endif
