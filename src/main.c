#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "netserver.h"

int main(int argc, char** argv) {

    unsigned short port;

    port = 1025;


    if(argc > 1) {
        for(unsigned i = 1; i < argc; i++) {
            if(!strcmp(argv[i], "-p")) {
                port = atoi(argv[i+1]);
            }
        }
    }


    server *s = server_create(port);
    server_net_start(s);

    return 0;
}
