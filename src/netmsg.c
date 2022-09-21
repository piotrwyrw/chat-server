//
// Created by tux on 13.09.22.
//

#include "msg.h"
#include "user.h"

#include <stdlib.h>
#include <string.h>

bool message_net_send(message *msg) {
    int n;

    // Compose the message
    char *message = malloc(((msg->author) ? strlen(msg->author) : 0) + strlen(msg->str) + 2);
    if (msg->author) {
        strcpy(message, msg->author);
        unsigned int len = strlen(message);
        message[len - 1] = ' ';
        message[len] = 0;
        strncat(message, msg->str, strlen(msg->str));
    } else strcpy(message, msg->str);

    // Send the msg
    n = send(msg->rcvd, message, (size_t) (strlen(message) + 1), 0);

    free(message);

    return (n > 0);
}

bool message_net_send_nostruct(char *author, char *msg, message_type type, int sockd) {
    int n;
    message *m = message_create(author, msg, type, sockd);
    n = message_net_send(m);
    message_destroy(m);
    return (n > 0);
}