#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 1024

typedef struct threadHelper{
int sockID;
int userID;
int* confirmation;
}threadHelper;

void sendMessage(char* str,int socket,char* str_2);
void registerFunction(int userID,int socket);
void waitForFriends(int socket);


void * receiveMessages(void * threadHelp){
        char* data=(char*)calloc(1024,sizeof(char));
	threadHelper helper=*((threadHelper*)threadHelp);
	int clientSocket = helper.sockID;
	int read;
	int control = 1,control_2=1;
	        while(control_2){
	        read = recv(clientSocket,data,MAX,0);
        if(read!=0 ){
           control=0;}
        if(!control){
         data[read] = '\0';
           if(strcmp(data,"register")==0){
           //printf("\n\n%s",data);
           registerFunction(helper.userID,clientSocket);
           control_2=0;
           }else if(strcmp(data,"login")==0){
           control_2 = 0 ; 
           }
        }
        }
        
        *(helper.confirmation) = 1;
	while(1){
	        
            
		char* data_2=(char*)calloc(MAX,sizeof(char));
		int read;
		if((read = recv(clientSocket,data_2,MAX,0)) && data_2[0]!='-'&& data_2[0] != '|' && data_2[0] != '$'){
		*(helper.confirmation)=0;
		data_2[read] = '\0';
		printf("\nYou have a message from %s\n",data_2);
		read = recv(clientSocket,data,MAX,0);
		//data[read] = '\0';
		}else if(data_2[0]=='-'){
		*(helper.confirmation)=0;
		printf("id\tname\tsurname\tphone\n");
		printf("%s\n",data_2 + 1);
		}else if(data_2[0]=='|'){
		*(helper.confirmation)=0;
		printf("\n%s\n",data_2 + 1);
		}else if(data_2[0]=='$'){
		*(helper.confirmation)=0;
		printf("\n%s\n",data_2 + 1);
		}
		*(helper.confirmation) = 1;

free(data_2);
	}
	free(data);
		

}

int main(int argc, char *argv[]){

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
        int userId = atoi(argv[1]);
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 8080;
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) return 1;

	printf("\nConnected to server.\n");
        send(clientSocket, argv[1], strlen(argv[1]), 0);
        threadHelper threadHelp;
        threadHelp.sockID=clientSocket;
        threadHelp.confirmation=(int*)calloc(1,sizeof(int));
        //threadHelp.messageHistory = (char*)calloc(MAX,sizeof(char));
        threadHelp.userID = userId;
	pthread_t thread;
	pthread_create(&thread, NULL, receiveMessages, (void *) &threadHelp );
	while(!*(threadHelp.confirmation));
        printf("\nChoose operation:");
        printf("\nSend Message: 1");
        printf("\nCheck Message: 2");
        printf("\nAdd Friend: 3");
        printf("\nRemove Friend: 4");
        printf("\nCheck Friends List: 5\n");
	while(1){

		char* input=(char*)calloc(MAX,sizeof(char));
		int op=0;

		while(!*(threadHelp.confirmation));
		scanf("%d",&op);
        switch(op){
        	case(1): 
        	        //*(threadHelp.confirmation)=0;
        	        printf("\nPlease enter the message you want to send:\n");
        	        scanf("%s",input);
                        sendMessage(input,clientSocket,"SENDMESSAGE");
        		break;
        	case(2):
        	        printf("\nEnter ID of whose messages you want to see:");
        	        scanf("%s",input);
        	        sendMessage(input,clientSocket,"CHECKINITIALIZE");
        		break;
        	case(3):
        	        printf("\nEnter ID of new friend:");
        	        scanf("%s",input);
        	        sendMessage(input,clientSocket,"ADD");
        	        printf("\nUser added to your friends list.\n");
        		break;
        	case(4):
        	        printf("\nEnter ID of delete target:");
        	        scanf("%s",input);
        	        sendMessage(input,clientSocket,"REMOVE");
        	        printf("\nUser removed from your friends.\n");
        		break;
        	case(5):
        	        sendMessage("",clientSocket,"FRIENDSLIST");
        	        
        	        break;
        	default:
        	        printf("\nInput with wrong format\n");
        	        exit(1);
        		break;
		
		}

	}


}
void sendMessage(char* str,int socket,char* str_2){
		        if(strcmp(str_2,"SENDMESSAGE") == 0){
			send(socket,str_2,1024,0);	
			send(socket,str,1024,0);	
			scanf("%[^\n]s",str);
			send(socket,str,1024,0);}else if(strcmp(str_2,"ADD") == 0 || strcmp(str_2,"REMOVE") == 0 || strcmp(str_2,"CHECKINITIALIZE") == 0 ){
			send(socket,str_2,1024,0);	
			scanf("%[^\n]s",str);
			send(socket,str,1024,0);
			}else if(strcmp(str_2,"FRIENDSLIST") == 0){
			send(socket,str_2,1024,0);	
			}
}
void registerFunction(int userID,int socket){
     char* allinfo=(char*)calloc(1024,sizeof(char));
     char* read=(char*)calloc(120,sizeof(char));
     printf("Enter your name surname phone:");
     scanf(" %s",read);

     
     send(socket,read,1024,0);	
     scanf("%s",read);
     send(socket,read,1024,0);	
     scanf("%[^\n]s",read);
     send(socket,read,1024,0);
     free(read);
     free(allinfo);
}

