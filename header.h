//
// Created by Pamula File on 7/6/21.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

typedef struct  s_cli
{
    int             id;
    int             sock;
    char            *send_buf;
    char            *receive_buf;
    int             max_fd;
    struct s_cli    *next;
}               t_cli;

void error_exit(char *error);
t_cli *new_cli(int id, int sock);
void lst_add_back(t_cli *temp,int ret, int *id);
void add_to_buffers( const char *msg, t_cli *point, int sock);
void check_fatal(void *buf);
void    del_one(t_cli *head, t_cli *del);
void    paste_or_concat(const char *buf, char **buffer_type);
char    *parse_buff(char *buffer, int id, char **remind);
void    cut(char **send_buf, int ret);

#endif //HEADER_H
