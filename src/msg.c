//
// Created by tux on 13.09.22.
//

#include "msg.h"

#include <stdlib.h>
#include <string.h>

message *message_create(char *author, char *str, message_type type, int rcvd) {
    message *msg = malloc(sizeof(message));
    if (author)
        msg->author = strdup(author);
    else
        msg->author = NULL;
    msg->str = strdup(str);
    msg->type = type;
    msg->rcvd = rcvd;
    return msg;
}

void message_destroy(message *msg) {
    if (msg->author)
        free(msg->author);
    free(msg->str);
    free(msg);
}

global_message *global_message_create(char *author, char *message, message_type type) {
    global_message *msg = malloc(sizeof(global_message));
    if (author)
        msg->author = strdup(author);
    else
        msg->author = NULL;
    msg->str = strdup(message);
    msg->type = type;
    return msg;
}

void global_message_destroy(global_message *msg) {
    if (msg->author)
        free(msg->author);
    free(msg->str);
    free(msg);
}