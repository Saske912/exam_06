//
//                                       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⣠⣴⣖⣁⣀⣀⡄⠀⠀⠀
//                                       ⠀⠀⠀⠀⠀⠀⢀⣀⣠⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣤⣄⣀⣀
//                                      ⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁
//                                      ⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠀
//                                      ⠀⠀⠀⠀⠀⣼⣿⡟⠀⢠⡲⠖⠀⣿⣿⣿⣿⣿⣿⣿⣉⠁⠀⠀⠀
//  ╱╱╱╱╭━╮╭╮                               ⣼⣿⣿⡷⢤⠤⠤⣥⡤⣿⠿⣿⣿⣿⡿⣿⣿⣷⡀
//  ╱╱╱╱┃╭╯┃┃                           ⠀⣀⣠⣼⣿⣿⣿⣧⠑⠟⠀⠈⠙⠱⢦⣿⣿⣿⠁⣸⣿⣿⣇⠀
//  ╭━━┳╯╰┳┫┃╭━━╮                       ⠊⠉⠉⠉⠉⠩⠞⠁⠀⠀⠄⠀⠀⡴⣿⣿⣿⠗⣵⣿⠡⠉⠉⠁⠀
//  ┃╭╮┣╮╭╋┫┃┃┃━┫                              ⠀⢡⠀⠀⠀⢈⣾⣟⣵⣯⣼⣿⣬⣄⣀⠀⠀
//  ┃╰╯┃┃┃┃┃╰┫┃━┫                           ⠀⠀⣶⣶⣶⣾⣥⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀
//  ┃╭━╯╰╯╰┻━┻━━╯                           ⠀⢺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⡄
//  ┃┃                                     ⢠⢤⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⡄
//  ╰╯                                  ⠀⠠⣰⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠇⠀
//07.07.2021

#include "header.h"

char    **split(char *buffer, char splitter)
{
    int     tmp = 0;
    int     count_lines = 0;
    int     stack[300];
    int     len;
    char    **ret;
    int     tmp2 = 0;
    int     tmp3 = 0;

    len = (int)strlen(buffer);
    bzero(stack,300);
    while (tmp < len)
    {
        if (buffer[tmp] == splitter)
        {
            if (count_lines)
                stack[count_lines] = tmp - stack[count_lines - 1];
            else
                stack[count_lines] = tmp;
            count_lines++;
        }
        tmp++;
    }
    ret = calloc(sizeof(char *), count_lines + 1);
    check_fatal(ret);
    tmp = 0;
    while (tmp2 < count_lines)
    {
        tmp3 += tmp;
        tmp = 0;
        ret[tmp2] = calloc(sizeof(char), stack[tmp2] + 1);
        check_fatal(ret[tmp2]);
        while (tmp < stack[tmp2])
        {
            ret[tmp2][tmp] = buffer[tmp3 + tmp];
            tmp++;
        }
        tmp2++;
    }
    return ret;
}

char    *parse_buff(char *buffer, int id, char **remind)
{
    char    *ret;
    char    cli[50];
    char    **messages;
    char    *buffer_and_reminder;
    int     len = 1;
    int     tmp = 0;

    bzero(cli, 50);
    sprintf(cli, "client %d:", id);
    if (*remind)
    {
        buffer_and_reminder = calloc(sizeof(char), strlen(buffer) + strlen(*remind) + 1);
        check_fatal(buffer_and_reminder);
        strcpy(buffer_and_reminder, *remind);
        free(*remind);
        strcat(buffer_and_reminder, buffer);
        messages = split(buffer_and_reminder, '\n');
    }
    else
        messages = split(buffer, '\n');
    ret = calloc(sizeof(char), strlen(cli) + strlen(messages[0]) + 1);
    check_fatal(ret);
    strcpy(ret, cli);
    strcat(ret, messages[0]);
    while (messages[len])
    {
        if (messages[len][strlen(messages[len] - 1)] == '\n')
        {
            ret = realloc(ret, strlen(ret) + strlen(cli) + strlen(messages[len]) + 1);
            check_fatal(ret);
            strcat(ret, cli);
            strcat(ret, messages[len]);
        }
        else
            *remind = messages[len];
        len++;
    }
    while (tmp < len)
    {
        if (*remind && messages[tmp] == *remind)
            break ;
        free(messages[tmp]);
    }
    free(messages);
    return ret;
}

void    paste_or_concat(const char *buf, char **buffer_type)
{
    char *temp_buf = *buffer_type;

    if (temp_buf)
    {
        temp_buf = realloc( temp_buf, strlen( temp_buf) + strlen( buf) + 1);
        check_fatal(temp_buf);
        strcat( temp_buf, buf);
    }
    else
    {
        temp_buf = calloc( 1, strlen( buf) + 1);
        check_fatal(temp_buf);
        strcpy( temp_buf, buf);
    }
}

void add_to_buffers( const char *msg, t_cli *point, int sock)
{
    while (point)
    {
        if (point->sock != sock)
            paste_or_concat(msg, &point->send_buf);
        point = point->next;
    }
}

void    cut(char **send_buf, int ret)
{
    char    *tmp = *send_buf;
    int     send_buf_len = (int)strlen(tmp);
    int     len = send_buf_len - ret;
    char    *new_send_buf;
    int     step = 0;

    new_send_buf = calloc(sizeof(char), len + 1);
    check_fatal(new_send_buf);
    while (len + step < send_buf_len)
    {
        new_send_buf[step] = tmp[len + step];
        step++;
    }
    free(*send_buf);
    *send_buf = new_send_buf;
}