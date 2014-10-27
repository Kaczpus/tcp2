#include <iostream>
#include <cstring>      
#include <sys/socket.h> 
#include <netdb.h>      

void *sending(void* data)
{
	while(1)
	{
	int new_sd = *static_cast<int*>(data);

 	std::string message;
 	std::getline(std::cin, message);
  
    ssize_t bytes_sent;
    bytes_sent = send(new_sd, message.c_str(), message.size(), 0);   	
    }


}
void *receiving(void* data)
{
	while(1)
	{
	int new_sd = *static_cast<int*>(data);
	ssize_t bytes_recieved;
    char incomming_data_buffer[1000];
    bytes_recieved = recv(new_sd, incomming_data_buffer,1000, 0);
    if (bytes_recieved == 0) std::cout << "host shut down." << std::endl ;
    if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
    incomming_data_buffer[bytes_recieved] = '\0';
   	std::cout << incomming_data_buffer << std::endl;
   }
}

int main()
{
	int status;
    struct addrinfo host_info;       
    struct addrinfo *host_info_list; 

    
    memset(&host_info, 0, sizeof host_info);

    std::cout << "Setting up the structs..."  << std::endl;

    host_info.ai_family = AF_UNSPEC;     
    host_info.ai_socktype = SOCK_STREAM; 

    status = getaddrinfo("127.0.0.1", "5556", &host_info, &host_info_list);
    if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;


    std::cout << "Creating a socket..."  << std::endl;
    int socketfd ; 
    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
    if (socketfd == -1)  std::cout << "socket error " ;


    std::cout << "Connect()ing..."  << std::endl;
    status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1)  std::cout << "connect error" ;

	pthread_t send,receive;
  
  	pthread_create(&send,NULL,sending,&socketfd);
  	pthread_create(&receive,NULL,receiving,&socketfd);

  	pthread_join(send,NULL);
  	pthread_join(receive,NULL);
	

	return 0;
}    

