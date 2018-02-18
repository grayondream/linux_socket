#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void udp_client(int argc, const char **argv);

int main(int argc, const char * argv[])
{
	udp_client(argc, argv);
	return 0;
}

void udp_client(int argc, const char **argv)
{
	if(argc < 3)
	{
		printf("input your server's ip and port!\n");
		exit(1);
	}
	
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	printf("address is %s:%d\n", ip, port);
	
	int ret = 0;
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	assert(sock >= 0);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	const char *data = "just for test the udp client and server!";
	ret = sendto(sock, data, strlen(data), 0, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1)
	{
		perror("send data to server failed!\n");
		exit(1);
	}
	
	printf("send data successfully!\n");
	
	close(sock);
}
