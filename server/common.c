/**
 * @file
 * @brief implement common functions
 */

#include "common.h"

/**
 * As destructor of struct hread_data
 * @param thd the object
 */
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

/**
 * Test if thd's status satisfy flag
 * @param  thd  thread_data
 * @param  flag one flag or the bit or of flags
 * @return      bool indicator
 */
int attr(thread_data *thd, unsigned long flag) {
    return (thd->status & flag) != 0;
}

/**
 * Set thread_data's status
 * @param thd    thread_data
 * @param flag   flag to set
 * @param method 0 means bit or, 1 means assignment
 */
void setattr(thread_data *thd, unsigned long flag, int method) {
    method ? thd->status = flag : (thd->status = thd->status | flag);
}

/**
 * Clear certain attributes in thread_data
 * @param thd  thread_data
 * @param flag attributes
 */
void clearattr(thread_data *thd, unsigned long flag) {
    thd->status = ~flag & thd->status;
}

/**
 * Free resource and make a copy of p stored at *dest
 * @param dest pointer's pointer
 * @param p    source data pointer
 * @param len  length in byte
 */
void copy_to(void **dest, const void *p, size_t len) {
    if (*dest)
        free(*dest);
    *dest = malloc(len);
    memcpy(*dest, p, len);
}

/**
 * For PORT command, set a sockaddr_in struct using given parameters
 * @param thd thread_data
 * @param h1  ip(1)
 * @param h2  ip(2)
 * @param h3  ip(3)
 * @param h4  ip(4)
 * @param p1  port(higher 8-bit)
 * @param p2  port(lower 8-bit)
 */
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

/**
 * Binary write function
 * @param  fd       file descriptor
 * @param  sentence data to send
 * @param  length   length of data
 * @return          length or -1 indicating error
 */
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
