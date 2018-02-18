#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void tcp_client(int argc, char **argv);
int main(int argc, char **argv)
{
	tcp_client(argc, argv);
	return 0;
}

void tcp_client(int argc, char **argv)
{
	if(argc < 3)
	{
		perror("please input the ip and port you want to connect!\n");
		exit(-1);
	}
	
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	
	printf("ip = %s,port = %d\n", ip, port);
	
	int ret = 0;
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);
	
	int SEND_SIZE = 4096;
	int len = sizeof(SEND_SIZE);
	setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &SEND_SIZE, sizeof(SEND_SIZE));
	getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &SEND_SIZE, (socklen_t*)len);
	printf("the size of sending buffer is %d\n", len);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	
	//convert the address into internet address
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	//connect to server
	ret = connect(sock, (struct sockaddr *) &addr, sizeof(addr));
	assert(ret >= 0);
	
	//send data to server
	const char *oob_data = "the is the oob data from client!";
	const char *data = "nothing to say just use the client!";
	
	//as the server act, client send three message
	send(sock, data, strlen(data), 0);
	send(sock, oob_data, strlen(oob_data), 0);
	send(sock, data, strlen(data), 0);
	
	close(sock);        //close socket
}
