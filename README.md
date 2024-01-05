To run the server: ./server

To run the client: ./client [any client id]

Problem Explanation

In this project, it is aimed to create a multithreaded messaging application that uses sockets. Server
should create a socket and Client connections should be listened. Users should be able to send
messages to people in their friends list. The user should be notified when a message arrives. Direct
communication should only be between server and client. Users should be able to add friends. Users
should be able to delete friends. Users should be able to view the information of all their friends.
Users should be able to see all messages received from a person of their choice. All information
should be read from the files by the server at the first runtime and forwarded to the client when
necessary.

Solution

When the client first connects to the server, a thread is created on the client that listens for
messages from the server. Likewise, a thread is created on the server side that listens to this client.
All information of registered users is stored in folders named with their ids under the 'rsc' folder.
The user gives the id as an argument when running the program. The client program forwards this id
to the server. If this id has been registered before, no registration is required and the login is
successful. If this id has not been registered before, registration is started. Server informs the user
that registration is required. On the client side, the necessary information is received from the user.
The Client program transmits this information to the Server. Server creates a new user with this
information and creates the necessary storage files.
After a successful login, the user is asked for the operation number he/she wants to perform. What
these operations do will be explained in detail in the function explanations section.

Some Program Screenshots:

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/59e772fc-f20d-4704-9b3b-a73e2bc77678)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/268f6640-4b78-499f-bb3b-83cc6f84183a)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/6ce8cf10-c878-47b3-b150-36b294163285)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/48e22e51-48eb-497a-afca-8499c2f33b59)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/6d0cd16a-4be1-4365-bd47-a72ebac4313f)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/1b14fd51-8362-42b8-bcc0-b029c1a2a353)

![image](https://github.com/yavuz-cetin/Chat-Program/assets/77074984/941bcab1-1e13-4f24-9f61-df8010551666)






