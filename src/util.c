//
// Created by tux on 13.09.22.
//

#include "util.h"

#include <string.h>

bool check_is_number(char *str) {
    for (int i = 0; i < strlen(str); i ++)
        if (!__IS_DIGIT__(str[i]))
            return false;
    return true;
}

void clear_buffer(char *buff) {
    for (unsigned int i = 0; i < strlen(buff); i ++)
        buff[i] = 0;
}