//
// Created by tux on 12.09.22.
//

#include "netserver.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <pthread.h>

void server_net_shutdown(server *srv) {
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (srv->users[i])
            pthread_cancel(srv->users[i]->thr);
    close(srv->sockd);
}

void *handle_user(void *param) {
    user_info *ifo = (user_info *) param;
    const char *welcome = "Welcome to the server!\n";
    send(ifo->sockd, welcome, strlen(welcome), 0);
    user_info_destroy(ifo);
    close(ifo->sockd);
    pthread_exit(NULL);
}

/**
 * Start the server and listen on the appropriate port (specified in the passed server struct)
 * @param srv
 */
void server_net_start(server *srv) {
    int n; // Variable for temporarily storing errors which occur in this functio
    int y = 1;

    if (srv->up) {
        printf("Could not start the server, because it is already running.\n");
        return;
    }

    printf("Starting server on port %d ..\n", srv->port);

    // Create the socket descriptor
    srv->sockd = socket(AF_INET, SOCK_STREAM, 0);
    __NETSERVER_ERROR__(srv->sockd, "Failed to create socket");

    // Change socket settings
    n = setsockopt(srv->sockd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &y, (socklen_t) sizeof(int));
    __NETSERVER_ERROR__(n, "Failed to set socket option SO_REUSEADDR | SO_REUSEPORT");

    // Zero out the address struct
    memset(&(srv->addr), 0, sizeof(struct sockaddr_in));

    // Set up the address struct
    srv->addr.sin_family = AF_INET;
    srv->addr.sin_port = htons(srv->port);
    srv->addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    n = bind(srv->sockd, (struct sockaddr *) &(srv->addr), (socklen_t) sizeof(struct sockaddr_in));
    __NETSERVER_ERROR__(n, "Failed to bind the socket");

    // Set up the socket to listen for connections, with a maximum queue length of 50
    n = listen(srv->sockd, 50);
    __NETSERVER_ERROR__(n, "Failed to listen");

    printf("Server listening on port %d ..\n", srv->port);

    // Handle any incoming connections
    while (true) {
        int av;
        struct sockaddr usr_addr;
        socklen_t usr_addr_len;
        n = accept(srv->sockd, &usr_addr, &usr_addr_len);
        if (n < 0) {
            printf("Failed to accept a new connection: %s\n", strerror(errno));
            continue;
        }
        printf("New connection accepted. Starting new thread.\n");
        pthread_t thread;
        user_info *info = user_info_create(n, thread, "name_not_yet_defined", usr_addr);
        pthread_create(&thread, NULL, handle_user, info);
        pthread_join(thread, NULL);
    }

}