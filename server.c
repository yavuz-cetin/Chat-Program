#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define MAX 1024




struct client{ //holds user socket info so it can be used in communication
	int sockID;
	struct sockaddr_in clientAddr;
	int length;
	int userId;
};

int clients = 0;
struct client ClientInfo[MAX]; //Global because when sending message server needs targets socket id.

int readUserIds(int** userIds);
void addUserIdToFile(int newUserId) ;
int isUserIdUnique(int* userIds, int numUserIds, int newUserId);
void registerUser(int userID,int clientSocket,char* strOfID);

void removeFirstCharacter(char *str);

int readFriendIds(int* friendIds, char* userID);
void addIdToFriendList(int** friends,int newFriend,char* userID);
void removeFriend(int removeID,int** friendsList,char* userID,int numF);
void sendFriendInfo(int numOfFriends,int* friendsList,int socket);
void readUserinfoFile(char *filePath, char *result) ;
void createMessagesFile(int id,char* userID);
void saveMessagesToFile(char* targetID,char* userID,char* message);

void findMessageHistory(char* userID,char* targetID,int socket);



void * handleClients(void * ClientDetail){

	struct client* clientDetail = (struct client*) ClientDetail;
	int clientSocket = clientDetail -> sockID;
        char* charId=(char*)calloc(10,sizeof(char));
	char* data=(char*)calloc(MAX,sizeof(char));
	int numOfFriends=0;
	int control=1;
        while(control){
        if(recv(clientSocket,data,MAX,0)!=0){
           control=0;}
           if(!control){
           strcpy(charId,data);
        clientDetail -> userId = atoi(data);
        int* userIds=(int*)calloc(MAX,sizeof(int));
        int numUserIds = readUserIds(&userIds);
        if(isUserIdUnique(userIds, numUserIds, clientDetail -> userId)){
          send(clientSocket,"register",MAX,0);//if user needs to register send info to client so they can provide their credentials.
          registerUser(clientDetail -> userId,clientSocket,charId);
        }else{
          send(clientSocket,"login",MAX,0);
        }
        printf("Client %d connected to the server.\n",clientDetail -> userId );
        int* friendsList = (int*)calloc(MAX,sizeof(int));
        numOfFriends = readFriendIds(friendsList,charId);
        }
        
        }
	while(1){

                int i;
		int read = recv(clientSocket,data,MAX,0);//receive message from client
		data[read] = '\0';

		if(strcmp(data,"SENDMESSAGE") == 0){//if client wants to send message enter here
		        
		        

			read = recv(clientSocket,data,MAX,0);
			
			

			data[read] = '\0';
                        char* targetID = (char*)calloc(10,sizeof(char));
                        strcpy(targetID,data);//hold the target id in string

			int id = atoi(data);
			
			int* friendsList = (int*)calloc(MAX,sizeof(int));
			int numOfFriends = readFriendIds(friendsList,charId);
                        if(!isUserIdUnique(friendsList, numOfFriends, id)){//checks if clients are friends or not
         
			read = recv(clientSocket,data,MAX,0);

			data[read] = '\0';
                        int count = 0;
                       
                        for(i=0;i < clients;i++){//travels until it finds the client with the same id so it can get the socketid
                       
                  
                        if(ClientInfo[i].userId == id){
                   
                        send(ClientInfo[i].sockID,charId ,MAX,0);

			send(ClientInfo[i].sockID,data,MAX,0);

                        saveMessagesToFile(targetID,charId,data);
                        }
			}
}else{
    send(clientSocket,"|Arkadas degilsiniz.",MAX,0);//if clients are not friends
}
		}else if(strcmp(data,"FRIENDSLIST")==0){//if client wants to see their friends and all their info enter here.
		int* friendsList = (int*)calloc(MAX,sizeof(int));
		numOfFriends = readFriendIds(friendsList,charId);
		sendFriendInfo(numOfFriends,friendsList,clientSocket);
		}else if(strcmp(data,"ADD")==0){//if client wants to add a friend enter here.
			read = recv(clientSocket,data,MAX,0);
			data[read] = '\0';
			int id = atoi(data);
			int* friendsList = (int*)calloc(MAX,sizeof(int));
                        numOfFriends = readFriendIds(friendsList,charId);
			addIdToFriendList(&friendsList,id,charId);
			numOfFriends = readFriendIds(friendsList,charId);
			createMessagesFile(id,charId);

		}else if(strcmp(data,"REMOVE")==0){//if client wants to remove a friend enter here.
		read = recv(clientSocket,data,MAX,0);
		data[read] = '\0';
		int id = atoi(data);
		int* friendsList = (int*)calloc(MAX,sizeof(int));
		numOfFriends = readFriendIds(friendsList,charId);
		removeFriend(id,&friendsList,charId,numOfFriends);
		numOfFriends = readFriendIds(friendsList,charId);
		}else if(strcmp(data,"CHECKINITIALIZE")==0){//if user wants to see all messages from a certain user.
                	read = recv(clientSocket,data,MAX,0);
			data[read] = '\0';
                        char* targetID = (char*)calloc(10,sizeof(char));
                        strcpy(targetID,data);
			
			findMessageHistory(charId,targetID,clientSocket);
			
			free(targetID);
		}
	}
	return NULL;

}

