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


//LRU Data Type
typedef struct{
	char Domain_Name[1][1000];
	char Page[1][1000];
	char Body[1][1000];
	int expired[1];
	//time last modified
	time_t LA_time[1];
	//last header information
	//expires header information
	//payload header would go here not sure what this is
}LRU_Cache;




int main()
{
	//Declare socket variables
	int serv_sock, new_sock,sock_act,port_num,master_check,max_clients,n,rw_check,getaddr_check,i,j,k,a,c,d,conn_act,recv_act_check,LRU_check,LRU_pointer;
	LRU_check=0;
	LRU_pointer=0;
	socklen_t length_client;
	struct sockaddr_in serv_addr, client_addr,serv_act_addr;
	struct timeval timeout;
	struct sockaddr_storage c_addr_stored;
	struct addrinfo hints, *res, *p; 	
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; 
	char Act_IP_str[INET6_ADDRSTRLEN];	
	time_t curr_time;
	LRU_Cache LRU;
	int time_thresh;
	time_thresh=86400; //not in last 24 hours
	//Declare FD_Set Variables
	fd_set read_fds;
	fd_set read_new;
	FD_ZERO(&read_fds);
	FD_ZERO(&read_new);

	//Buffer Variables
	char buffer[1000];
	char buffer_act[1000];
	int buffer_size=sizeof(buffer); 
	char serv_addr_str[100];
	char URL_Addr[1000]; 
   	 char Host_name[1000];
    char Page[1000];
	char GET_check[100];
	char Host_Check[100];
	char Footer_Check[100];  
	max_clients=10; 
	int client_count=0;
	char Get_Req_Act[1000];

	//take user inputs for IP Address and Port Number 
	printf("Enter the IP Address\n");
	scanf("%s",serv_addr_str);
	inet_pton(AF_INET, serv_addr_str, &(serv_addr.sin_addr));

	printf("Enter the port number:\n");
	scanf("%d",&port_num);

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
		if(serv_sock<0)
		{
		printf("Error Attempting to Open Socket\n");
		}
		printf("Socket Created\n"); 

	//allow multiple connections
		int opt=1; 
		if( setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,sizeof(opt))<0)
		{
		printf("Error allowing multiple connections on server socket\n");	
		}

	//bind socket
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port_num);	
		if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		{
		printf("Error Attempting to Bind Socket\n"); 
		perror("Error"); 
		}
	printf("Socket is Bound and Listening\n");

	//listen for client
	listen(serv_sock,max_clients);
	printf("Listen complete\n");

	length_client = sizeof(client_addr);
	timeout.tv_sec = 30; //timeout of 30 seconds	
	n=serv_sock;  //n will be used to compute largest fds
			
	FD_SET(serv_sock,&read_new); //add server socket to FD list
	// Accept Socket
	//new_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &length_client);
	//printf("Socket Accepted \n"); 
	master_check=1;
	while(master_check)
	{
   	//Insert Loops for Select Function
	//Update FD Set
		printf("Entered While Loop\n"); 
		read_fds=read_new;
				
		rw_check = select (n+1,&read_fds, NULL, NULL, NULL); //replace NULL with timeout value if desired
		if(rw_check < 0) 
		{
		printf("Error In Select Function\n");
		}
		for(i=0; i<=n;i++)
		{
			if(FD_ISSET(i, &read_fds))	//Data received on the ith connection
			{
				if(i==serv_sock) //Data received on main server so client trying to connect
				{
					if(client_count<(max_clients+1))
					{
					new_sock = accept(serv_sock, (struct sockaddr *) &client_addr, &length_client);
						if(new_sock<0)
						{
						printf("Error Accepting Socket\n");
						}
						else
						{
						client_count=client_count+1; 
						FD_SET(new_sock, &read_new); //add to FD set
							if(new_sock>n)
							{
							n=new_sock; //new largest fds 
							}
						printf("socket accepted\n");
						} 
					}
					else
					{
					printf("Client cannot be added because chat room is full\n");
					}
				//FD_CLR(i,&read_new);
				}
				else 
				{
					//Receive Message 
					int recv_check=recv(new_sock,buffer,buffer_size,0);
					if(recv_check<=0)
					{
					FD_CLR(i,&read_new);
					}
					//Parse Buffer for Host Name and Page
					for( j=0; j<100; j++)
					{
						if (buffer[j]=='H' && buffer[j+1]=='T' && buffer[j+2]=='T' && buffer[j+3]=='P')
						{
						strncpy(Page,buffer+4,j-5);
						a=j;
						//Host_name[i]='\0';
						//printf("Host Name %s",Host_name); 
						//break;
						}
					} 
					for( k=0; k<100; k++)
					{
						if (buffer[k+2]=='U' && buffer[k+3]=='s' && buffer[k+4]=='e' && buffer[k+5]=='r')
						{
						strncpy(Host_name,buffer+a+16,k-a-16);
						//Host_name[i]='\0';
						//printf("Host Name %s",Host_name); 
						//break;
						}
					} 
				//printf("Host name is %s \n", Host_name);
				printf("Buffer is %s\n",buffer);	
				printf("Host Name is %s\n",Host_name); 
				printf("Page is %s\n",Page);

				//Use Host Name and Page to check if in LRU Cache
				for(d=0;d<10;d++)
				{				
				if((strcmp(Host_name,LRU.Domain_Name[d])==0) && (strcmp(Page,LRU.Page[d])==0))
					{
					//send to client
					LRU_check=1;
					write(i,LRU.Body[d],strlen(LRU.Body[d]));
					(void) time(&LRU.LA_time[d]);
					}
	
				}
				if(LRU_check==1)
				{
				//send cached document
				LRU_check=0;
				printf("Data forwarded from cache\n");
				}
				else	
					{
					//Find IP Address of Actual Server using getaddrinfo
					if((getaddr_check=getaddrinfo(Host_name,NULL,&hints,&res))!=0)
					{
					printf("Error on Get Addr Info\n"); 
					}
					
					//for(p=res; p!= NULL; p=p->ai_next)
					//{
					p=res;
					void *addr;
					if(p->ai_family==AF_INET)
					{
					struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
					addr=&(ipv4->sin_addr);
					}		
					//else
					//{
					//struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
					//addr=&(ipv6->sin6_addr);
					//}
					inet_ntop(p->ai_family,addr,Act_IP_str,sizeof Act_IP_str); 
					printf("IP Address of Request is%s\n",Act_IP_str);
					inet_pton(AF_INET, Act_IP_str, &(serv_act_addr.sin_addr));
					printf("Ip add %d",&serv_act_addr);
					//Create Socket using Info
					
					sock_act=socket(AF_INET,SOCK_STREAM,0); //create socket
    						if (sock_act<0)
    						{
						printf("Error creating socket\n");exit(1);
						}
					    	else
    						{
						printf("Socket is successfully created\n");
						}

					//memset(&serv_act_addr,'\0',sizeof(serv_act_addr)); //zero structure out
        				serv_act_addr.sin_family=AF_INET;
 				  	// memcpy((char *)&serv_addr.sin_addr.s_addr,serv->h_addr_list[0],serv->h_length);  //copy the address
    					serv_act_addr.sin_port=htons(80);

					//Connect to Actual Server
					conn_act=connect(sock_act,(struct sockaddr*) &serv_act_addr,sizeof(serv_act_addr));
    					if (conn_act< 0)
    					{printf("Error connecting with error code %d\n",conn_act);}
    					else
    					{
					printf("Proxy Server is connecting to the Actual Server\n");
					//REQ_loop=1;		
					}				
					//Send the Get Request to Actual Server
					sprintf(Get_Req_Act,"GET /%s HTTP/1.0\r\nHost: %s\r\n\r\n",Page,Host_name);
					printf("Length of Get Request is %d",strlen(Get_Req_Act)); 
					//Send Get Request 
    					write(sock_act,Get_Req_Act,strlen(Get_Req_Act));
					bzero(Get_Req_Act,sizeof(Get_Req_Act));
					//Receive From Actual Server
					while(recv_act_check=recv(sock_act,buffer_act,sizeof(buffer_act),0)>0)
					{
					//Keep reading from the buffer 
					}
					printf("Message Received from Actual Server \n"); 
					printf("Buffer from actual server is %s\n",buffer_act); 
					//Forward to Client
					printf("Forwarding data to client \n");
					write(i,buffer_act,strlen(buffer_act));
					printf("Data forwarded to client \n"); 
					//}
				//Update LRU pointer to an open slot
				int LRU_open_check,open_check;
				LRU_open_check=0;
				for(open_check=0; open_check<10;open_check++)
				{
					//Check for Expired Packets
					(void) time (&curr_time);
					if((curr_time-LRU.LA_time[open_check])>time_thresh)
					{
					LRU.expired[open_check]==1;
					}
					if(strcmp(LRU.Domain_Name[open_check],"")==0)
					{
					sprintf(LRU.Domain_Name[open_check],"%s",Host_name);								
					sprintf(LRU.Page[open_check],"%s",Page);
					sprintf(LRU.Body[open_check],"%s",buffer_act);
					(void) time(&LRU.LA_time[open_check]);
					LRU_open_check=1;
					break;
					}
				}
				//If Open Slot is unavailable, pick an expired slot to replace 
				int exp_check,LRU_exp_check;
				int LRM_pointer;
				int LRU_oldest;
				LRU_oldest=LRU.LA_time[0];
				LRM_pointer=0;
				LRU_exp_check=0;
				if(LRU_open_check==0)
				{
				for(exp_check=0;exp_check<10;exp_check++)
				{
					if(LRU.LA_time[exp_check]<LRU_oldest)					
					{					
					LRM_pointer=exp_check;
					LRU_oldest=LRU.LA_time[exp_check];
					}
					if(LRU.expired[exp_check]==1)
					{
					sprintf(LRU.Domain_Name[exp_check],"%s",Host_name);								
					sprintf(LRU.Page[exp_check],"%s",Page);
					sprintf(LRU.Body[exp_check],"%s",buffer_act);
					(void) time(&LRU.LA_time[exp_check]);
					LRU_exp_check=1;
					break;
					}
				}
				}
				//If Expired Slot is unavailabe, pick Least Recently Modified
				int LRM_check,LRU_LRM_check;
				LRU_LRM_check=0;
				if(LRU_exp_check==0 && LRU_open_check==0)
				{
				sprintf(LRU.Domain_Name[LRM_pointer],"%s",Host_name);								
				sprintf(LRU.Page[LRM_pointer],"%s",Page);
				sprintf(LRU.Body[LRM_pointer],"%s",buffer_act);
				(void) time(&LRU.LA_time[LRM_pointer]);
				} 
				bzero(Host_name,sizeof(Host_name));
				bzero(Page,sizeof(Page));
				//FD_SET(0,&readfd);
				//FD_SET(sockfd,&readfd);
				LRU_check=0; 
				}	
				}//End of Else Statement

			
		} //End of Incoming Data

	} //FD Iterations
	} //while loop 
printf("Exiting Program");
return 0; 
} //main loop
