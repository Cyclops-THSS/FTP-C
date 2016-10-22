/**
 * @file
 * @brief implement handlers
 */

#include "common.h"

static const char *allowed_users[] = {"anonymous"};

int logged_in(const thread_data *thd) { return thd->status == 1; }

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

int user_handle(thread_data *thd, char *st) {
    for (size_t i = 0; i < sizeof(allowed_users) / sizeof(const char *); i++)
        if (!strcmp(st + 5, allowed_users[i])) {
            int len = strlen(st + 5);
            if (len >= USER_NAME_MAX - 1) {
                return sprintf(st, "%d %s", ERR_BAD_PARAM,
                               "username too long!\r\n");
            }
            memcpy(thd->user, st + 5, len + 1);
            thd->status = 0;
            return sprintf(st, "%s", MSG_331);
        }
    return sprintf(st, "%d %s", ERR_BAD_PARAM, "user unregistered!\r\n");
}

int pass_handle(thread_data *thd, char *st) {
    static regex_t regex;
    regcomp(&regex, REG_EMAIL, REG_ICASE | REG_EXTENDED);
    if (thd->status != 0)
        return sprintf(st, "%d %s", ERR_BAD_VERB,
                       "user logged in or not set!\r\n");
    if (regexec(&regex, st + 5, 0, NULL, 0))
        return sprintf(st, "%d %s", ERR_BAD_PARAM,
                       "invalid email address!\r\n");
    thd->status = 1;
    return sprintf(st, "%s", MSG_230);
}

int syst_handle(thread_data *thd, char *st) {
    return sprintf(st, "%s", MSG_215);
}

int type_handle(thread_data *thd, char *st) {
    if (thd->type[0] != -1)
        return sprintf(st, "%d %s\r\n", ERR_BAD_VERB, "type already set!");
    memcpy(thd->type, st + 5, strlen(st + 5) + 1);
    return sprintf(st, "%d %s %s\r\n", CODE_TYPE, "Type set to", thd->type);
}

int quit_handle(thread_data *thd, char *st) {
    int len = sprintf(st, "%s", MSG_221);
    write_s(thd, st, len);
    thread_exit(thd);
    return len; // Useless
}

int port_handle(thread_data *thd, char *st) {
    int h1, h2, h3, h4, p1, p2;
    sscanf(st, "PORT %d,%d,%d,%d,%d,%d", &h1, &h2, &h3, &h4, &p1, &p2);
    unsigned long port = htonl(p1 * 256 + p2);

    return sprintf(st, "%s", MSG_200_PORT);
}

int pasv_handle(thread_data *thd, char *st) {}

int retr_handle(thread_data *thd, char *st) {}

int stor_handle(thread_data *thd, char *st) {}

Handler _register(pf_check c, pf_handle h) {
    Handler hd;
    hd.check = c;
    hd.handle = h;
    return hd;
}

void Register_Handlers(Handler *arr) {
    arr[0] = _register(user_check, user_handle);
    arr[1] = _register(pass_check, pass_handle);
    arr[2] = _register(syst_check, syst_handle);
    arr[3] = _register(type_check, type_handle);
    arr[4] = _register(quit_check, quit_handle);
    arr[5] = _register(abor_check, quit_handle);
    arr[6] = _register(port_check, port_handle);
    arr[7] = _register(pasv_check, pasv_handle);
    arr[8] = _register(retr_check, retr_handle);
    arr[9] = _register(stor_check, stor_handle);
}