int main(){
        pthread_t* thread=(pthread_t*)calloc(MAX,sizeof(pthread_t));
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(serverSocket ==0){
	return 1;}
        int* userIds=(int*)calloc(MAX,sizeof(int));
        int numOfUsers;
        int i;
        int opt=1;
            if(setsockopt(serverSocket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))||setsockopt(serverSocket,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt))){
      return 1;
    }
    printf("\nsetsockopt was successful\n");
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = 8080;
	serverAddr.sin_addr.s_addr = INADDR_ANY;


	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) <0) { 
	return 1;}

	if(listen(serverSocket,MAX) <0){
	 return 1;}

	printf("Server is Listening...\n");
	numOfUsers = readUserIds(&userIds);
	
	
	printf("\nCurrently registered users(At the server start time):\n");
	for(i=0;i<numOfUsers;i++){
	if(i!=numOfUsers){
	printf("%d\n",userIds[i]);}
	}
        
	while(1){

		ClientInfo[clients].sockID = accept(serverSocket, (struct sockaddr*) &ClientInfo[clients].clientAddr, &ClientInfo[clients].length);

		pthread_create(&thread[clients], NULL, handleClients, (void *) &ClientInfo[clients]); //create thread to receive requests from user.

		clients ++;

 
	}
	for(int i = 0 ; i < clients ; i ++){
		pthread_join(thread[i],NULL);
		}
  return 0;
}

int readUserIds(int** userIds) {
    FILE *file = fopen("rsc/users.txt", "r");
    int* ids = *userIds;
    if (file == NULL) {
        system("mkdir -p ./rsc");
        system("touch rsc/users.txt");
        //fprintf(stderr, "Error opening file: %s\n", "rsc/users.txt");
        //return 0;
        file = fopen("rsc/users.txt", "r");
        if(file==NULL){
        return 1;
        }
    }

    int userIdCount = 0;
    int userId;

    while (fscanf(file, "%d", &userId) == 1) {
        ids[userIdCount] = userId;
        userIdCount++;
    }


    fclose(file);

    return userIdCount;
}

int isUserIdUnique(int* userIds, int numUserIds, int newUserId) {//checks if int array contains the newUserId
int i;
    for ( i= 0; i < numUserIds; i++) {
        if (userIds[i] == newUserId) {
            return 0; // if not unique return 0
        }
    }
    return 1; //if unique return 1
}

