
#include "serverM.h"
#include "socketHelper.h"

//defining port number
//int containsWallet(char * msg, char* *username);
int construct_udp_recv_from(char* portNumber);
int send_udp_msg(char * message, int socket_desc);

int randNum(){
  int randomnumber;
  srand(time(0));
  randomnumber = rand() % 3;
  return randomnumber+1;

}

void getUsersName(char *msg_from_client,char *userName1,char *userName2,char *txAmt){
  char * ptr;
  char msg_cpy[strlen(msg_from_client)+1];
  memset(msg_cpy,'\0',sizeof(msg_cpy)); 
  char *end_token;
  int pos=1;
  strncpy(msg_cpy,msg_from_client,strlen(msg_from_client));
  //strncat(msg_cpy," ",1);
  ptr=strtok_r(msg_cpy," ",&end_token);
  while(ptr !=NULL){
    if(pos ==1){//assign userName1
      strncpy(userName1,ptr,strlen(ptr)); 
    }
    else if(pos == 2){
      strncpy(userName2,ptr,strlen(ptr));
    }
    else if(pos ==3){
      strncpy(txAmt,ptr,strlen(ptr));
    }
    ptr=strtok_r(NULL," ",&end_token);
    pos++; 
  }
  ptr=NULL;
  free(ptr);
  
}
int containsUser(char *msg,char * userName,int printOut){
//  const char *ptr= msg;
  if(strstr(msg,userName) != NULL){
    //TODO EXACT MATCH use strcmp
    return 1; 
  }else{
    if(printOut){
      //char msg[500];
      //memset(msg,'\0',sizeof(msg));
      //strcat(msg,"Unable to proceed with the transaction as ");
      //strcat(msg,userName);
      //strcat(msg," is not part of the network.\n");
      //send(printOut,msg,strlen(msg),0);
    }
    //username not found
    return 0; 
  }

}

void getLogic(char * str, int *value, int* logic, char*userName, int* cnt, int *id){
  char str_cpy[strlen(str)+1];
  memset(str_cpy,'\0',sizeof(str_cpy));
  char* ptr;
  int spot_counter=0;
  int found_name=0;
  char *end_token; 
  int ii=*cnt; 
  strncpy(str_cpy,str,strlen(str));
  ptr=strtok_r(str_cpy, " ",&end_token);
  while(ptr !=NULL){

    if(spot_counter ==0){
      if( *id <atoi(ptr)){
        *id=atoi(ptr); 
      }

    }
    if(strcmp(ptr,userName)==0){
      if( spot_counter ==1){
        logic[ii] =1; //which means subtract 
      }else{
        logic[ii] =2;//which means add
      }
      found_name=1;
      
    }
    if(spot_counter ==3 && found_name){
      value[ii]=atoi(ptr);
      ii++;
    }
    
    spot_counter++; 
    ptr=strtok_r(NULL," ",&end_token); 
  }
  *cnt=ii;
  ptr=NULL;
  end_token=NULL; 
  free(ptr);
  free(end_token);
}
int getWalletInfo(int socketDescriptor,int udp_recv,struct sockaddr_in  server_addr,char* msg,int *total_value,char *user_name, int * serial_id,int socket){
  int msg_len = strlen(msg);
  int out =-1;
  char  buf[2950];
  int id = *serial_id;
  int contains =0;
  memset(buf,'\0',sizeof(buf)); 
  struct sockaddr from;
  memset(&from,0,sizeof(from));
  int fromlen=sizeof(from);
   int udp_out=sendto(socketDescriptor,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr,sizeof(server_addr));
            if(udp_out<0){
              printf("ERROR: UDP message to server A  was not sent out. \n"); 
            }
            out=recvfrom(udp_recv,buf,2940,MSG_WAITALL,&from,&fromlen);
            if(out < 0){
              printf("Error in recieving udp info \n");
            }else{
              if(containsUser(buf,user_name,socket)){//peform parsing for checking check wallet from server A info
                contains=1;
                char *str;
                int array_size=100;
                int logic[array_size];
                int current_value[array_size];
                memset(logic,0,sizeof(logic));
                memset(current_value,0,sizeof(current_value));
                char buf_cpy[out+1];
                char *end_str;
                strncpy(buf_cpy,buf,strlen(buf));
                str=strtok_r(buf_cpy,"\n",&end_str);
                int cnt=0;
                while(str !=NULL){
                  getLogic(str,&current_value,&logic,user_name,&cnt,&id);
                  str=strtok_r(NULL,"\n",&end_str);
                }
                //buf_cpy=NULL;
//free(buf_ptr);
                //free(buf_cpy);
                for(int jj=0; jj<array_size; jj++)
                {
                  if(logic[jj] ==2){
//perform addition
                    *total_value=*total_value + current_value[jj];
                  }
                  else if(logic[jj] ==1){
//peform subtraction
                    *total_value=*total_value - current_value[jj];
                    if(*total_value <0){
                      *total_value =0;
                    }
                  }
                }
              }
              else{contains =0;}
            }
            *serial_id=id;
            return contains;
}

