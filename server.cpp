/* run using ./server <port> */
#include "http_server.hh"
//#include "http_server.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <bits/stdc++.h>

pthread_mutex_t loc=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t conQ=PTHREAD_COND_INITIALIZER;

queue<int> socketQ;

pthread_t th[20];

void error(char *msg) {
  perror(msg);
  exit(1);
}


void  *threadFunc(void *socketId){
int sockId;
while(1){

 pthread_mutex_lock(&loc);
 while(socketQ.empty())
 pthread_cond_wait(&conQ,&loc);
 sockId=socketQ.front();
 socketQ.pop();
 pthread_cond_signal(&conQ);
 pthread_mutex_unlock(&loc);
 
  if(sockId){
  int portno;
  char buffer[255];
  int n;

  bzero(buffer, 255);
  n = read(sockId, buffer, 256);
  if (n < 0)

    error("ERROR reading from socket");



  HTTP_Response *response = handle_request(buffer);

  int len=0;
  char dataB[1024];
  len=response->body.length();
  for(int i=0;i<len;i++){
    dataB[i]=response->body[i];

}
  n = write(sockId, dataB,len);
  if (n < 0)
  error("ERROR writing to socket");
  }
  

close(sockId);

}
  return 0;
  }

  
void handle_sigint(int sig){
  exit(0);
}



int main(int argc, char *argv[]) {
   signal(SIGINT, handle_sigint);
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* listen for incoming connection requests */

  listen(sockfd, 100);
  clilen = sizeof(cli_addr);


  /*creating thread*/

  /* accept a new request, create a newsockfd */
for(int i=0;i<20;i++)
  if( pthread_create(&th[i],NULL,threadFunc,NULL)<0)
  error("Error in thread creating.");


  while(1){
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  if (newsockfd < 0){
    error("ERROR on accept");
    break;
  }
  int ns =newsockfd;
  pthread_mutex_lock(&loc);
  //(socketQ.size()<=3)
  //pthread_cond_wait(&conQ,&loc);
  socketQ.push(ns);
  pthread_cond_signal(&conQ);
  pthread_mutex_unlock(&loc);

  /* read message from client */
 
}

  close(sockfd);

for(int i=0;i<20;i++)
  pthread_join(th[i],NULL);

  return 0;
}