void addUserIdToFile(int newUserId) {//If user is registering save his/her id to users.txt file
    int* userIds=(int*)calloc(MAX,sizeof(int));
    int numUserIds = readUserIds(&userIds);

    if (isUserIdUnique(userIds, numUserIds, newUserId)) {
        FILE *file = fopen("rsc/users.txt", "a");

        if (file != NULL) {
            fprintf(file, "%d\n", newUserId);
            fclose(file);
        } else {
            fprintf(stderr, "Error opening file for append: %s\n", "rsc/users.txt");
        }
    }
    free(userIds);
}
void registerUser(int userID,int clientSocket,char* strOfID){//wait for user contact info so server can register it.
        char* data=(char*)calloc(MAX,sizeof(char));
        char* name=(char*)calloc(30,sizeof(char));
        char* surname=(char*)calloc(30,sizeof(char));
        char* phone=(char*)calloc(30,sizeof(char));
        char* data_2=(char*)calloc(30,sizeof(char));
        int read;
        FILE *file;
	int control = 1,control_2=1;
	        while(control_2){
	        read = recv(clientSocket,data,MAX,0);
        if(read!=0){
           control=0;}
        if(!control){
		
		data[read] = '\0';
		strcpy(name,data);
		read = recv(clientSocket,data,MAX,0);
		data[read] = '\0';
		strcpy(surname,data);
		read = recv(clientSocket,data,MAX,0);
		data[read] = '\0';
		strcpy(phone,data);
		removeFirstCharacter(phone);
		
		addUserIdToFile(userID);
		
		strcat(data_2,"mkdir -p rsc/");
                strcat(data_2,strOfID);
                system(data_2);
                strcpy(data_2,"touch rsc/");
                strcat(data_2,strOfID);
                strcat(data_2,"/userinfo.txt");
                system(data_2);
                strcpy(data_2,"touch rsc/");
                strcat(data_2,strOfID);
                strcat(data_2,"/friends.txt");
                system(data_2);
                
                strcpy(data_2,"rsc/");
                strcat(data_2,strOfID);
                strcat(data_2,"/userinfo.txt");
                file = fopen(data_2,"w");
                    if (file != NULL) {
                   fprintf(file, "%s\n", strOfID);
        fprintf(file, "%s\n", name);
        fprintf(file, "%s\n", surname);
        fprintf(file, "%s", phone);
        
        fclose(file);
    } else {
        fprintf(stderr, "\nError opening file for writing: %s\n", data_2);
    }
    
                //printf("\n\n%s %s %s",name,surname,phone);
		control_2=0;
		
        }
        }
        free(data);
        free(name);
        free(surname);
        free(phone);
        free(data_2);
}
void removeFirstCharacter(char *str) {
    if (str == NULL || strlen(str) <= 1|| str[0]!=' ') {
        return;
    }
    for (int i = 0; i < strlen(str) - 1; i++) {
        str[i] = str[i + 1];
    }

    str[strlen(str) - 1] = '\0';
}
void addIdToFriendList(int** friends,int newFriend,char* userID){//if someone adds someone as friends friends.txt is updated here.
int* frn = *(friends);
FILE* file;
int i=0;
int control = 1;
while(i!=MAX && control && frn[i]!=0){
     if(frn[i]==newFriend){
     control = 0;
     }
     i++;
}
if(control){
                  frn[i] = newFriend;
                  char* data_2 = (char*)calloc(30,sizeof(char));
                  strcpy(data_2,"rsc/");
                  strcat(data_2,userID);
                  strcat(data_2,"/friends.txt");
                  file = fopen(data_2,"a");
                    if (file != NULL) {
                   fprintf(file, "%d\n", newFriend);
        free(data_2);
        fclose(file);
    } else {
        fprintf(stderr, "\nError opening file for writing: %s\n", data_2);
    }
}

}

