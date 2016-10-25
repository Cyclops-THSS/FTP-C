/**
 * @file
 * @brief implement handlers
 */

#include "common.h"

static const char *allowed_users[] = {"anonymous"};

int user_check(const char *st) { return !strncmp(st, "USER", 4); }
int pass_check(const char *st) { return !strncmp(st, "PASS", 4); }
int syst_check(const char *st) { return !strncmp(st, "SYST", 4); }
int type_check(const char *st) { return !strncmp(st, "TYPE", 4); }
int quit_check(const char *st) { return !strncmp(st, "QUIT", 4); }
int abor_check(const char *st) { return !strncmp(st, "ABOR", 4); }
int port_check(const char *st) { return !strncmp(st, "PORT", 4); }
int pasv_check(const char *st) { return !strncmp(st, "PASV", 4); }
int retr_check(const char *st) { return !strncmp(st, "RETR", 4); }
int stor_check(const char *st) { return !strncmp(st, "STOR", 4); }
int cwd_check(const char *st) { return !strncmp(st, "CWD", 3); }
int pwd_check(const char *st) { return !strncmp(st, "PWD", 3); }
int cdup_check(const char *st) { return !strncmp(st, "CDUP", 4); }
int dele_check(const char *st) { return !strncmp(st, "DELE", 4); }
int mkd_check(const char *st) { return !strncmp(st, "MKD", 3); }
int rmd_check(const char *st) { return !strncmp(st, "RMD", 3); }
int rnfr_check(const char *st) { return !strncmp(st, "RNFR", 4); }
int rnto_check(const char *st) { return !strncmp(st, "RNTO", 4); }
int list_check(const char *st) { return !strncmp(st, "LIST", 4); }

static pf_check all_check_in_turn[] = {
    user_check, pass_check, syst_check, type_check, quit_check,
    abor_check, port_check, pasv_check, retr_check, stor_check,
    cwd_check,  pwd_check,  cdup_check, dele_check, mkd_check,
    rmd_check,  rnfr_check, rnto_check, list_check};

/**
 * USER command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     331 if ready to log in
 */
int user_handle(thread_data *thd, char *st) {
    if (attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "user logged in!");
    clearattr(thd, ST_WAIT_PASS);
    for (size_t i = 0; i < sizeof(allowed_users) / sizeof(const char *); i++)
        if (!strcmp(st + 5, allowed_users[i])) {
            int len = strlen(st + 5);
            if (len >= USER_NAME_MAX - 1) {
                return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM,
                               "username too long!");
            }
            copy_to(&thd->user, st + 5, len + 1);
            setattr(thd, ST_WAIT_PASS, 0);
            return sprintf(st, "%s", MSG_331);
        }
    return sprintf(st, "%d %s", ERR_BAD_PARAM, "user unregistered!\r\n");
}

/**
 * PASS command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     230 if logged in
 */
int pass_handle(thread_data *thd, char *st) {
    if (attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "user logged in!");
    static regex_t regex;
    regcomp(&regex, REG_EMAIL, REG_ICASE | REG_EXTENDED);
    if (!attr(thd, ST_WAIT_PASS))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "need USER first!");
    if (regexec(&regex, st + 5, 0, NULL, 0))
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM,
                       "invalid email address!");
    setattr(thd, ST_LOGGED_IN, 0);
    clearattr(thd, ST_WAIT_PASS);
    return sprintf(st, "%s", MSG_230);
}

/**
 * SYST command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     215 everytime
 */
int syst_handle(thread_data *thd, char *st) {
    return sprintf(st, "%s", MSG_215);
}

/**
 * TYPE command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     200 and the new type
 */
int type_handle(thread_data *thd, char *st) {
    copy_to(&thd->type, st + 5, strlen(st + 5) + 1);
    setattr(thd, ST_TYPE_SET, 0);
    return sprintf(st, "%d %s %s.\r\n", CODE_TYPE, "Type set to", thd->type);
}

/**
 * QUIT command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     useless
 */
int quit_handle(thread_data *thd, char *st) {
    int len = sprintf(st, "%s", MSG_221);
    write_s(thd, st, len);
    thread_exit(thd);
    return len; // Useless
}

/**
 * PORT command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     200 if set successfully
 */
