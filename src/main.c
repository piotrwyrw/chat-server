#include <stdio.h>

#include "netserver.h"

int main() {

    server *s = server_create(1025);
    server_net_start(s);

    return 0;
}