struct socketInfo{
  int socket_A_desc;
  int socket_B_desc;
  int socket_C_desc;
  struct sockaddr_in server_addr_A;
  struct sockaddr_in server_addr_B;
  struct sockaddr_in server_addr_C;
  
}socketInfo;


void performWalletTask(struct socketInfo socketInfo,int udp_recv,int * init_value,char *user_name, int *serial_Id,int socket,int pFlag){
  int value=*init_value;
  int id=*serial_Id;
  char *msg;
  msg = " WALLET";
  if(pFlag==1|| pFlag ==2){printf("The main server sent a request to server A.\n");}
  int containerA=getWalletInfo(socketInfo.socket_A_desc,udp_recv,socketInfo.server_addr_A,msg,&value,user_name,&id,socket);
  if(pFlag==1){printf("The main server recived transactions from Server A using UDP over port 21623\n");}
  if(pFlag==2){printf("The main server recived the feedback from server A using UDP over port 21623\n");}
  int total_value_from_B=value;
  if(pFlag){printf("The main server sent a request to server B.\n");}
  int containerB=getWalletInfo(socketInfo.socket_B_desc,udp_recv,socketInfo.server_addr_B,msg,&total_value_from_B,user_name,&id,socket);
  if(pFlag==1){printf("The main server recived transactions from Server B using UDP over port 22623\n");}
  if(pFlag==2){printf("The main server recived the feedback from server B using UDP over port 22623\n");}
  int total_value_from_C=total_value_from_B;
  if(pFlag){printf("The main server sent a request to server C.\n");}
  int containerC=getWalletInfo(socketInfo.socket_C_desc,udp_recv,socketInfo.server_addr_C,msg,&total_value_from_C,user_name,&id,socket);
  if(pFlag==1){printf("The main server recived transactions from Server C using UDP over port 23623\n");}
  if(pFlag==2){printf("The main server recived the feedback from server C using UDP over port 23623\n");}
  int final_total_value=total_value_from_C;
  *init_value=final_total_value;//update new init value
  *serial_Id=id;

  if(containerA ==1 || containerB ==1 || containerC ==1){//at least one server has the name
    //wrap with a pflag for bother users not in list
    if(pFlag ==1){
      printf("The main server sent the current balance to client X\n");
      char Num[10];
      memset(Num,'\0',sizeof(Num));
      sprintf(Num, "%d", final_total_value);
      char msg[500];
      memset(msg,'\0',sizeof(msg));
      strcat(msg,"The current balance of ");
      strcat(msg,user_name);
      strcat(msg," is ");
      strcat(msg,Num);
      strcat(msg," alicoins.\n");
      send(socket,msg,strlen(msg),0);
    }
    
    
  }else{//if text does not exisit
       char msg[500];
       memset(msg,'\0',sizeof(msg));
       strcat(msg,"Unable to proceed with the transaction as ");
       strcat(msg,user_name);
       strcat(msg," is not part of the network.\n");
       send(socket,msg,strlen(msg),0);
    
    //printf("The total value from all servers for %s is %i\n",user_name,final_total_value); //send to clients
  }
  //return 1 if name not found
}


