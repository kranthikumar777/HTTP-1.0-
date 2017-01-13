# HTTP-1.0-
ECEN602 Programming Assignment 03
----------------------------------------------
Team Number:23
Member 1: Kranthi Kumar Katam  (UIN: 225009204)
Member 2: Matthew Roe          (UIN: 321007055)
Member 3: Jifang Mu            (UIN: 821005313)
-----------------------------------------------
In this project assignment, we built a http proxy server and client. The proxy server has a LRU cache used to store up to 10 documents which are replaced if expired (24 hours after requested) and then based on date last requested. The client uses a get command to request a file, the server takes the request and checks against cache, if file is found it sends to client otherwise it forwards request to website, adds document to cache and sends the document to client.
----------------------------------------------
The package contains 3 files:
1. http_final_serv.c
2. http_final_client.c
3. make_http
(Not used with make file) 
4. http_proxy_serv.c
5. http_proxy_client.c
-----------------------------------------------
Functions implemented:
write() This is used to send TCP packets
recv() This is used to receive TCP packets
time() This is used to determine current time of system
select() This is used to handle multiple clients

-----------------------------------------------
http_proxy_serv.c
1) Take user input via printf() and scanf() to receive the IP address and Port for the program 
2) Create a TCP socket, bind the socket using the port number provided
3) Select function sees data on main socket, this is a client attempting to connect. Use accept function and add to set of connections
4) Use a receive function in a loop to accept the get request. 
5) Parse the get request to obtain the host name and page of the document
6) Host name and page match an entry in the cache, check to see if document is "fresh" if so send data. Else send data request to actual server...
7) Data is not cached, so server must turn into client for the actual server. A socket needs to be created and connected to actual server
8) Send get request to actual server
9) Receive data from actual server
10) Forward data to client
11) This function loops through the cache to create a pointer to the data that will be filled. This loops through looking for an empty spot, if that fails it looks for an expired slot. If both of the previous fails, it looks for the least recently accessed. 
12) Store new document into the LRU cache in the correct spot using the LRU pointer
13) This is the cache of documents. It stores the host name, page, body, expiration status, and time it was last accessed for each document.

http_proxy_client.c 
1) Take user input for IP address and Port Number
2) Take in URL for the request
3) Take in file name to be written to
4) Open file to be written to
5) Parse url for host name and page
6) Send get request to server
7) Receive data from Server
8) Write data to file specified   
-----------------------------------------------

To compile the code using make file run the makefile***: make -f make_http
Run the server by using the command line: ./http_serv
Run the client by using the command line: ./http_client

To compile the code without make file run the following commands in command line
gcc http_proxy_serv.c -o http_serv
gcc http_proxy_client.c -o http_client
./http_serv
./http_client

***Code written with make file has problem finding file matches in the cache whereas the code written to run on command line has warnings left for make file but utilizes cache properly
-----------------------------------------------
Structure of Code
Server 

(13) LRU Cache to store documents 
int main()
{
//Declare Variables

(1) Take user input for IP address and Port Number 
(2) Create and Bind Socket
while(true) //used to take in get requests and data
	{
	if(connection on main socket)
	{
	(3) Accept Socket
	}
	else //Get request accepted
	{
	(4) Receive get request 
	(5) Parse request for Host name and Page
	if(Host Name && Page match LRU cache) //check if host_name and page are in the cache
	{
	(6) Send cache data to client
	}
	else
	{
	(7) Open socket to actual server
	(8) Send get request to actual server
	(9) Receive data from actual server
	(10) Forward data to client
	(11) Function to update LRU pointer
	(12) Store Document in LRU 
	}
	}	
	}
}

Client
int main()
{
//Declare Variables
(1) Take user input for IP address and Port Number 

while(true) //used so client can request multiple documents
{
(2) Take in URL for the request
(3) Take in file name to be written to
(4) Open file to be written to
(5) Parse url for host name and page
(6) Send get request to server
(7) Receive data from Server
(8) Write data to file specified 
}
}
