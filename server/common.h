#ifndef __Common__HeaderFile__
#define __Common__HeaderFile__

#include <arpa/inet.h>
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

#define MAX_CONNECTIONS 20
#define HANDLER_COUNT 10
#define BUFFER_SIZE 8193
#define CHUNK_SIZE 4096
#define USER_NAME_MAX 21
#define TYPE_MAX 21
#define REG_EMAIL_STRICT "^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,}$"
#define REG_EMAIL "^[A-Z0-9._%+-]+@[A-Z0-9.-]*$"
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
#define MSG_200 "200 PORT command successful.\r\n"
#define MSG_226 "226 Transfer complete.\r\n"
#define MSG_150 "150 transmission begin.\r\n"
#define MSG_425 "425 Cannot activate connection.\r\n"
#define MSG_451 "451 Cannot open file on server.\r\n"
#define MSG_500 "500 Internal error occurred.\r\n"
#define MSG_426 "426 connection failure.\r\n"

// status definition
#define ST_NOTHING 0x0UL
#define ST_WAIT_PASS 0x1UL
#define ST_LOGGED_IN 0x2UL
#define ST_TYPE_SET 0x4UL
#define ST_PORT_SET 0x8UL
#define ST_PASV_SET 0x10UL

extern const char *root_path;
extern int listen_port;

typedef struct {
    pthread_t tid;
    int connfd;
    char *user;
    char *type;
    struct sockaddr_in *remote;
    int listen;
    unsigned long status;
} thread_data;

typedef int (*pf_check)(const char *);
typedef int (*pf_handle)(thread_data *, char *);

typedef struct {
    pf_check check;
    pf_handle handle;
} Handler;

typedef struct { int a, b, c, d, p, q; } Pasv_info;

extern void Register_Handlers(Handler *);
extern void thread_exit(thread_data *);
extern void write_s(thread_data *, const char *, size_t);
extern int write_b(int, const char *, size_t);
extern void clean_thd(thread_data *);
extern int attr(thread_data *, unsigned long);
extern void setattr(thread_data *, unsigned long, int);
extern void clearattr(thread_data *, unsigned long);
extern void copy_to(void **, const void *, size_t);
extern void set_remote(thread_data *, int, int, int, int, int, int);
extern Pasv_info pasv_init(thread_data *);

#endif
