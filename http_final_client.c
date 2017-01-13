//Insert Header Files
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>

int main()
{
    int sockfd,port_num,connfd,REQ_loop;//,recv_check;
    REQ_loop=0;
    struct sockaddr_in serv_addr;
    //struct hostent *serv;
    //struct timeval tv;
    //struct message msg;
    fd_set readfd;
    //int fdmax;

    char serv_addr_str[100];
    char URL_Addr[1000]; 
    char Host_name[1000];
    char Page[1000]; 
    //char buffer[1000];
    char buffer_rcv[1000];
    char Get_Req[1000]; 
    char file_name[1000];
    FILE *fP; //File pointer

	//Take in the IP Address and Port of the Server 
	printf("Enter the IP Address\n");
	scanf("%s",serv_addr_str);
	inet_pton(AF_INET, serv_addr_str, &(serv_addr.sin_addr));
	
	printf("Enter the port number:\n");
	scanf("%d",&port_num);

 	sockfd=socket(AF_INET,SOCK_STREAM,0); //create socket
    	if (sockfd<0)
    	{printf("Error creating socket\n");exit(1);}
    	else
    	{
	printf("Socket is successfully created\n");
	}
    
	memset(&serv_addr,'\0',sizeof(serv_addr)); //zero structure out
        serv_addr.sin_family=AF_INET;
   	// memcpy((char *)&serv_addr.sin_addr.s_addr,serv->h_addr_list[0],serv->h_length);  //copy the address
    	serv_addr.sin_port=htons(port_num);
	//printf("IP Address used to connect is %d\n", &serv_addr); 
   	connfd=connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr));
    		if (connfd< 0)
    		{printf("Error connecting\n");}
    		else
    		{
		printf("Client is connecting to the server\n");
		REQ_loop=1;		
		}

	//Clear the socket set
   	FD_ZERO(&readfd);
    	FD_SET(0,&readfd);
    	FD_SET(sockfd,&readfd);
    //While Loop for Each URL Request 
	while(REQ_loop==1)
	{
	//Enter the URL to be requested
	printf("Enter the URL of the Document\n");
	if (FD_ISSET(0,&readfd))
         {
	scanf("%s",URL_Addr);

	printf("Enter the filename (used to write to)\n");
	scanf("%s",file_name);
	fP=fopen(file_name,"w"); //write file
	
	//Parse URL for Domain Name and Page
	int i; 	
	for( i=0; i<strlen(URL_Addr); i++)
	{
		if (URL_Addr[i]=='/')
		{
		strncpy(Host_name,URL_Addr,i);
		Host_name[i]='\0';
		break;
		}
	} 
	for(i=i+1;i<strlen(URL_Addr); i++)
		{
		strcat(Page,&URL_Addr[i]); 
		printf("Page is equal to %s on iteration %d \n",Page,i); 
		break; 
		}
	
	printf("Host Name is %s\n", Host_name); 
	printf("Page is %s\n", Page);
	sprintf(Get_Req,"GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: HTMLGET 1.0\r\n\r\n",Page,Host_name);
	//printf("Length of Get Request is %d\n",strlen(Get_Req)); 
	//Send Get Request 
    	write(sockfd,Get_Req,strlen(Get_Req));
  	//{
	//error("Error writing to socket\n");
	//}
	printf("Waiting for Data to be received\n"); 
		recv(sockfd,buffer_rcv,sizeof(buffer_rcv),0); 
		//{
		//printf("recv_check is %d\n",recv_check);
		//Keep reading from the buffer 
		//}
	printf("Data received \n"); 
	printf("Buffer from Proxy Server is %s\n",buffer_rcv); 
	fputs(buffer_rcv, fP);
	fclose(fP);
	bzero(Page,strlen(Page));
	}
	}
	FD_SET(0,&readfd);
	FD_SET(sockfd,&readfd);	
	return 0;
}
