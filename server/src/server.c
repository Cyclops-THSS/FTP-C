/**
 * @file
 * @brief implement server
 */

#include "common.h"

static int listenfd;
static thread_data thr_d[MAX_CONNECTIONS] = {};
static Handler *handlers = NULL;
int listen_port = 21;
const char *root_path = "/tmp";

void process_exit(int);
ssize_t read_s(thread_data *, char *);
int handle_s(thread_data *, char *);
void *dispatcher(void *);

int main(int argc, char **argv) {

    if (argc > 1) {
        for (size_t i = 1; i < argc; i += 2) {
            if (!strcmp(argv[i], "-port"))
                listen_port = atoi(argv[i + 1]);
            if (!strcmp(argv[i], "-root"))
                root_path = argv[i + 1];
        }
    }

    Register_Handlers(&handlers);
    signal(SIGINT, process_exit);

    for (size_t i = 0; i < sizeof(thr_d) / sizeof(thread_data); i++)
        clean_thd(&thr_d[i]);

    struct sockaddr_in addr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(listen_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("Error bind(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

    if (listen(listenfd, MAX_CONNECTIONS) == -1) {
        printf("Error listen(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }

    while (1) {
        int connfd;
        if ((connfd = accept(listenfd, NULL, NULL)) == -1) {
            printf("Error accept(): %s(%d)\n", strerror(errno), errno);
            continue;
        }
        size_t index = 0;
        for (; index < MAX_CONNECTIONS; index++) {
            if (thr_d[index].connfd == -1)
                break;
        }
        if (index == MAX_CONNECTIONS) {
            printf("Error main(): Connections overflow!\n");
            continue;
        }
        thr_d[index].connfd = connfd;
        copy_to(&thr_d[index].prefix, root_path, strlen(root_path) + 1);
        if (pthread_create(&thr_d[index].tid, NULL, dispatcher,
                           &thr_d[index])) {
            printf("Error pthread_create(): %s(%d)\n", strerror(errno), errno);
            continue;
        }
    }

    process_exit(0);
}

/**
 * Get host ip, choose a port, and return
 *
 * @param  thd current thread
 * @return     (a, b, c, d, p, q) indicates a.b.c.d:(p * 256 + q)
 */
Pasv_info pasv_init(thread_data *thd) {
    static const char *google_dns = "8.8.8.8";
    static const int dns_port = 53;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        pthread_exit(thd);
    }
    Pasv_info pi;
    memset(&pi, 0, sizeof(pi));
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(google_dns);
    local.sin_port = htons(dns_port);
    if (connect(sock, (const struct sockaddr *)&local, sizeof(local)) != 0) {
        printf("Error connect(): %s(%d)\n", strerror(errno), errno);
        pthread_exit(thd);
    }
    memset(&local, 0, sizeof(local));
    socklen_t len = sizeof(local);
    if (getsockname(sock, (struct sockaddr *)&local, &len) != 0) {
        printf("Error getsockname(): %s(%d)\n", strerror(errno), errno);
        pthread_exit(NULL);
    }
    char buffer[100];
    const char *p = inet_ntop(AF_INET, &local.sin_addr, buffer, 100);
    sscanf(p, "%d.%d.%d.%d", &pi.a, &pi.b, &pi.c, &pi.d);
    int ran = rand() % 45535 + 20000;
    pi.p = ran / 256;
    pi.q = ran - pi.p * 256;
    close(sock);
    return pi;
}

/**
 * Join all threads and exit main process.
 *
 * @param sig signal Keyboard_Interrupt
 */
void process_exit(int sig) {
    for (size_t i = 0; i < MAX_CONNECTIONS; i++)
        if (thr_d[i].connfd != -1)
            pthread_join(thr_d[i].tid, NULL);
    close(listenfd);
    if (handlers)
        free(handlers);
    exit(0);
}

/**
 * Exit a thread and free resource.
 *
 * @param d thread descriptor to exit
 */
void thread_exit(thread_data *d) {
    close(d->connfd);
    clean_thd(d);
    pthread_exit(NULL);
}

/**
 * Write a string to a socket in a thread
 *
 * @param thd      current thread
 * @param sentence string to send
 * @param length   string length (exclude '\0')
 */
void write_s(thread_data *thd, const char *sentence, size_t length) {
    int p = write_b(thd->connfd, sentence, length);
    if (p <= 0)
        thread_exit(thd);
    if (p != length) {
        printf("Error write_s(): Unfinished writing.\n");
        thread_exit(thd);
    }
}

/**
 * Read a string from a socket in a thread.
 *
 * @param  thd  current thread
 * @param  dest destination to store the string
 * @return      received string length (exclude '\0')
 */
ssize_t read_s(thread_data *thd, char *dest) {
    size_t p = 0;
    while (p < BUFFER_SIZE) {
        ssize_t n = read(thd->connfd, dest + p, BUFFER_SIZE - 1 - p);
        if (n < 0) {
            printf("Error read(): %s(%d)\n", strerror(errno), errno);
            thread_exit(thd);
        } else if (n == 0)
            break; // TODO: broken ?
        p += n;
        if (dest[p - 1] == '\n')
            break;
    }
    dest[p] = '\0';
    return p;
}

/**
 * Handle operation string
 *
 * @param  thd      current thread
 * @param  sentence operation
 * @return          response stirng's length
 */
int handle_s(thread_data *thd, char *sentence) {
    // Truncate crlf
    char *crlf = strrchr(sentence, '\r');
    if (crlf && crlf[1] == '\n' && crlf[2] == '\0')
        crlf[0] = '\0';
    // Select handler
    size_t i = 0, ans = -1;
    for (; i < handler_count; i++)
        if (handlers[i].check(sentence)) {
            ans = handlers[i].handle(thd, sentence);
            break;
        }
    if (handler_count == i)
        ans = sprintf(sentence, "%d %s", ERR_BAD_VERB,
                      "Commond not implemented!\r\n");
    return ans;
}

/**
 * Dispatch all operation messages.
 * Exit if no more messages or on error.
 *
 * @param  arg thread_data of the thread
 * @return     always NULL
 */
void *dispatcher(void *arg) {
    thread_data *thd = (thread_data *)arg;
    char sentence[BUFFER_SIZE] = {};
    // Initial welcome
    write_s(thd, MSG_220, strlen(MSG_220));
    // Message loop
    while (1) {
        memset(sentence, 0, sizeof(sentence));
        read_s(thd, sentence);
        int len = handle_s(thd, sentence);
        write_s(thd, sentence, len);
    }
    thread_exit(thd);
}
