#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>

void error_exit(char *error)
{
    write(2, error, strlen(error));
    exit(1);
}

char * reverse(char *port )
{
    char *ret = malloc( strlen(port) + 1);
    int     len = strlen(port);
    int counter = 0;

    if (!ret)
        error_exit("Fatal error\n");
    while (counter < len)
    {
        ret[counter] = port[len - 1 -counter];
        counter++;
    }
    ret[counter] = 0;
    return ret;
}

void loop(int sock)
{
    fd_set  read;
    int     fds[500];

    memset(fds, 0, sizeof(fds));
    while (1)
    {
        FD_ZERO(&read);
        FD_SET(sock, &read);
        while ()
    }
}

int main(int ac, char **av)
{
    int     sock;
    int     ret;
    struct sockaddr_in address;
    char    *buf;

    address.sin_addr.s_addr = (1 << 24) | 127;
    address.sin_family = PF_INET;
    buf = reverse(av[1]);
    address.sin_port = atoi(buf);
    free(buf);
    unsigned int    address_len = sizeof(address);
    if (ac >= 2)
    {
        sock = socket( PF_INET, SOCK_STREAM, 0);
        if ( sock == -1)
            error_exit("Fatal error\n");
        //testing
        fcntl(sock, F_SETFL, O_NONBLOCK);
        ret = bind(sock, &address, address_len);
        if (ret == -1)
            error_exit("Fatal error\n");
        ret = listen(sock, 128);
        if (ret == -1)
            error_exit("Fatal error\n");
        loop(sock);
    }
    else
        error_exit("Wrong number of arguments\n");
    return 0;
}
