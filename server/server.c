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

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 1024

void main(int argc , char **argv){
	// struct sockadrr_in server;
	struct sockaddr_in server, client;
	int sock;
	int new;
	int sockaddr_len = sizeof(struct sockaddr_in);
	int data_len;
	char data[MAX_DATA];

	if((sock = socket(AF_INET,SOCK_STREAM,0)) == ERROR){
		perror("server socket : ");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(atoi(argv[1]));
	server.sin_addr.s_addr = INADDR_ANY;
	bzero(&server.sin_zero,8);

	if((bind(sock, (struct sockaddr *) &server, sockaddr_len))==ERROR){
		perror("bind : ");
		exit(-1);
	}

	if((listen(sock, MAX_CLIENTS))==ERROR){
		perror("listen");
		exit(-1);
	}

	while(1){
		if((new = accept(sock,(struct sockaddr *)&client,&sockaddr_len))==ERROR){
			perror("accept");
			exit(-1);
		}

		printf("NEW client connected from port no. %d and IP is %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));
		
		char input[100] = "Welcome ! u are connected.Please enter valid username and password";
		// data_len = sizeof(data);
		send(new, input, 100, 0);
		data_len = recv(new, data, MAX_DATA, 0);
		// data[data_len] = '\0';
		// printf("username:password : %s",data);

        char c[1024];
	    FILE *fptr;
	    int flag = 0;
	    if ((fptr = fopen("users.txt", "r")) == NULL)
	    {
	        printf("Error! opening file");
	        // Program exits if file pointer returns NULL.
	        exit(1);         
	    }
	    char username[100];
	    for(int i =0;i<data_len;i++){
	    	if(data[i]==':')
	    		break;
	    	else
	    		username[i]=data[i];
	    }
	    while (fgets(c, sizeof(c), fptr)) {
	        if(!strcmp(data,c)){
		        printf("User Authenticated\n");
		        printf("Hello %s\n",username);
		        flag = 1;
		    } 
	    }
	    if(!flag){
	    	char file[100] = "Authentication Failed";
			send(new, file, 100, 0);
		    close(new);
		}

	    fclose(fptr);

	    char file[100] = "Please enter the filename";
		send(new, file, 100, 0);

		data_len = recv(new, data, MAX_DATA, 0);
		FILE *fp;
	   	data[data_len] = '\0';
	   	char filename[100] = "file1.txt";
	   	for(int i=0;i<data_len-1;i++){
	   		filename[i] = data[i];
	   	}
	    if ((fp = fopen(filename, "r")) == NULL)
	    {
	        printf("Error! opening file");
	        // Program exits if file pointer returns NULL.
	        exit(1);         
	    }
	    else{
	    	printf("Sending File %s",data);
		    while(1)
	        {
	            /* First read file in chunks of 256 bytes */
	            unsigned char buff[256]={0};
	            int nread = fread(buff,1,256,fp);
	            printf("Bytes read %d \n", nread);        

	            /*Sending file*/
	            if(nread > 0)
	            write(new, buff, nread);

	            if (nread < 256)
	            {
	                if (feof(fp))
	                    printf("End of file\n");
	                if (ferror(fp))
	                    printf("Error reading\n");
	                break;
	            }

	        }
	    	fclose(fp);
		}

		printf("client disconnected\n");

			close(new);
		}

		close(sock);
}
