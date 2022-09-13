//
// Created by tux on 12.09.22.
//

#ifndef CHATSERVER_USER_H
#define CHATSERVER_USER_H

#include "bool.h"

#include <pthread.h>
#include <netinet/in.h>

typedef struct {
    // Socket descriptor
    int sockd;

    // Thread on which the client I/O is handled
    pthread_t thr;

    // The user's nickname
    char *nick;

    // User address
    struct sockaddr addr;
    socklen_t addr_len;

    // The server will refer to the user by their ID
    int user_id;
} user_info;

user_info *user_info_create(int, pthread_t, char *, struct sockaddr, socklen_t, int);
void user_info_destroy(user_info *);

bool user_info_compare(user_info *, user_info *);

#endif //CHATSERVER_USER_H
