#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"

int main(void)
{
    int s, s2, t, len;
    struct sockaddr_un local, remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        printf("Error setting up socket\n");
        exit(1);
    }

    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) {
        printf("Error binding the socket\n");
        exit(1);
    }

    if (listen(s, 5) == -1) {
        printf("Error listening socket\n");
        exit(1);
    }

    for(;;) {
        int done, n;
        printf("Waiting for a connection...\n");
        t = sizeof(remote);
        if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
            printf("Error accepting connection\n");
            exit(1);
        }

        printf("Connected.\n");

        done = 0;
        do {
            n = recv(s2, str, 100, 0);
            char* userInput[100];
            printf("%s", str);
            scanf(" %s", (char *)&userInput);
            if (n <= 0) {
                if (n < 0) printf("Error recieving from socket\n");;
                done = 1;
            }

            if (!done) 
                if (send(s2, userInput, n, 0) < 0) {
                    printf("Error sending to socket\n");
                    done = 1;
                }
        } while (!done);

        close(s2);
    }

    return 0;
}