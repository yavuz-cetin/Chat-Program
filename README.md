Multithreaded chat program in C using socket programming.

To run the server: ./server

To run the client: ./client [any client id]

Problem Explanation
The goal of this project is to create a multithreaded messaging application using sockets. The server will create a socket and listen for client connections. Users should be able to send messages to people on their friends list, and they should receive notifications when a message arrives. Communication should occur directly between the server and client, with no peer-to-peer messaging between clients. Additionally, users should be able to:

Add friends.
Delete friends.
View information about all their friends.
See all messages received from a specific person of their choice.
All user data should be stored in files on the server. Upon startup, the server reads this data from the files, and it will provide the necessary information to the client when requested.

Solution
Client Connection:

When a client connects to the server, the server creates a dedicated thread to handle communication with that client. Similarly, the client creates a thread to listen for incoming messages from the server.
User Data Storage:

All registered user information is stored in directories named after their user IDs under the 'rsc' folder on the server.
The user provides their ID as an argument when launching the client program. The client sends this ID to the server.
User Registration:

If the provided ID is already registered, the user logs in successfully without requiring re-registration.
If the ID is not registered, the server prompts the client to start the registration process.
The client collects the necessary registration information from the user and sends it to the server.
The server creates a new user with this information and stores it in the appropriate files.
Operations:

After a successful login, the server presents the user with a list of operations they can perform, such as adding or deleting friends, viewing friend information, or retrieving messages.


Some Program Screenshots:

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/59e772fc-f20d-4704-9b3b-a73e2bc77678)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/268f6640-4b78-499f-bb3b-83cc6f84183a)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/6ce8cf10-c878-47b3-b150-36b294163285)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/48e22e51-48eb-497a-afca-8499c2f33b59)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/6d0cd16a-4be1-4365-bd47-a72ebac4313f)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/1b14fd51-8362-42b8-bcc0-b029c1a2a353)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/941bcab1-1e13-4f24-9f61-df8010551666)