void performTXTask(char *msg_from_client,struct socketInfo socketInfo,int udp_recv, int socket){
  char userName1[100];
  memset(userName1,'\0',sizeof(userName1));
  char userName2[100];
  memset(userName2,'\0',sizeof(userName2));
  char txAmt[100];
  memset(txAmt,'\0',sizeof(txAmt));
  
  getUsersName(msg_from_client,&userName1,&userName2,&txAmt);
  printf("The main server received from %s to transfer %s coins to %s using TCP over port 25623.\n",userName1,txAmt,userName2);//fix port number
  int userName1Value=1000;
  int temp=0;
  performWalletTask(socketInfo,udp_recv,&userName1Value,userName1,&temp,socket,0);
  int userName2Value=1000;
  int serial_Id=-1;
  performWalletTask(socketInfo,udp_recv,&userName2Value,userName2,&serial_Id,socket,2);
  
  if(userName1Value -atoi(txAmt) <0){
      char tx_er_msg[900];
      memset(tx_er_msg,'\0',sizeof(tx_er_msg));
      strcat(tx_er_msg,userName1);
      strcat(tx_er_msg," was unable to transfer ");
      strcat(tx_er_msg,txAmt);
      strcat(tx_er_msg," alicoins to ");
      strcat(tx_er_msg,userName2);
      strcat(tx_er_msg,".\n");
      strcat(tx_er_msg,"The current balance of ");
      strcat(tx_er_msg,userName1);
      strcat(tx_er_msg," is ");
      int curr_amt=userName1Value;
      char curr_amt_char[10];
      memset(curr_amt_char,'\0',sizeof(curr_amt_char));
      sprintf(curr_amt_char, "%d", curr_amt);
      strcat(tx_er_msg,curr_amt_char);
      strcat(tx_er_msg," alicoins.\n");
      send(socket,tx_er_msg,strlen(tx_er_msg),0);
    return;
  }
  //send transaction info to random server
   char serialNum[10];
   memset(serialNum,'\0',sizeof(serialNum));
  serial_Id=serial_Id+1;
  sprintf(serialNum, "%d", serial_Id);
  int udp_out=-1;
  int out =-1;
  char msg[200];
  memset(msg,'\0',sizeof(msg));
  strncat(msg,serialNum,strlen(serialNum));
  strncat(msg," ",1);
  strncat(msg,userName1,strlen(userName1));
  strncat(msg," ",1);
  strncat(msg,userName2,strlen(userName2));
  strncat(msg," ",1);
  strncat(msg,txAmt,strlen(txAmt));
  int msg_len= strlen(serialNum)+ 1 + strlen(userName1)+ 1 +strlen(userName2)+ 1+ strlen(txAmt);
  msg[msg_len+1]='\0';

  int socketDescriptor=-1;
  struct sockaddr_in server_addr;
  struct sockaddr from;
  memset(&from,0,sizeof(from));
  int fromlen=sizeof(from);
  char buf[200];
  memset(buf,'\0',sizeof(buf));

  
  switch(randNum()){
  case 1:
//init for  server A
    socketDescriptor=socketInfo.socket_A_desc;
    server_addr=socketInfo.server_addr_A;
    break;
  case 2:
//init for  server B
    socketDescriptor=socketInfo.socket_B_desc;
    server_addr=socketInfo.server_addr_B;
    break;
  case 3:
//init for  server C
    socketDescriptor=socketInfo.socket_C_desc;
    server_addr=socketInfo.server_addr_C;
    break;
  default:
    break;
  }
  //send transaction
  udp_out=sendto(socketDescriptor,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr,sizeof(server_addr));
  if(udp_out<0){
    printf("ERROR: UDP message to server A  was not sent out. \n");
  }
  out=recvfrom(udp_recv,buf,2940,MSG_WAITALL,&from,&fromlen);
  if(out < 0){
    printf("Error in recieving udp info \n");
  }
  int pass =atoi(buf); 
  if(pass ==1){
      char msg_to_send[900];
      memset(msg_to_send,'\0',sizeof(msg_to_send));
      strcat(msg_to_send,userName1);
      strcat(msg_to_send," successfully transferred ");
      strcat(msg_to_send,txAmt);
      strcat(msg_to_send," alicoins to ");
      strcat(msg_to_send,userName2);
      strcat(msg_to_send,".\n");
      strcat(msg_to_send,"The current balance of ");
      strcat(msg_to_send,userName1);
      strcat(msg_to_send," is ");
      int curr_amt=userName1Value-atoi(txAmt);
      char curr_amt_char[10];
      memset(curr_amt_char,'\0',sizeof(curr_amt_char));
      sprintf(curr_amt_char, "%d", curr_amt);
      strcat(msg_to_send,curr_amt_char);
      strcat(msg_to_send," alicoins\n");
      send(socket,msg_to_send,strlen(msg_to_send),0);
  }else{
    printf("fail to write to file \n");  //send info to client 
  }
  printf("The main server sent the result of the transaction to client X\n");
  //printf("User1 is : %s\n",userName1);
  //printf("User2 is : %s\n",userName2);    
}

