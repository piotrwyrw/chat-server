//
// Created by tux on 12.09.22.
//

#include "netserver.h"

#include <stdlib.h>
#include <unistd.h>

server *server_create(short port) {
    server *srv = malloc(sizeof(server));
    srv->port = port;
    srv->user_ct = 0;
    srv->users = malloc(sizeof(user_info *) * __SERVER_MAX_USERS__);
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        srv->users[i] = NULL;
    srv->sockd = 0;
    srv->up = false;
    return srv;
}

void server_destroy(server *srv) {
    server_net_shutdown(srv);
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (srv->users[i])
            user_info_destroy(srv->users[i]);
    free(srv->users);
    free(srv);
}

bool server_add_usr(server *srv, user_info *ifo) {
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (!srv->users[i]) {
            srv->users[i] = ifo;
            return true;
        }
    return false;
}

bool server_remove_usr(server *srv, user_info *ifo) {
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (srv->users[i])
            if (user_info_compare(ifo, srv->users[i])) {
                user_info_destroy(srv->users[i]);
                srv->users[i] = NULL;
                return true;
            }
    return false;
}