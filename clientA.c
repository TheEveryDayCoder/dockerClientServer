int init_socket(char * port_number); 
int send_msg(int my_socket_disc,char * msg, int message_size); 
int transaction(int my_socket_disc,char * msg);
void recv_msg_clients(int my_socket_disc);
int containsKill(char *msg);
int msgIsEmpty(char * msg);

#include "clientA.h"
#include "socketHelper.h"

int main(int argc, char * argv[]){
//USCID:9281603623
//udp port: 21623
//TCP client A 25623
//TCP client B 26623
int input_argument=0;
char port_number[]= "25623";
printf("The client A us up and running.\n");
int my_socket_disc=init_socket(port_number);
char msg[200];
//printig public message

input_argument = argc-1;
switch ( input_argument ){
  case 1: ;
    if(containsTXLIST(argv[1])){
      //perform txList task
      printf("Sent a sorted list request to the main server.\n");
      memset(msg,'\0',sizeof(msg));
      strncat(msg,argv[1],strlen(argv[1]));
      send_msg(my_socket_disc,msg,strlen(msg));
    }
    else{
      //checkwallet
      printf("%s sent a blance enquiry request to the main server.\n",argv[1]);
      memset(msg,'\0',sizeof(msg));
      strncat(msg,argv[1],strlen(argv[1]));
      char check_wallet_str[]=" WALLET";//space needed for dlimeter parsing
      strncat(msg,check_wallet_str,strlen(check_wallet_str)+1); 
      send_msg(my_socket_disc,msg,strlen(msg));
    }
      break;
  case 3: //transaction
    printf("%s has requested to transfer %s coins to %s.\n",argv[1],argv[2],argv[3]);
    memset(msg,'\0',sizeof(msg));
    for(int ii=1;ii<=input_argument; ii++){
      if(ii !=1){
        strncat(msg," ",1); //create spaces for each except for the first time 
      }
      strncat(msg,argv[ii],strlen(argv[ii])); 
    }
    send_msg(my_socket_disc,msg,strlen(msg));
  break;
  default:
  {
    printf("There was either too many or too little input arguemnts\n"); 
    break;
  }
}
   return 0;
 }


int init_socket(char *port_num){
struct addrinfo hints, *res;                                                                                
   memset(&hints,0,sizeof(hints));                                                    
   hints.ai_family = PF_INET;                                                   
   hints.ai_socktype = SOCK_STREAM;                                                                                                                      
   getaddrinfo("localhost",port_num,&hints,&res);                                        
                                                                                
   int mySockDesc = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
   if(mySockDesc <0){
     printf("could not create socket\n");
     return -1; 
   }
   //int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);                                                                            
   int myConnection = connect(mySockDesc,res->ai_addr,res->ai_addrlen);
   if (myConnection <0){
     printf("could not establish a connection\n"); 
     return -1; 
   }

return mySockDesc; 
}



int send_msg(int my_socket_disc,char * msg,int message_size){
int mySend= send(my_socket_disc, msg,message_size,0); 
if( mySend <0){
     printf("could not send data.\n");
}else{
  recv_msg_clients(my_socket_disc); 
    }
   close(my_socket_disc);
   return 0; 
}

void recv_msg_clients(int my_socket_disc){
  int kill_command=0;
  char msg_from_server[500];
  while(!kill_command){
    memset(msg_from_server,'\0',sizeof(msg_from_server));
    recv(my_socket_disc, msg_from_server,sizeof(msg_from_server),0);

    if(!containsKill(msg_from_server) || msgIsEmpty(msg_from_server)){
      if(!msgIsEmpty(msg_from_server)){
        printf("%s\n",msg_from_server);
      }
    }else{
      kill_command=1;
    }
  }
}

int msgIsEmpty(char * msg){
  if(strlen(msg) ==0){
      return 1;
    }
    return 0; 
}

int containsKill(char *msg){
  char killMsg[]="KILL";
  if (strstr(killMsg,msg)){
      return 1;
    }
    return 0; 
}
