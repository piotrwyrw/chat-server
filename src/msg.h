//
// Created by tux on 13.09.22.
//

#ifndef CHATSERVER_MSG_H
#define CHATSERVER_MSG_H

#include <stddef.h>

typedef enum {
    message_type_regular,
    message_type_disconnect
} message_type;

typedef struct {

    // The author will be placed in front of the actual message. Set to NULL to bypass.
    char *author;

    // The actual message content
    char *str;

    // Descriptor of the receiver socket
    int rcvd;

    // How to handle the message. If message_regular is chosen, the client will display the message as usual. If message_disconnect is selected,
    // the client will show a disconnect screen with the provided message.
    message_type type;

} message;

message *message_create(char *, char *, message_type, int);
void message_destroy(message *);

typedef struct {

    // Same as in message
    char *author;
    char *str;
    message_type type;

} global_message;

global_message *global_message_create(char *, char *, message_type);
void global_message_destroy(global_message *);

#endif //CHATSERVER_MSG_H
