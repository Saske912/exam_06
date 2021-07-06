//
// Created by Pamula File on 7/6/21.
//
#include "header.h"

t_cli *new_cli(int id, int sock)
{
    t_cli *cli = malloc(sizeof(t_cli));

    if (!cli)
        error_exit("Fatal error\n");
    cli->sock = sock;
    cli->send_buf = NULL;
    cli->receive_buf = NULL;
    cli->id = id;
    cli->next = NULL;
    cli->max_fd = 0;
    return cli;
}

void lst_add_back(t_cli *temp, int ret, int *id)
{
    t_cli *t = temp;
    t_cli *new = new_cli(*id++, ret);
    while (t->next)
        t = t->next;
    t->next = new;
}

void add_to_buffers( const char *msg, int new_id, t_cli *point)
{
    int     len_int = 0;
    int     temp = new_id;
    char    *buf;

    while (temp > 9)
    {
        temp /= 10;
        len_int++;
    }
    buf = calloc(1, strlen(msg) - 2 + 1 + len_int);
    if (!buf)
        error_exit("Fatal error\n");
    sprintf(buf, msg, new_id);
    while (point)
    {
        if (point->send_buf)
        {
            point->send_buf = realloc( point->send_buf, strlen( point->send_buf) + strlen( buf) + 1);
            check_fatal(point->send_buf);
            strcat( point->send_buf, buf);
        }
        else
        {
            point->send_buf = calloc( 1, strlen( buf) + 1);
            check_fatal(point->send_buf);
            strcpy( point->send_buf, buf);
        }
        point = point->next;
    }
}