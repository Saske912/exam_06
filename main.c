#include "header.h"

void error_exit(char *error)
{
    write(2, error, strlen(error));
    exit(1);
}

void check_fatal(void *buf)
{
    if (!buf)
        error_exit("Fatal error\n");
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
    t_cli   head;
    t_cli   *temp;
    struct timeval tv;
    int     ret;
    struct sockaddr addr;
    char    buffer[20000];
    char    small_buffer[400];
    socklen_t len = sizeof(addr);
    char    *parsed_buffer;
    int     error = 0;

    tv.tv_sec = 3;
    tv.tv_usec = 0;
    head.next = NULL;
    head.sock = sock;
    head.id = 0;
    while (1)
    {
        head.max_fd = 0;
        temp = &head;
        FD_ZERO(&read);
        while (temp)
        {
            FD_SET(temp->sock, &read);
            if (temp->sock > head.max_fd)
                head.max_fd = temp->sock;
            temp = temp->next;
        }
        ret = select(head.max_fd + 1, &read, NULL, NULL, &tv);
        if (ret == -1)
            continue ;
        temp = &head;
        if ( FD_ISSET(temp->sock, &read))
        {
            ret = accept(temp->sock, &addr, &len);
            if (ret != -1)
            {
                lst_add_back(temp, ret, &head.id);
                bzero(small_buffer, 400);
                sprintf(small_buffer, "server: client %d just arrived\n", head.id);
                add_to_buffers( small_buffer, head.next, ret );
            }
        }
        temp = head.next;
        while (temp)
        {
            if (getsockopt(temp->sock, SOL_SOCKET, SO_ERROR, &error, &len))
            {
                if (error == ECONNRESET || error == ECONNREFUSED || error == ECONNABORTED)
                {
                    bzero(small_buffer, 400);
                    sprintf(small_buffer, "server: client %d just left\n", temp->id);
                    add_to_buffers( small_buffer, head.next, temp->sock );
                    del_one(&head, temp);
                    continue ;
                }
            }
            if ( FD_ISSET(temp->sock, &read))
            {
                bzero(buffer, 20000);
                ret = recv(temp->sock, buffer, 19999, 0);
                if (ret == 0)
                {
                    bzero(small_buffer, 400);
                    sprintf(small_buffer, "server: client %d just left\n", temp->id);
                    add_to_buffers( small_buffer, head.next, temp->sock );
                    del_one(&head, temp);
                    continue ;
                }
                else if (ret == -1)
                    continue;
                else
                {
                    parsed_buffer = parse_buff(buffer, temp->id, &temp->receive_buf);
                    add_to_buffers(parsed_buffer, head.next, temp->sock);
                    free(parsed_buffer);
                }
            }
            temp = temp->next;
        }
        temp = head.next;
        while (temp)
        {
            if (temp->send_buf)
            {
                ret = send(temp->sock, temp->send_buf, strlen(temp->send_buf), 0);
                if (ret == -1)
                    continue;
                else if (ret != strlen(temp->send_buf))
                {
                    cut(&temp->send_buf, ret);
                }
                else
                    free(temp->send_buf);
            }
            temp = temp->next;
        }
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