void getAllInfo(int socketDescriptor,int udp_recv,struct sockaddr_in  server_addr,char* msg, char id[10000][1],char return_msg[10000][100], int *index){
  int msg_len = strlen(msg);
  int out =-1;
  char  buf[2950];
  int curr_index=*index; 
  memset(buf,'\0',sizeof(buf));
  struct sockaddr from;
  memset(&from,0,sizeof(from));
  int fromlen=sizeof(from);
   int udp_out=sendto(socketDescriptor,msg,msg_len,MSG_CONFIRM,(const struct sockaddr *) &server_addr,sizeof(server_addr));
            if(udp_out<0){
              printf("ERROR: UDP message to server A  was not sent out. \n"); 
            }
            out=recvfrom(udp_recv,buf,2940,MSG_WAITALL,&from,&fromlen);
            if(out < 0){
              printf("Error in recieving udp info \n");
            }else{//message from server recvied correctly
              char buf_cpy[out+1];
              char * str;
              char *end_str;
              int curr_value=*index;
              memset(buf_cpy,'\0',sizeof(buf_cpy));
              strncpy(buf_cpy,buf,strlen(buf));
              buf_cpy[strlen(buf_cpy)+1]='\0';
              //strcat(buf_cpy,'\n'); 
              str=strtok_r(buf_cpy,"\n",&end_str);
              while(str !=NULL){
                char *end_str_id;
                char *str_id;
                char str_cpy[strlen(str)];
                memset(str_cpy,'\0',sizeof(str_cpy));
                strncpy(str_cpy,str,strlen(str));
                str_id=strtok_r(str_cpy," ",&end_str_id);
                if(str_id !=NULL){
                  strncpy(return_msg[atoi(str_id)-1],str,strlen(str));//perhaps problem later
                  strncpy(id[curr_index],str_id,1);
                  
                 
                }
                str=strtok_r(NULL,"\n",&end_str);
                 curr_index++;
              }
            }
            *index=curr_index;
}

void performTXTaskList(char *msg_from_client,struct socketInfo socketInfo,int udp_recv, int socket){

  //get all info from all servers
  char *msg;
  msg = "TXLIST";
  char return_msg[10000][100];
  memset(return_msg,'\0',sizeof(return_msg));
  
  char id[10000][1];
  memset(id,'\0',sizeof(id));
  int index=0; 
  getAllInfo(socketInfo.socket_A_desc,udp_recv,socketInfo.server_addr_A,msg,&id, &return_msg, &index);
  getAllInfo(socketInfo.socket_B_desc,udp_recv,socketInfo.server_addr_B,msg,&id, &return_msg, &index);
  getAllInfo(socketInfo.socket_C_desc,udp_recv,socketInfo.server_addr_C,msg,&id, &return_msg, &index);

  //intf("grabbing all data for servers call in txtas list \n");

  char fileName[]="alicoin.txt";
  FILE *fp=fopen(fileName,"w");
  for(int ii =0; ii<index; ii++){
    if(return_msg[ii] != NULL){
      fprintf(fp,"%s\n",return_msg[ii]);
      // printf("%s\n",return_msg[ii]);
    }
  }
  fclose(fp);
  // free(return_msg);
  // free(index);
  //free(id);

  
  

}



