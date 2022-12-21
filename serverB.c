
#include "serverB.h"
#include "socketHelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
char ** get_file_to_parse(char * fileName, char **myString,int MAX_LENGTH){
  FILE *fp=fopen(fileName,"r");
  int ii=0;
  //const unsigned MAX_LENGTH = 256;
  char  buffer[MAX_LENGTH];
  //char *myString[MAX_LENGTH];
  
  //memset(&myString, 0,sizeof(myString) );
   while(fgets(buffer,MAX_LENGTH,fp)){
     // myString[ii] = malloc(strlen(buffer)+1);
     strncpy(myString[ii],buffer,strlen(buffer));
     ii++;
     //printf("%s",buffer); 
   }
   fclose(fp);
   return myString;
}
int main(){
  //creating a udp socket 
 struct addrinfo hints, *res;
 memset(&hints, 0, sizeof hints);
 hints.ai_family = PF_INET; // don't care IPv4 or IPv6
 hints.ai_socktype =SOCK_DGRAM; // UDP stream sockets (TCP SOCK_STREAM
 hints.ai_flags = AI_PASSIVE; // fill in my IP for me
 //udp port 21623
 printf("The ServerB is up and running using UDP on port 22623 \n");
 getaddrinfo(NULL,"22623",&hints,&res);
 
 int socket_desc;
 struct sockaddr_in server_addr_M;
 memset(&server_addr_M,0,sizeof(server_addr_M));
 int socket_M_desc = construct_udp_to_send(24623, &server_addr_M);
 
 socket_desc = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
    if( socket_desc== -1)
    {
      printf( "Could not create socket\n");
    }
    int isBinded;
    isBinded=bind(socket_desc,res->ai_addr,res->ai_addrlen);
     if(isBinded <0)
     {
       printf("I could not bind");
     }
     int proc=1;

     while(proc)
     {
       char  buf[200];
       char empty_buf[1];
       memset(buf,'\0',sizeof(buf));
       struct sockaddr from;
       int  out;
       memset(&from,0,sizeof(from));
       socklen_t fromlen=sizeof(from);
       out=recvfrom(socket_desc,buf,2940,MSG_WAITALL,&from,&fromlen);
       if(out < 0)
       {
         printf("Error in receiving data.");
       }

       else
       {
         if(containsWallet(buf,empty_buf))
         {
           printf("The ServerB received a request from the Main Server\n");
         char fileName[]="block2.txt";
         FILE *fp=fopen(fileName,"r");
         int ii=0;
         const unsigned MAX_LENGTH = 2560;
         char  buffer[MAX_LENGTH];
         char myString[MAX_LENGTH*MAX_LENGTH];
         int size_of_list=0;
         memset(&myString, 0,sizeof(myString) );
         while(fgets(buffer,MAX_LENGTH,fp))
         {
           strcat(myString,buffer);
           size_of_list=ii;
           ii++;
         }
         fclose(fp);
         char *msg;
         msg=myString; 
         int msg_len = strlen(msg);
         int udp_out=sendto(socket_M_desc,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr_M,sizeof(server_addr_M));
         if(udp_out<0)
         {
           printf("ERROR: UDP message to server B  was not sent out. \n");
         }
         printf("The ServerB finished sending the response to the Main Server.\n");
         }
         else if (containsTXLIST(buf))
         { //performtxList
           printf("The ServerB finished sending the response to the Main Server.\n");
           char fileName[]="block2.txt";
         FILE *fp=fopen(fileName,"r");
         int ii=0;
         const unsigned MAX_LENGTH = 2560;
         char  buffer[MAX_LENGTH];
         char myString[MAX_LENGTH*MAX_LENGTH];
         int size_of_list=0;
         memset(&myString, 0,sizeof(myString) );
         while(fgets(buffer,MAX_LENGTH,fp))
         {
           //myString[ii] = malloc(strlen(buffer)+1);
           //strncpy(myString[ii],buffer,strlen(buffer));
           strcat(myString,buffer);
           size_of_list=ii;
           ii++;
         }
         fclose(fp);
         char *msg;
         msg=myString; 
         int msg_len = strlen(msg);
         int udp_out=sendto(socket_M_desc,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr_M,sizeof(server_addr_M));
         if(udp_out<0)
         {
           printf("ERROR: UDP message to server B  was not sent out. \n");
         }
         printf("The ServerB finished sending the response to the Main Server.\n");
         }
        else
        {
          //performtransaction
         printf("The ServerB received a request from the Main Server.\n"); 
          char fileName[]="block2.txt";
          FILE *fp=fopen(fileName,"a");
          int ii=0;
          const unsigned MAX_LENGTH =256;
          char buffer[MAX_LENGTH];
          memset(buffer,'\0',sizeof(buffer));
          strncpy(buffer,buf,strlen(buf));
          int pass=fprintf(fp,"\n%s\n",buffer); fclose(fp);
          char msg[1];
          memset(msg,'\0',sizeof(msg)); 
          if(pass<0){
            strncpy(msg,"0",1);
          }else
          {
            strncpy(msg,"1",1); 
          }
          int msg_len = sizeof(msg);
         int udp_out=sendto(socket_M_desc,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr_M,sizeof(server_addr_M));
         if(udp_out<0)
         {
           printf("ERROR: UDP message to server A  was not sent out. \n");
         }
         printf("The ServerB finished sending the response to the Main Server.\n");
        }
       }
     }
       
     
     return 0;
}