int port_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (attr(thd, ST_PASV_SET)) {
        close(thd->listen);
        thd->listen = -1;
        clearattr(thd, ST_PASV_SET);
    }
    int h1, h2, h3, h4, p1, p2;
    sscanf(st, "PORT %d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2);
    set_remote(thd, h1, h2, h3, h4, p1, p2);
    setattr(thd, ST_PORT_SET, 0);
    return sprintf(st, "%s", MSG_200);
}

/**
 * PASV command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     227, ip and port
 */
int pasv_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (attr(thd, ST_PORT_SET))
        clearattr(thd, ST_PORT_SET);
    if (attr(thd, ST_PASV_SET)) {
        close(thd->listen);
        thd->listen = -1;
        clearattr(thd, ST_PASV_SET);
    }
    Pasv_info pi = pasv_init(thd);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        printf("Error socket(): %s(%d)\n", strerror(errno), errno);
        return sprintf(st, "%s", MSG_500);
    }
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(pi.p * 256 + pi.q);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("Error bind(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }
    if (listen(sock, MAX_CONNECTIONS) == -1) {
        printf("Error listen(): %s(%d)\n", strerror(errno), errno);
        return 1;
    }
    setattr(thd, ST_PASV_SET, 0);
    thd->listen = sock;
    return sprintf(st, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\r\n",
                   pi.a, pi.b, pi.c, pi.d, pi.p, pi.q);
}

/**
 * Helper function for sending a file to client
 * @param thd thread_data
 * @param st  command
 * @param fd  file descriptor
 * @param fp  file pointer
 */
void send_file(thread_data *thd, char *st, int fd, FILE *fp) {
    char chunk[CHUNK_SIZE];
    ssize_t bytes = 0;
    sprintf(st, "%s", MSG_150);
    write_s(thd, st, strlen(st));
    while ((bytes = fread(chunk, sizeof(char), CHUNK_SIZE, fp)) > 0)
        if (write_b(fd, chunk, bytes) < 0) {
            sprintf(st, "%s", MSG_426);
            write_s(thd, st, strlen(st));
            break;
        }
    fclose(fp);
}

/**
 * RETR command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     226 on finishing transmission
 */
int retr_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (strstr(st + 5, "../"))
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM, "unsupported!");
    if (!attr(thd, ST_PASV_SET | ST_PORT_SET))
        return sprintf(st, "%s", MSG_425);
    char file_name[1024];
    strcpy(file_name, thd->prefix);
    strcat(file_name, "/");
    strcat(file_name, st + 5);
    FILE *fp = fopen(file_name, "rb");
    if (fp == NULL)
        return sprintf(st, "%s", MSG_451);
    int so_data = socket(AF_INET, SOCK_STREAM, 0);
    if (attr(thd, ST_PORT_SET) &&
        connect(so_data, (const struct sockaddr *)thd->remote,
                sizeof(struct sockaddr)) == 0) {
        send_file(thd, st, so_data, fp);
    } else if (attr(thd, ST_PASV_SET) && thd->listen != -1) {
        int connfd = accept(thd->listen, NULL, NULL);
        if (connfd == -1) {
            printf("Error accept(): %s(%d)\n", strerror(errno), errno);
            return sprintf(st, "%s", MSG_500);
        }
        send_file(thd, st, connfd, fp);
        close(connfd);
    }
    close(so_data);
    return sprintf(st, "%s", MSG_226);
}

/**
 * LIST command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     info about file or directory in /bin/ls format: written to data
 * connection
 */
int list_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (!attr(thd, ST_PASV_SET | ST_PORT_SET))
        return sprintf(st, "%s", MSG_425);
    char file_name[1024] = {};
    strcpy(file_name, thd->prefix);
    strcat(file_name, "/");
    strcat(file_name, st + 5);
    if (check_path(file_name) == IS_INVALID)
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM, "file not exist.");
    char cmd[1030] = "ls -l ";
    strcat(cmd, file_name);
    FILE *fp = popen(cmd, "r");
    if (fp == NULL)
        return sprintf(st, "%s", MSG_451);
    int so_data = socket(AF_INET, SOCK_STREAM, 0);
    if (attr(thd, ST_PORT_SET) &&
        connect(so_data, (const struct sockaddr *)thd->remote,
                sizeof(struct sockaddr)) == 0) {
        send_file(thd, st, so_data, fp);
    } else if (attr(thd, ST_PASV_SET) && thd->listen != -1) {
        int connfd = accept(thd->listen, NULL, NULL);
        if (connfd == -1) {
            printf("Error accept(): %s(%d)\n", strerror(errno), errno);
            return sprintf(st, "%s", MSG_500);
        }
        send_file(thd, st, connfd, fp);
        close(connfd);
    }
    pclose(fp);
    close(so_data);
    return sprintf(st, "%s", MSG_226);
}

