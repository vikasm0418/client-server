#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

#define BUFFER 1024

void main(int argc, char **argv)
{
	struct sockaddr_in server;
	int sock;
	char input[BUFFER];
	char output[BUFFER];
	int len;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[2]));
	server.sin_addr.s_addr = inet_addr(argv[1]);
	bzero(&server.sin_zero,8);

	if((connect(sock, (struct sockaddr *)&server,sizeof(struct sockaddr_in)) == -1)){
		perror("connect");
		exit(-1);
	}

	len = recv(sock, output, BUFFER, 0);
	output[len] = '\0';
	printf("%s\n",output);
	
	fgets(input, BUFFER, stdin);
	send(sock, input, strlen(input), 0);

	len = recv(sock, output, BUFFER, 0);
	output[len] = '\0';
	printf("%s\n",output);

	fgets(input, BUFFER, stdin);
	send(sock, input, strlen(input), 0);

	char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff));
    int bytesReceived = 0;

	FILE *fp;

    if ((fp = fopen(input, "ab")) == NULL)
	    {
	        printf("Error! opening file");
	        // Program exits if file pointer returns NULL.
	        exit(1);         
	    }

    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(sock, recvBuff, 256)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
	// while(1){
	// 	fgets(input, BUFFER, stdin);
	// 	send(sock, input, strlen(input), 0);

	// 	len = recv(sock, output, BUFFER, 0);
	// 	output[len] = '\0';
	// 	printf("%s\n",output);
	// }
	close(sock);
}