int readFriendIds(int* friendIds, char* userID) {//reads someones friend ids into a given array.
    char* filePath = (char*)calloc(MAX,sizeof(char));
    strcpy(filePath,"rsc/");
    strcat(filePath,userID);
    strcat(filePath,"/friends.txt");
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return 0;
    }

    int friendIdCount = 0;
    char* line=(char*)calloc(MAX,sizeof(char));

    while (fgets(line, sizeof(line), file) != NULL) {
        int friendId;
        if (sscanf(line, "%d", &friendId) == 1) {
            friendIds[friendIdCount] = friendId;
            friendIdCount++;
        }
    }
    free(filePath);
    free(line);
    fclose(file);

    return friendIdCount;
}
void removeFriend(int removeID,int** friendsList,char* userID,int numF){//first removes friend id from array. then writes it into friends.txt from zero
int i,j;
int* array = *friendsList;
    char* filePath = (char*)calloc(MAX,sizeof(char));
    strcpy(filePath,"rsc/");
    strcat(filePath,userID);
    strcat(filePath,"/friends.txt");
    FILE *file = fopen(filePath, "w");
int size = numF;
    for (i = 0; i < size; i++) {
        if (array[i] == removeID) {
            for (j = i; j < size - 1; j++) {
                array[j] = array[j + 1];
            }

            (size)--;
            i = size;
        }
    }
        for (i = 0; i < size; i++) {
        fprintf(file, "%d\n", array[i]);
    }
    free(filePath);
    fclose(file);
    
    
}
void sendFriendInfo(int numOfFriends,int* friendsList,int socket){//Send all friend contact information to client.
     char* info = (char*)calloc(MAX,sizeof(char));
      char* tmpInfo = (char*)calloc(MAX,sizeof(char));
      char* myString = (char*)calloc(5,sizeof(char));
     int i;
     FILE* file;
     char* filePath = (char*)calloc(30,sizeof(char));

     strcpy(info,"-");
     for(i=0;i<numOfFriends;i++){
     strcpy(filePath,"");
     strcat(filePath,"rsc/");
     sprintf(myString, "%d", friendsList[i]);
     strcat(filePath,myString);
     strcat(filePath,"/userinfo.txt");
     readUserinfoFile(filePath,tmpInfo);
     strcat(info,tmpInfo);
     strcat(info,"\n");
     }
     send(socket,info,MAX,0);
     free(info);
     free(tmpInfo);
     free(myString);
     free(filePath);
}

void readUserinfoFile(char *filePath, char *result) {//reads userinfo files and gets single users contact info.
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filePath);
        return;
    }

    result[0] = '\0';

    char* line= (char*)calloc(MAX,sizeof(char));

    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        strcat(result, line);
        strcat(result, "\t");
    }
    free(line);
    fclose(file);
}
void createMessagesFile(int id,char* userID){
    char* command = (char*)calloc(50,sizeof(char));
    char* targetID = (char*)calloc(5,sizeof(char));
    sprintf(targetID, "%d", id);
    strcpy(command,"touch rsc/");
    strcat(command,targetID);
    strcat(command,"/messagesFrom_");
    strcat(command,userID);
    strcat(command,".txt");
    system(command);
    
    free(targetID);
    free(command);
}
void saveMessagesToFile(char* targetID,char* userID,char* message){//saves received messages to txt file
     time_t times;
     time(&times);
     char* currentTime = (char*)calloc(120,sizeof(char));
     char* filePath = (char*)calloc(120,sizeof(char));
     struct tm* timeInfo = localtime(&times);
     strftime(currentTime, 120, "%Y-%m-%d/%H:%M:%S", timeInfo);
     char* finalForm = (char*)calloc(MAX,sizeof(char));
     FILE* file;
     
     strcpy(finalForm,userID);
     strcat(finalForm,": ");
     strcat(finalForm,message);
     strcat(finalForm,"  ");
     strcat(finalForm,currentTime);
     strcpy(filePath,"rsc/");
     strcat(filePath,targetID);
     strcat(filePath,"/messagesFrom_");
     strcat(filePath,userID);
     strcat(filePath,".txt");
     

     
     file = fopen(filePath, "a");
     
     if (file != NULL) {
     fprintf(file, "%s\n", finalForm);
        
    fclose(file);
    } else {
        fprintf(stderr, "\nError opening file for writing: %s\n", filePath);
    }

     
     free(currentTime);
     free(finalForm);
     free(filePath);
}

void findMessageHistory(char* userID,char* targetID,int socket){//sends all messages from certain user to client.
     char* filePath = (char*)calloc(120,sizeof(char));

     FILE* file;
     strcpy(filePath,"rsc/");
     strcat(filePath,userID);
     strcat(filePath,"/messagesFrom_");
     strcat(filePath,targetID);
     strcat(filePath,".txt");
     
     
     file = fopen(filePath, "r");
     
         if (file == NULL) {
        perror("\nError opening file\n");
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* containsAll = (char*)calloc(file_size+1,sizeof(char));
    char* finalForm = (char*)calloc(MAX,sizeof(char));
    fread(containsAll, 1, file_size, file);
    containsAll[file_size]='\0';

    strcpy(finalForm,"$");
    strcat(finalForm,containsAll);
    send(socket,finalForm,MAX,0);
     
    fclose(file);
    free(finalForm);
    free(filePath);
    free(containsAll);
}