/**
 * Helper function for getting a file from client
 * @param thd thread_data
 * @param st  command
 * @param fd  file descriptor
 * @param fp  file pointer
 */
void get_file(thread_data *thd, char *st, int fd, FILE *fp) {
    sprintf(st, "%s", MSG_150);
    write_s(thd, st, strlen(st));
    char chunk[CHUNK_SIZE];
    ssize_t bytes = 0;
    while (1) {
        if ((bytes = read(fd, chunk, CHUNK_SIZE)) < 0) {
            sprintf(st, "%s", MSG_426);
            write_s(thd, st, strlen(st));
        } else if (bytes == 0)
            break;
        fwrite(chunk, sizeof(char), bytes, fp);
    }
    fclose(fp);
}

/**
 * STOR command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     226 on finishing transmission
 */
int stor_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (strstr(st + 5, "../"))
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM, "unsupported!");
    if (!attr(thd, ST_PASV_SET | ST_PORT_SET))
        return sprintf(st, "%s", MSG_425);
    char file_name[1024];
    strcpy(file_name, thd->prefix);
    strcat(file_name, "/");
    strcat(file_name, st + 5);
    FILE *fp = fopen(file_name, "wb");
    if (fp == NULL)
        return sprintf(st, "%s", MSG_451);
    int so_data = socket(AF_INET, SOCK_STREAM, 0);
    if (attr(thd, ST_PORT_SET) &&
        connect(so_data, (const struct sockaddr *)thd->remote,
                sizeof(struct sockaddr)) == 0) {
        get_file(thd, st, so_data, fp);
    } else if (attr(thd, ST_PASV_SET) && thd->listen != -1) {
        int connfd = accept(thd->listen, NULL, NULL);
        if (connfd == -1) {
            printf("Error accept(): %s(%d)\n", strerror(errno), errno);
            return sprintf(st, "%s", MSG_500);
        }
        get_file(thd, st, connfd, fp);
        close(connfd);
    }
    close(so_data);
    return sprintf(st, "%s", MSG_226);
}

/**
 * CWD command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     250 on success else 550
 */
int cwd_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (strstr(st + 4, "../"))
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM, "unsupported!");
    char prefix[1024];
    strcpy(prefix, thd->prefix);
    if (*(st + 4) != '/')
        strcat(prefix, "/");
    strcat(prefix, st + 4);
    if (check_path(prefix) == IS_DIR) {
        copy_to(&thd->prefix, prefix, strlen(prefix) + 1);
        return sprintf(st, "%s", MSG_250);
    } else
        return sprintf(st, "%d %s\r\n", ERR_BAD_FILE,
                       "No such file or directory");
}

/**
 * CDUP command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     250 on success else 550
 */
int cdup_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    if (!strcmp(thd->prefix, root_path))
        return sprintf(st, "%d %s\r\n", ERR_BAD_FILE, "root path reached!");
    *strrchr(thd->prefix, '/') = '\0';
    return sprintf(st, MSG_250);
}

/**
 * PWD command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     257 and current prefix
 */
int pwd_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    return sprintf(st, "%d \"%s\"\r\n", CODE_PWD, thd->prefix);
}

/**
 * DELE command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     250 on success else 550
 */
int dele_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    char path[1024];
    strcpy(path, thd->prefix);
    strcat(path, "/");
    strcat(path, st + 5);
    if (strstr(path, "../"))
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM, "unsupported!");
    int prop;
    if ((prop = check_path(path)) != IS_INVALID) {
        if (prop == IS_FILE)
            remove(path);
        else {
            char cmd[1035] = {};
            sprintf(cmd, "rm -rf %s", path);
            int status;
            if (!((status = system(cmd)) != -1 && WIFEXITED(status) &&
                  WEXITSTATUS(status) == 0))
                return sprintf(st, "%d %s\r\n", ERR_BAD_FILE,
                               "removal failed!");
        }
        return sprintf(st, "%s", MSG_250);
    } else
        return sprintf(st, "%d %s\r\n", ERR_BAD_FILE, "removal failed!");
}

