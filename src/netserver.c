//
// Created by tux on 12.09.22.
//

#include "netserver.h"
#include "netmsg.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

bool server_net_close = false;

thread_info *thread_info_create(user_info *usr, server *srv) {
    thread_info  *ifo = malloc(sizeof(thread_info));
    ifo->usr = usr;
    ifo->srv = srv;
    return ifo;
}

void thread_info_destroy(thread_info *ifo) {
    free(ifo);
}

void server_net_broadcast(server *srv, global_message *msg) {
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (srv->users[i]) {
            message *m = message_create(msg->author, msg->str, msg->type, srv->users[i]->sockd);
            message_net_send(m);
            message_destroy(m);
        }
}

void server_net_shutdown(server *srv) {
    for (int i = 0; i < __SERVER_MAX_USERS__; i ++)
        if (srv->users[i] != NULL) {
            pthread_cancel(srv->users[i]->thr);
            close(srv->users[i]->sockd);
        }
    close(srv->sockd);
}

void *handle_net_user(void *param) {
    thread_info *ifo = (thread_info *) param;

    // Extract the structs
    user_info *usr = ifo->usr;
    server *srv = ifo->srv;

    usr->thr = pthread_self();

    // Send a welcome message
    bool b = server_add_usr(srv, usr);
    if (!b) {
        printf("Failed to add user to the list.\n");
        pthread_exit(NULL);
    }
    message_net_send_nostruct(NULL, "Hello, world!", message_type_regular, usr->sockd);

    int n;
    char buff[100] = {0};

    while (true) {
        n = recv(usr->sockd, buff, (size_t) 100, 0);
        if (n < 0) {
            printf("Failed to received incoming data: %s.\n", strerror(errno));
            continue;
        }
        if (strcmp(buff, "shutdown") == 0 || strcmp(buff, "shutdown\n") == 0) {
            server_net_close = true;
        }
        printf("[%d] %s", usr->user_id, buff);
    }

}

void *connection_handler(void *param) {
    server *srv = (server *) param;
    int n = 0;

    // Handle any incoming connections
    while (true) {

        // User data that we'll receive
        struct sockaddr usr_addr;
        socklen_t usr_addr_len = 0;

        // Accept the connection and fill up the struct/s
        n = accept(srv->sockd, &usr_addr, &usr_addr_len);
        if (n < 0) {
            printf("Failed to accept a new connection: %s\n", strerror(errno));
            continue;
        }

        // Get the connection ID
        int id = srv->user_ct;
        srv->user_ct ++;
        if (srv->user_ct > __SERVER_MAX_USERS__) {
            printf("Refusing a new connection because the server is overfilled.\n");
            close(n);
            continue;
        }

        // Handle the new user in a separate thread
        printf("New connection accepted (ID %d). Starting new thread.\n", id);
        pthread_t thread;
        user_info *usr = user_info_create(n, 0, NULL, usr_addr, usr_addr_len, id);
        thread_info *info = thread_info_create(usr, srv);
        pthread_create(&thread, NULL, handle_net_user, info);
        pthread_detach(thread);
    }
}

/**
 * Start the server and listen on the appropriate port (specified in the passed server struct)
 * @param srv
 */
void server_net_start(server *srv) {
    int n; // Variable for temporarily storing errors which occur in this function
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

    pthread_t man;
    pthread_create(&man, NULL, connection_handler, srv);
    pthread_detach(man);

    // Wait for a close request
    while (!server_net_close) {}

    server_net_shutdown(srv);
    pthread_cancel(man);

    server_destroy(srv);
    return;
}