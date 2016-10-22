#ifndef __Common__HeaderFile__
#define __Common__HeaderFile__

#include <ctype.h>
#include <errno.h>
#include <memory.h>
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define LISTEN_PORT 21
#define MAX_CONNECTIONS 20
#define HANDLER_COUNT 10
#define BUFFER_SIZE 8193
#define USER_NAME_MAX 21
#define TYPE_MAX 21
#define REG_EMAIL "^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}$"
#define ERR_BAD_VERB 502
#define ERR_BAD_PARAM 504
#define ERR_UNKNOWN 500
#define CODE_TYPE 200
#define MSG_220 "220 FTP server ready.\r\n"
#define MSG_230 "230 Guest login ok, access restrictions apply.\r\n"
#define MSG_331                                                                \
    "331 Guest login ok, send your complete e-mail address as password.\r\n"
#define MSG_215 "215 UNIX Type: L8\r\n"
#define MSG_221 "221 Goodbye.\r\n"
#define MSG_200_PORT "200 PORT command successful.\r\n"

typedef struct {
    pthread_t tid;
    int connfd;
    char user[USER_NAME_MAX];
    char type[TYPE_MAX];
    unsigned long status;
} thread_data;

typedef int (*pf_check)(const char *);
typedef int (*pf_handle)(thread_data *, char *);

typedef struct {
    pf_check check;
    pf_handle handle;
} Handler;

extern void Register_Handlers(Handler *);
extern void thread_exit(thread_data *);
extern void write_s(thread_data *, const char *, size_t);

#endif