/**
 * MKD command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     257 and maked dir on success
 */
int mkd_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    char path[1024];
    strcpy(path, thd->prefix);
    strcat(path, "/");
    strcat(path, st + 4);
    if (check_path(path) == IS_INVALID && !strstr(path, "../")) {
        if (mkdir(path, 0777) == -1) {
            printf("Error mkdir(): %s(%d)\n", strerror(errno), errno);
            return sprintf(st, "%s", MSG_500);
        }
        return sprintf(st, "%d \"%s\"\r\n", CODE_PWD, path);
    } else
        return sprintf(st, "%d %s\r\n", ERR_BAD_PARAM,
                       "path exists or illegal!");
}

/**
 * RMD command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     250 on success else 550 or 504
 */
int rmd_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    char path[1024] = {};
    strcpy(path, thd->prefix);
    strcat(path, "/");
    strcat(path, st + 4);
    char cmd[1035] = {};
    sprintf(cmd, "rm -rf %s", path);
    if (check_path(path) != IS_DIR && !strstr(path, "../")) {
        int status;
        if ((status = system(cmd)) != -1 && WIFEXITED(status) &&
            WEXITSTATUS(status) == 0) {
            return sprintf(st, "%s", MSG_250);
        } else
            return sprintf(st, "%d %s\r\n", ERR_BAD_FILE, "removal failed!");
    }
    return sprintf(st, "%d \"%s\"\r\n", ERR_BAD_PARAM,
                   "path non-exist or illegal!");
}

/**
 * RNFR command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     350 on success else 550
 */
int rnfr_handle(thread_data *thd, char *st) {
    if (!attr(thd, ST_LOGGED_IN))
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "login first!");
    char path[1024] = {};
    strcpy(path, thd->prefix);
    strcat(path, "/");
    strcat(path, st + 5);
    if (check_path(path) != IS_INVALID) {
        copy_to(&thd->temp, path, strlen(path) + 1);
        return sprintf(st, "%s", MSG_350);
    }
    return sprintf(st, "%d %s\r\n", ERR_BAD_FILE, "path does not exist!");
}

/**
 * RNTO command handler
 * @param  thd thread_data
 * @param  st  command
 * @return     250 on success else 503
 */
int rnto_handle(thread_data *thd, char *st) {
    if (!thd->temp)
        return sprintf(st, "%d %s\r\n", ERR_BAD_SEQ, "bad operation sequence!");
    char path[1024] = {};
    strcpy(path, thd->prefix);
    strcat(path, "/");
    strcat(path, st + 5);
    if (rename(thd->temp, path) == -1) {
        printf("Error rename(): %s(%d)\n", strerror(errno), errno);
        thread_exit(thd);
    }
    free(thd->temp);
    thd->temp = NULL;
    return sprintf(st, "%s", MSG_250);
}

/**
 * Helper function for register a Handler
 * @param  c check function
 * @param  h handle function
 * @return   Handler object
 */
Handler _register(pf_check c, pf_handle h) {
    Handler hd;
    hd.check = c;
    hd.handle = h;
    return hd;
}

static pf_handle all_handle_in_turn[] = {
    user_handle, pass_handle, syst_handle, type_handle, quit_handle,
    quit_handle, port_handle, pasv_handle, retr_handle, stor_handle,
    cwd_handle,  pwd_handle,  cdup_handle, dele_handle, mkd_handle,
    rmd_handle,  rnfr_handle, rnto_handle, list_handle};

const size_t handler_count = sizeof(all_check_in_turn) / sizeof(pf_check);

/**
 * Register all handlers
 * @param arr container for Handlers
 */
void Register_Handlers(Handler **arr) {
    *arr = (Handler *)malloc(handler_count * sizeof(Handler));
    for (size_t i = 0; i < handler_count; i++)
        (*arr)[i] = _register(all_check_in_turn[i], all_handle_in_turn[i]);
}
