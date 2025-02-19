#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "echo_socket"

int main()
{
    int s, t, len;
    struct sockaddr_un remote;
    char str[100];

    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
    {
        printf("Error setting up socket\n");
        exit(1);
    }
    printf("Connecting to socket...\n");

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, SOCK_PATH);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);

    if (connect(s, (struct sockaddr *)&remote, len) == -1) 
    {
        printf("Error connecting to socket\n");
        exit(1);
    }

    printf("Connected.\n");

    while(printf("$ "), fgets(str, 100, stdin), !feof(stdin))
    {
        if (send(s, str, strlen(str), 0) == -1)
        {
            printf("Error sending the message");
            exit(1);
        }

        if ((t=recv(s, str, 100, 0)) > 0)
        {
            str[t] = '\0';
            printf("server $ %s\n", str);
        }
        else
        {
            t < 0 ? printf("Error recieving message") : printf("Server closed connection");
            exit(1);
        }
        
    }
    close(s);
    return 0;
}