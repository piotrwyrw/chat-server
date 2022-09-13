//
// Created by tux on 13.09.22.
//

#ifndef CHATSERVER_NETMSG_H
#define CHATSERVER_NETMSG_H

#include "msg.h"
#include "bool.h"

bool message_net_send(message *);
bool message_net_send_nostruct(char *, char *, message_type, int);

#endif //CHATSERVER_NETMSG_H