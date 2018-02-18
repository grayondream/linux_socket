/**
 * a simple socket progra which use the tcp and ipv4,and just receive few information from client without any response
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

/**
 * buffer size
 */
#define BUFF_SIZE 1024

void tcp_server(int argc,char **argv);

int main(int argc,char **argv)
{
	tcp_server(argc, argv);
	return 0;
}

void tcp_server(int argc,char **argv)
{
	if(argc < 4)
	{
		perror("the parameter is not enough!You should input ip ,port and the limitation of connection number\n");
		exit(-1);
	}
	
	const char *ip = argv[1];                   //ip
	int   port = atoi(argv[2]);                 //port
	int   con_count = atoi(argv[3]);            //the limitation of connection number
	printf("ip is %d .port is %d .backlog is %d.\n", ip, port, con_count);
	
	int ret = 0;
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(sock >= 0);                          //if the socket is invaild abort
	
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));                 //write zero into the struct
	
	//conver the address into internet address for the sequence of net address is different in different mechine
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;                  //ipv4
	//convert ip address
	inet_pton(AF_INET, ip, &addr.sin_addr);
	
	int recv_buff = 4096;
	int len = sizeof(recv_buff);
	setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recv_buff, sizeof(recv_buff));
	getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &recv_buff, (socklen_t *)&len);
	printf("the size of receive buffer is %d.\n", len);
	
	//bind ip and port into socket
	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	assert(ret >= 0);
	
	//listen the port to receive request from client
	ret = listen(sock, con_count);
	assert(ret >= 0);
	
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	//accept request from client
	int conn_sock = accept(sock, (struct sockaddr*)&client_addr, &client_len);
	if(conn_sock < 0)
	{
		perror("accept the connection from client something error occur!\n");
	}
	else
	{
		char client_info[INET_ADDRSTRLEN];          //information of client
		printf("%s:%ud\n", inet_ntop(AF_INET,&client_addr.sin_addr,client_info,INET_ADDRSTRLEN),ntohs(client_addr.sin_port));
		
		char buffer[BUFF_SIZE];
		size_t ret = 0;
		
		memset(buffer, '\0', sizeof(buffer));
		ret = recv(conn_sock, buffer, BUFF_SIZE - 1, 0);    //receive data fomr client
		printf("return code = %d,msg = %s.\n", ret, buffer);
		
		memset(buffer, '\0', sizeof(buffer));
		ret = recv(conn_sock, buffer, BUFF_SIZE - 1, MSG_OOB);    //receive data fomr client
		printf("return code = %d,msg = %s.\n", ret, buffer);
		
		memset(buffer, '\0', sizeof(buffer));
		ret = recv(conn_sock, buffer, BUFF_SIZE - 1, 0);    //receive data fomr client
		printf("return code = %d,msg = %s.\n", ret, buffer);
		
		struct sockaddr_in tmp_addr;
		client_len = sizeof(tmp_addr);
		//get the information of local socket
		getsockname(sock, (struct sockaddr *)&tmp_addr, &client_len);
		printf("%s:%d.\n",inet_ntop(AF_INET, &tmp_addr.sin_addr, buffer, INET_ADDRSTRLEN), ntohs(tmp_addr.sin_port));
		
		//get the information of remote client socket
		getsockname(conn_sock, (struct sockaddr *)&tmp_addr, &client_len);
		printf("%s:%d.\n",inet_ntop(AF_INET, &tmp_addr.sin_addr, buffer, INET_ADDRSTRLEN), ntohs(tmp_addr.sin_port));
		
		close(conn_sock);
	}
	
	close(sock);
}