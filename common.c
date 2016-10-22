#include "common.h"

void clean_thd(thread_data *thd) {
    thd->connfd = -1;
    free(thd->user);
    thd->user = NULL;
    free(thd->type);
    thd->type = NULL;
    thd->status = 0;
}

int attr(thread_data *thd, unsigned long flag) {
    return (thd->status & flag) != 0;
}

void setattr(thread_data *thd, unsigned long flag, int method) {
    method ? thd->status = flag : (thd->status = thd->status | flag);
}

void clearattr(thread_data *thd, unsigned long flag) {
    thd->status = ~flag & thd->status;
}

void copy_to(void **dest, const void *p, size_t len) {
    if (*dest)
        free(*dest);
    *dest = malloc(len);
    memcpy(*dest, p, len);
}
