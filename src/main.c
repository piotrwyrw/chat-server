#include "netserver.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    int real_argc = argc - 1;

    server *s = server_create(1024);

    if (real_argc <= 0) {
        printf("No port specified. Defaulting to %d.\n", s->port);
        goto start;
    }

    for (int i = 1; i < argc; i ++) {
        char *arg = argv[i];
        if (i + 1 >= argc) {
            printf("Value expected for parameter '%s'.\n", arg);
            goto exit;
        }
        char *val = argv[++ i];
        if (strcmp(arg, "--port") == 0 || strcmp(arg, "-p") == 0) {
            if (!check_is_number(val)) {
                printf("The value of the parameter '%s' is expected to be a positive integer.\n", arg);
                goto exit;
            }
            s->port = atoi(val);
            if (s->port < 1024) {
                printf("Warn: It is not recommended to use any port number below 1024.\n");
            }
            continue;
        }
        printf("Unknown parameter: '%s'.\n", arg);
        goto exit;
    }

    start:
    server_net_start(s);
    goto only_return;

    exit:
    server_destroy(s);

    only_return:

    return 0;
}
