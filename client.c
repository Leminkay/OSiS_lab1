#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#define DEF_PORT 4311


char login[1024] = "emil", pass[1024] = "password"; 
int main(int argc, char *argv[]) {

	char msg[1024] = "QQ\n", buf[1024];
	struct sockaddr_in addr;

	short int port;

	if (argc == 1) {
		port = DEF_PORT;
	} else {
		port = atoi(argv[1]);
	}
	
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("ERROR: Cannot create socket.\n");
		return 1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if (connect(sock, (struct sockaddr *)&addr , sizeof(addr)) < 0){
		perror("ERROR: Cannot connect.\n");
		return 2;
	}

	//send login ans pass
	send(sock,login, sizeof(login), MSG_NOSIGNAL | MSG_DONTROUTE);
	send(sock,pass, sizeof(pass), MSG_NOSIGNAL | MSG_DONTROUTE);
	// get auth answer
	int size_in = recv(sock, buf, 1024, MSG_NOSIGNAL);
	printf("%s\n", buf);
	if(strcmp("OK", buf) != 0){
		perror("ERROR: Can't login to server.\n");
		close(sock);
		return 3;
	}
	
	//ask for n and m
	char n[100];

	printf("Please enter n.\n");
	scanf("%s", n);

	char m[100];
	printf("Please enter m.\n");
	scanf("%s", m);
	// send n and m to server
	perror(n);
	send(sock, n, 1024, MSG_NOSIGNAL | MSG_DONTROUTE);
	send(sock, m, 1024, MSG_NOSIGNAL | MSG_DONTROUTE);
	// get answer
	char nbuf[1024];
	size_in = recv(sock, nbuf, 1024, MSG_NOSIGNAL);
	printf("%s\n", nbuf);
	
	
	close(sock);
	
	return 0;
}
