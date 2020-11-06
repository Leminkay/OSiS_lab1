#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "func.h"
#define DEF_PORT 4311


char login[1024] = "emil", pass[1024] = "password"; 
int main(int argc, char *argv[]) {

	struct sockaddr_in addr;

	short int port;

	if (argc == 1) {
		port = DEF_PORT;
	} else {
		port = atoi(argv[1]);
	}
	
	int listner = socket(AF_INET, SOCK_STREAM, 0);
	if (listner < 0) {
		perror("ERROR: Cannot create socket.\n");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(listner, (struct sockaddr *)&addr , sizeof(addr)) < 0){
		perror("ERROR: Cannot connect.\n");
		return 2;
	}
	int sock_cl;
	int size_in;
	char buf1[1024], buf2[1024];
	char n[1024], m[1024];

	listen(listner, 3);
	while(1){

		sock_cl = accept(listner, NULL, NULL);
		perror("CONNECTED");
		if (sock_cl < 0){
			perror("ERROR: Can't accept socket.\n");
			return 3;
		}
		// getting login and password
		size_in = recv(sock_cl,buf1, sizeof(login), MSG_NOSIGNAL);
		if(size_in == 0) {
			perror("ERROR: client disconnected.\n");
			close(sock_cl);
			continue;
		}
		size_in = recv(sock_cl,buf2, sizeof(pass), MSG_NOSIGNAL );
		if(size_in == 0) {
			perror("ERROR: client disconnected.\n");
			close(sock_cl);
			continue;
		}

		char stat[1024];
		// auth check
		if (strcmp(buf1, login) != 0 || strcmp(buf2, pass) != 0){
			perror("ERROR: failed validation");
			strcpy(stat, "ERROR");
			send(sock_cl, stat, sizeof(stat), 0);
			close(sock_cl);
			continue;
		}
		strcpy(stat, "OK");

		send(sock_cl, stat, sizeof(stat), 0);


		//getting n and m
		size_in = recv(sock_cl, n, sizeof(n), MSG_NOSIGNAL);
		perror(n);

		if(size_in == 0) {
			perror("ERROR: client disconnected.\n");
			close(sock_cl);
			continue;
		}
		size_in = recv(sock_cl, m, sizeof(m), MSG_NOSIGNAL);
		perror(m);
		if(size_in == 0) {
			perror("ERROR: client disconnected.\n");
			close(sock_cl);
			continue;
		}
		

		int res = delta(atoi(n), atoi(m));
		char resC[1024];
		sprintf(resC, "%d", res);
		perror(resC);
		//send answer
		send(sock_cl, resC, sizeof(resC), 0);



		close(sock_cl);
	}
	close(listner);
	return 0;
}
