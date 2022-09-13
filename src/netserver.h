//
// Created by tux on 12.09.22.
//

#ifndef CHATSERVER_NETSERVER_H
#define CHATSERVER_NETSERVER_H

#include "server.h"
#include "msg.h"

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * Handle errors that occur due to the networking functions and have an effect on the errno value.
 */
#define __NETSERVER_ERROR__(val, msg) \
        if (val < 0) {              \
            printf("%s: %s\n", msg, strerror(errno)); \
            return;                   \
        }\

typedef struct {
    user_info *usr;
    server *srv;
} thread_info;

thread_info *thread_info_create(user_info *, server *);
void thread_info_destroy(thread_info *);

extern bool server_net_close;

void server_net_broadcast(server *, global_message *);
void server_net_shutdown(server *);
void server_net_start(server *);

#endif //CHATSERVER_NETSERVER_H
