#include "socketHelper.h"



 //////////////////////////////////////////////////////////////////////////////
 // This code block construct_udp_to_send is a helper function used to clean //
 // up some code. It uses a port number send empty udp port init. it returns //
 // the socket desc. and the a filled in server_addr_in type                 //
 //////////////////////////////////////////////////////////////////////////////
int construct_udp_to_send(int portNumber, struct sockaddr_in * server_addr){
  (*server_addr).sin_family = AF_INET;
  (*server_addr).sin_port = htons(portNumber);
//  (*server_addr).sin_addr.s_addr = INADDR_ANY;
  (*server_addr).sin_addr.s_addr = inet_addr("0.0.0.0");
  int socket_desc = socket(AF_INET,SOCK_DGRAM,0);  
  return socket_desc;
}


int containsWallet(char * msg, char **username){
  char checking_wallet_text[] =" WALLET";
char*loc; 
loc=strstr(msg,checking_wallet_text); 
if(loc ==NULL){
  return 0; 
}
else{
  *username= strtok(msg, " ");
  //printf("username :%s\n",*username);
    return 1; 
  }
}

int containsTXLIST(char * input){
  char txList[]= "TXLIST" ;
  if(  strstr(txList, input)){
    return 1;
    
  }else{
    return 0;
  } 
}

//////////////////////////////////////////////////////////////////////////////
// This code block defines and a map to be used and init with 1000 alicoins //
//////////////////////////////////////////////////////////////////////////////
//typedef struct Key{
//  char name[100]; 
//}Key;
//
//typedef struct Value{
//  int aliCoins; 
//}Value; 
//
//typedef struct Map{
//   Key key;
//   Value value; 
//}Map;

