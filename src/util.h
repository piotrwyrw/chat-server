//
// Created by tux on 13.09.22.
//

#ifndef CHATSERVER_UTIL_H
#define CHATSERVER_UTIL_H

#include "bool.h"

#define __IS_DIGIT__(x) \
        (x >= '0' && x <= '9')

bool check_is_number(char *);

#endif //CHATSERVER_UTIL_H