int main(){
   const char TCP_PORT_A[] ="25623";
   const char TCP_PORT_B[] ="26623";
   const char UDP_PORT[]   ="24623";//receiving port
   const int INIT_ALI_COIN=1000;
   //const int UDP_PORT_A=21623;
   //const int UDP_PORT_B=22623;
   //const int UDP_PORT_B=22623;
   
  printf("The main server is up and running. \n");
  //printf("port Num is 25623\n");
  
  //create TCP client A sockets
  int clientA_socket_descr=parent_socket_des(TCP_PORT_A);
  int clientB_socket_descr=parent_socket_des(TCP_PORT_B);

  //construct listening udp port
  int udp_recv=construct_udp_recv_from(UDP_PORT);
  
  //construct a sending UDP sockets
  struct sockaddr_in server_addr_A;
  struct sockaddr_in server_addr_B;
  struct sockaddr_in server_addr_C;
  memset(&server_addr_A,0,sizeof(server_addr_A));
  memset(&server_addr_B,0,sizeof(server_addr_B));
  memset(&server_addr_C,0,sizeof(server_addr_C));
  int socket_A_desc = construct_udp_to_send(21623, &server_addr_A);
  int socket_B_desc = construct_udp_to_send(22623, &server_addr_B);
  int socket_C_desc = construct_udp_to_send(23623, &server_addr_C);
  //init struct for readability
  socketInfo.socket_A_desc=socket_A_desc;
  socketInfo.socket_B_desc=socket_B_desc;
  socketInfo.socket_C_desc=socket_C_desc;
  socketInfo.server_addr_A=server_addr_A; 
  socketInfo.server_addr_B=server_addr_B;
  socketInfo.server_addr_C=server_addr_C;   
  
  
  struct pollfd myPoll[1024];// = malloc(sizeof *myPoll * fd_size);
  int num = sizeof(myPoll)/sizeof(myPoll[0]);
  //define the pollfd for each socket
  //init myPoll

  for(int ii=0; ii<num; ii++){
    myPoll[ii].fd = -1;// File descriptor
    myPoll[ii].events = 0;// Set of events to monitor
    myPoll[ii].revents = 0;// Ready Event Set of Concerned Descriptors
  }
  
  
  myPoll[0].fd = clientA_socket_descr;//clientA_socket_descr;//myListener;
  myPoll[0].events =POLLIN;

  myPoll[1].fd = clientB_socket_descr; 
  myPoll[1].events =POLLIN; 

  
  char msg_from_client[200];
  memset(msg_from_client,'\0',sizeof(msg_from_client));
  int time_out=500;
  int myAccept; 
  while(1){
    int num_outs = poll(myPoll, num, time_out);
    if (num_outs == -1) {
      perror("poll");
      exit(1);
    }
    else if(num_outs ==0){
      //printf("poll timed out \n");
    }
    else{
      for(int ii =0; ii<num; ii++){
        if( myPoll[ii].fd == -1 ){
        continue;
        }
        if (myPoll[ii].fd == clientA_socket_descr && (myPoll[ii].revents && POLLIN)){
            struct sockaddr_in their_addr;
            socklen_t addr_size = sizeof(their_addr);
            myAccept = accept(clientA_socket_descr,(struct sockaddr *)&their_addr,&addr_size);
            memset(msg_from_client,'\0',sizeof(msg_from_client));
            int myRecv = recv(myAccept, msg_from_client,sizeof(msg_from_client),0);//recv from clinetA
            char * user_name[100]; 
            if(myAccept<0){
              printf("Error in accept...\n");
            }
            //************************************
            else if(containsWallet(msg_from_client,user_name)){            //perform check wallet task
            printf("The main server received input=\"%s\" from the client using TCP over port 25623.\n",*user_name);
            int init_value=INIT_ALI_COIN;
             int serial_Id[5000];
             memset(serial_Id,0,sizeof(serial_Id));
             performWalletTask(socketInfo,udp_recv,&init_value,*user_name,&serial_Id,myAccept,1);
             //send to client
             char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0); 
            }
            //************************************
            else if(containsTXLIST(msg_from_client)){ //peform txList task
              performTXTaskList(msg_from_client,socketInfo,udp_recv,myAccept);
              char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0);  
            }
            //************************************
            else{//perform transaction
              performTXTask(msg_from_client,socketInfo,udp_recv,myAccept);
              char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0); 
              }
            

        }
        else if (myPoll[ii].fd == clientB_socket_descr && (myPoll[ii].revents && POLLIN)){
          //---------------------------------------------
          struct sockaddr_in their_addr;
            socklen_t addr_size = sizeof(their_addr);
            myAccept = accept(clientB_socket_descr,(struct sockaddr *)&their_addr,&addr_size);
            memset(msg_from_client,'\0',sizeof(msg_from_client));
            int myRecv = recv(myAccept, msg_from_client,sizeof(msg_from_client),0);//recv from clinetA
            char * user_name[100]; 
            if(myAccept<0){
              printf("Error in accept...\n");
            }
            //************************************
            else if(containsWallet(msg_from_client,user_name)){            //perform check wallet task
            printf("The main server received input=\"%s\" from the client using TCP over port 26623.\n",*user_name);
            int init_value=INIT_ALI_COIN;
             int serial_Id[5000];
             memset(serial_Id,0,sizeof(serial_Id));
             performWalletTask(socketInfo,udp_recv,&init_value,*user_name,&serial_Id,myAccept,1);
             //send to client
             char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0); 
            }
            //************************************
            else if(containsTXLIST(msg_from_client)){ //peform txList task
              performTXTaskList(msg_from_client,socketInfo,udp_recv,myAccept);
              char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0);  
            }
            //************************************
            else{//perform transaction
              performTXTask(msg_from_client,socketInfo,udp_recv,myAccept);
              char msg[]="KILL";
             int msg_size=strlen(msg);
             sleep(1);
             send(myAccept,msg,msg_size,0); 
              }
            
            //-------------------------------------
        }

      }
      close(myAccept);
      }
    }



  return 0;
}


