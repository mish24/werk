#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>

void error(const char *message)
{
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        error("Usage: inetd <port> <command>");
    char *cmd = argv[2];
    int port = atoi(argv[1]);
    int server, client;
    struct sockaddr_in address;
    int addrlen = sizeof(struct sockaddr_in);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    server = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(server, (struct sockaddr *) &address, sizeof(address));
    listen(server, 1024);
    signal(SIGCHLD, SIG_IGN);
    while (1) {
        client = accept(server, (struct sockaddr *)&address, &addrlen);
        int pid = fork();
        if (pid == 0) {
            close(server);
            dup2(client, STDIN_FILENO);
            dup2(client, STDOUT_FILENO);
            dup2(client, STDERR_FILENO);
            execvp(cmd, argv + 2);
        }
        close(client);
    }
    return 0;
}