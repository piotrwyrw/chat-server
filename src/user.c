//
// Created by tux on 12.09.22.
//

#include "user.h"

#include <stdlib.h>
#include <string.h>

user_info *user_info_create(int sockd, pthread_t thr, char *nick, struct sockaddr addr, socklen_t addr_len, int id) {
    user_info *ifo = malloc(sizeof(user_info));
    ifo->sockd = sockd;
    ifo->thr = thr;
    if (!nick)
        ifo->nick = NULL;
    else {
        ifo->nick =  strdup(nick);
    }
    ifo->addr = addr;
    ifo->addr_len = addr_len;
    ifo->user_id = id;
    return ifo;
}

void user_info_destroy(user_info *ifo) {
    if (ifo->nick)
        free(ifo->nick);
    free(ifo);
}

// Perform an in-depth comparison between the two structs
bool user_info_compare(user_info *a, user_info *b) {
    bool equal = true;
    if (a->sockd != b->sockd)
        equal = false;
    if (a->thr != b->thr)
        equal = false;
    if (a->nick != NULL && b->nick != NULL)
        if (strcmp(a->nick, b->nick) != 0)
            equal = false;
    return equal;
}