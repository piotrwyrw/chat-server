//
// Created by tux on 12.09.22.
//

#ifndef CHATSERVER_NETSERVER_H
#define CHATSERVER_NETSERVER_H

#include "server.h"

#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/**
 * Handle errors that occur due to the networking functions and have an effect on the errno value.
 */
#define __NETSERVER_ERROR__(val, msg) \
        if (val < 0) {              \
            printf("%s: %s", msg, strerror(errno)); \
            return;                   \
        }\

void server_net_shutdown(server *);
void server_net_start(server *);

#endif //CHATSERVER_NETSERVER_H
