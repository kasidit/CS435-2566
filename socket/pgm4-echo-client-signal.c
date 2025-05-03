/*
 A simple TCP client program 
 Kasidit Chanchio (kasiditchanchio@gmail.com)
*/
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP		"127.0.0.1"
#define SERV_PORT 	18800

#define	MAXLINE	100

#include <signal.h>

int accept_cr(int fd, struct sockaddr *addr, socklen_t *len);
int read_full(int fd, void *buf, size_t count);
int write_full(int fd, const void *buf, size_t count);

static void handler(int sig){
   printf("signal received %d\n", sig);
   fflush(stdout);
}

int conn_fd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int n, m;
        char line[MAXLINE];

	if(signal(SIGINT, handler) == SIG_ERR){
	    exit(1);
	}

	conn_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

        if (connect(conn_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
            perror("Problem in connecting to the server");
            exit(3);
        }

        while (fgets(line, MAXLINE, stdin) != NULL) {

            n = write_full(conn_fd, line, MAXLINE);
            printf("send %s with n = %d characters\n", line, n);
            m = read_full(conn_fd, line, MAXLINE);
            printf("receive %s with m = %d characters\n", line, m);

            fputs(line, stdout);

        }

	close(conn_fd);

}

// -----Basic Communication Utilities-----
int accept_cr(int fd, struct sockaddr *addr, socklen_t *len){
	int ret;
repeat_accept:
        ret = accept(fd, addr, len);
        if (ret < 0) {
            if (errno == EINTR){
                goto repeat_accept;
	    }
	    printf("accept error errno=%d fd=%d\n", errno, fd);
        }
	return ret;
}

int write_full(int fd, const void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = write(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){
                continue;
	    }
	    printf("write error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}

int read_full(int fd, void *buf, size_t count){
    ssize_t ret = 0;
    ssize_t total = 0;

    while (count) {
        ret = read(fd, buf, count);
        if (ret < 0) {
            if (errno == EINTR){ 
                continue;
	    } 
	    printf("read error errno=%d fd=%d\n", errno, fd);
            return ret;
        }
        else if (ret == 0){
            return ret; 
        }

        count -= ret;
        buf += ret;
        total += ret;
    }

    return total;
}

