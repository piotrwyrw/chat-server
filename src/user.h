//
// Created by tux on 12.09.22.
//

#ifndef CHATSERVER_USER_H
#define CHATSERVER_USER_H

#include "bool.h"

#include <pthread.h>
#include <netinet/in.h>

typedef struct  {
    int sockd;
    pthread_t thr;
    char *nick;
    struct sockaddr addr;
} user_info;

user_info *user_info_create(int, pthread_t, char *, struct sockaddr);
void user_info_destroy(user_info *);

bool user_info_compare(user_info *, user_info *);

#endif //CHATSERVER_USER_H