int parent_socket_des(const char * port_number){

 //setting up TCP port
  struct addrinfo tcp_hints, *tcp_res;
  int tcp_addr=-1;
  memset(&tcp_hints,0,sizeof(tcp_hints));
  tcp_hints.ai_family=AF_INET;
  tcp_hints.ai_socktype=SOCK_STREAM;
  tcp_hints.ai_flags = AI_PASSIVE;

  //setting up port and address
  //char TCP_PORT_NUM = port_number;
  // getaddrinfo("localhost",port_number,&tcp_hints,&tcp_res);
  getaddrinfo("0.0.0.0",port_number,&tcp_hints,&tcp_res);
  tcp_addr = socket(tcp_res->ai_family,tcp_res->ai_socktype,tcp_res->ai_protocol);
  printf("my socket is (fd) %i\n",tcp_addr);//for debuging 
  struct sockaddr_in *addr;
  addr = (struct sockaddr_in *)tcp_res->ai_addr;
  //intf("inet_ntoa(in_addr)sin = %s\n",inet_ntoa((struct in_addr)addr->sin_addr));//for debugging

  if (tcp_addr <=0){
    printf("There was an error in creating a tcp socket \n");
    return -1;
  }
  //binding to address:port
  int myBind=bind(tcp_addr,tcp_res->ai_addr,tcp_res->ai_addrlen);
  if (myBind <0){
  printf("There was an error in binding to a tcp socket \n");
  return -1;
  }
  // creating a lister socket 
  if( listen(tcp_addr,25) < 0){
    printf("There was an error in creating a listing socket\n"); 
  }
  return tcp_addr; 
  
}

//construct udp that server will recive from 
int construct_udp_recv_from(char* portNumber){
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = PF_INET; // don't care IPv4 or IPv6
  hints.ai_socktype =SOCK_DGRAM; // UDP stream sockets (TCP SOCK_STREAM
  hints.ai_flags = AI_PASSIVE; // fill in my IP for me
  getaddrinfo(NULL,portNumber,&hints,&res);
  int socket_desc = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
  if( socket_desc <0){
    printf( "Could not create socket\n");
    return -1;
  }
  int isBinded;
  isBinded=bind(socket_desc,res->ai_addr,res->ai_addrlen);
  if(isBinded <0){
    printf("I could not bind");
    return -1;
  }
  return socket_desc;
}

