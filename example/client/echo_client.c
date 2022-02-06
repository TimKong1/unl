#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SERVER_PORT     8888

void sys_error(const char * err_msg, ...)
{
    va_list args;
    va_start(args, err_msg);
    printf(err_msg, args);
    va_end(args);
    exit(-1);
}


int main(int argc, char *argv[])
{
    int ret = 0;
    int fd = 0;
    struct sockaddr_in server_addr;
    char buf[BUFSIZ];

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    ret = inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);
    if (ret == -1){
        sys_error("inet_pton error");
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1){
        sys_error("socket_error");
    }
    ret = connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret != 0){
        sys_error("connect error");
    }
    while (1)
    {
        memset(buf,0,sizeof(buf));
        scanf("%s", buf);
        write(fd, buf, sizeof(buf));
        printf("send: %s\n", buf);
        memset(buf,0,sizeof(buf));
        read(fd, buf, sizeof(buf));
        printf("recive: %s\n", buf);
        sleep(1);
    }
    close(fd);
    
    return 0;
}
