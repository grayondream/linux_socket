/**
 * the is a linux socket program which use udp and ip
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void udp_server(int argc, char **argv);

int main(int argc, char **argv)
{
	udp_server(argc, argv);
	return 0;
}

void udp_server(int argc, char **argv)
{
	if(argc < 4)
	{
		perror("you should input the ip,port and limitation of connection number!\n");
		exit(-1);
	}
	
	const char *ip = argv[1];
	int port = atoi(argv[2]);
	int conn_limation = atoi(argv[3]);
	int ret = 0;
	
	printf("ip is %s,port is %d,the limation of connection is %d\n", ip, port,conn_limation);
	
	int sock = socket(PF_INET, SOCK_DGRAM, 0);
	assert(sock >= 0);
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	
	//convert the addr into internet address
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	//bind ip and port
	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	assert(ret >= 0);
	while(1)
	{
		char buffer[4096];
		
		struct sockaddr_in tmp_addr;
		int len = sizeof(tmp_addr);
		//get the information of local socket
		getsockname(sock, (struct sockaddr *)&tmp_addr, &len);
		printf("loal information:%s:%d.\n",inet_ntop(AF_INET, &tmp_addr.sin_addr, buffer, INET_ADDRSTRLEN), ntohs(tmp_addr.sin_port));
		
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		memset(buffer, '\0', 4069);
		ret  = recvfrom(sock, buffer, 4096 - 1, 0, (struct sockaddr *)&client_addr, &client_len);
		if(ret == -1)
		{
			perror("receive data from client failed!\n");
			exit(1);
		}
		
		printf("data from client is:%s\n", buffer);
		
	}
	
	close(sock);
}
