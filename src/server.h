//
// Created by tux on 12.09.22.
//

#ifndef CHARSERVER_SERVER_H
#define CHATSERVER_SERVER_H

#include "user.h"
#include "bool.h"
#include <netinet/in.h>

#define __SERVER_MAX_USERS__ 100

typedef struct {
    short port;
    user_info **users;
    unsigned int user_ct;

    struct sockaddr_in addr;
    int sockd;
    bool up;
} server;

server *server_create(short);
void server_destroy(server *);

bool server_add_usr(server *, user_info *);
bool server_remove_usr(server *, user_info *);

#endif //CHATSERVER_SERVER_H
