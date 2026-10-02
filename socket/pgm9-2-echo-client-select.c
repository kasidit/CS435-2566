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

int client_shutdown_flag = 0;

int conn_fd;
struct sockaddr_in serv_addr;

int main(int argc, char *argv[]){

        int n, m;
	fd_set base_rfds, rfds; 
        int fdmax = 0; 
        char line[MAXLINE];

	conn_fd = socket(AF_INET, SOCK_STREAM, 0); 

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	
	serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);

        if (connect(conn_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0) { 
            perror("Problem in connecting to the server");
            exit(3);
        }

	FD_ZERO(&base_rfds);
	FD_ZERO(&rfds);
	FD_SET(fileno(stdin), &base_rfds);
	FD_SET(conn_fd, &base_rfds);

	fdmax = conn_fd;

	while(1){
	  memcpy(&rfds, &base_rfds, sizeof(fd_set)); // copy it
          if (select(fdmax+1, &rfds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(4);
          }
	  
	  if(FD_ISSET(fileno(stdin), &rfds)){
            if(fgets(line, MAXLINE, stdin) == NULL){
	      printf("Shutdown writing to TCP connection\n");
	      shutdown(conn_fd, SHUT_WR);
	      client_shutdown_flag = 1;
	    }
	    else{
              n = write(conn_fd, line, MAXLINE);
              printf("send %s with n = %d characters\n", line, n);
	    }
	  }

	  if(FD_ISSET(conn_fd, &rfds)){
            if(read(conn_fd, line, MAXLINE) == 0){
	      if(client_shutdown_flag){
	        printf("TCP connection closed after client shutdown\n");
	        close(conn_fd);
	        break;
	      }
	      else{
	        printf("Error: TCP connection closed unexpectedly\n");
	        exit(1);
	      }
	    }
	    else{
              printf("receive %s with m = %d characters\n", line, m);
              fputs(line, stdout);
	    }
	  }
	}
}

