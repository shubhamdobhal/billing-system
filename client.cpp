#include<iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <signal.h>

/*for getting file size using stat()*/
#include<sys/stat.h>
/*for sendfile()*/
#include<sys/sendfile.h>
/*for O_RDONLY*/
#include<fcntl.h>

using namespace std;

int main(int argc,char *argv[]){
	int sockfd;
	struct sockaddr_in client_addr;

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	
	memset(&client_addr,'0',sizeof(client_addr));

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(7500);	
	
	if(inet_pton(AF_INET, argv[1], &client_addr.sin_addr)<=0)
   	{
        	cout<<"\n inet_pton error occured\n";
        	return 1;
    	} 

	if(connect(sockfd,(struct sockaddr *)&client_addr,sizeof(client_addr))<0){
		cout<<"Connection Failed!!\n";
		return 1;
	}
	
	int size = 0;
	recv(sockfd,&size,sizeof(int),0);
	//cout<<size<<endl;
	char *f = new char[size];
 	recv(sockfd,f,size,0);
		
	cout<<string(f)<<endl;
	
	cout<<"\nPlace your order :: ";
	int order;
	int bill=0;
	while(1){
		cin>>order;
		send(sockfd,&order,sizeof(int),0);
		if(order==-1){
			recv(sockfd,&bill,sizeof(int),0);
			cout<<"\n\nBill = "<<bill<<endl;
			break;
		}
		cout<<"Quantity :: ";
		cin>>order;
		send(sockfd,&order,sizeof(int),0);
		cout<<"more order :: ";
	}
	return 0;
}
