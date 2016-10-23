#include "common.h"

void clean_thd(thread_data *thd) {
    thd->connfd = -1;
    free(thd->user);
    thd->user = NULL;
    free(thd->type);
    thd->type = NULL;
    free(thd->remote);
    thd->remote = NULL;
    thd->listen = -1;
    thd->status = ST_NOTHING;
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

void set_remote(thread_data *thd, int h1, int h2, int h3, int h4, int p1,
                int p2) {
    if (thd->remote)
        free(thd->remote);
    char ip_dec[40];
    sprintf(ip_dec, "%d.%d.%d.%d", h1, h2, h3, h4);
    thd->remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
    thd->remote->sin_family = AF_INET;
    thd->remote->sin_port = htons(p1 * 256 + p2);
    thd->remote->sin_addr.s_addr = inet_addr(ip_dec);
}

int write_b(int fd, const char *sentence, size_t length) {
    size_t p = 0;
    while (p < length) {
        ssize_t n = write(fd, sentence + p, length - p);
        if (n < 0) {
            printf("Error write(): %s(%d)\n", strerror(errno), errno);
            return -1;
        }
        p += n;
    }
    return p;
}
