#include <iostream>
#include  <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>


/*for getting file size using stat()*/
#include<sys/stat.h>
/*for sendfile()*/
#include<sys/sendfile.h>
/*for O_RDONLY*/
#include<fcntl.h>

#define MAX 10

using namespace std;

int connfd[MAX];
pthread_t threads[MAX];


//==============READ FUNCTION=====================

void *read(void *arg){
	map<int,int> menu;
	menu[1] = 5;
	menu[2] = 30;
	menu[3] = 50;
	menu[4] = 150;
	menu[5] = 180;
	struct stat obj;
	int* conn = (int *)arg;
	int file_handler;
	file_handler = open("menu.txt",O_RDONLY);
	stat("menu.txt",&obj);
	int size = obj.st_size;
	send(*conn,&size,sizeof(int),0);
	sendfile(*conn,file_handler,NULL,size);
	int order=0,quant = 0;
	int bill=0;
	while(1){
		recv(*conn,&order,sizeof(int),0);
		if(order==-1){
			send(*conn,&bill,sizeof(int),0);
			cout<<"Client "<<*conn<<" thanks for being with us!!!!\n\n";
			break;
		}
		recv(*conn,&quant,sizeof(int),0);
		bill+=quant*menu[order];
	}
		
}

//==================MAIN FUNCTION==================
int main(){
	int listenfd = 0;
	struct sockaddr_in server_addr;
	
	listenfd = socket(AF_INET,SOCK_STREAM,0);
			
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(7500);

	bind(listenfd,(struct sockaddr*)&server_addr, sizeof(server_addr));
	
	listen(listenfd,MAX);

	while(1){
		for(int i = 0;i<MAX;i++){
			connfd[i]=accept(listenfd,(struct sockaddr*)NULL,NULL);
			cout<<"CLIENT "<<connfd[i]<<" connected\n";
			pthread_create(&threads[i],NULL,&read,&connfd[i]); 
		}	
		for(int i=0;i<MAX;i++){
			pthread_join(threads[i],NULL);	
		}
	}		




}
