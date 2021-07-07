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

void    del_one(t_cli *head, t_cli *del)
{
    t_cli *tmp = head;
    t_cli *tmp2;
    while (tmp->next != del)
        tmp = tmp->next;
    tmp2 = tmp->next;
    tmp->next = tmp2->next;
    if (tmp2->send_buf)
        free(tmp2->send_buf);
    if (tmp2->receive_buf)
        free(tmp2->receive_buf);
    free(tmp2);
}