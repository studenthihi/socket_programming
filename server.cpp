//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <iostream>
using namespace std;	

#define TRUE 1
#define FALSE 0
#define PORT 8080
	
const int N  = 4; // maximum number of clients
const int num_question = 4; // maximum number of questions
char serialized_questions[num_question][1000] = {"What is the capital of Vietnam? | Hanoi | Ho Chi Minh | Haiphong | Da Nang",
                                "What is the capital of Japan? | Tokyo | Osaka | Kyoto | Yokohama",
                                "What is the capital of China? | Beijing | Shanghai | Guangzhou | Shenzhen",
                                "What is the capital of India? | New Delhi | Mumbai | Kolkata | Chennai"}; 
char answer[num_question][10] = {"1", "1", "1", "1"};

int main(int argc , char *argv[])
{
	 // track the current question index on each client, index = -1 means the client is disconnected
    int cur_question_index[N] = {-1};

	int opt = TRUE;
	int master_socket , addrlen , new_socket , client_socket[N] ,
		max_clients = N , activity, i , valread , sd;
	int max_sd;
	struct sockaddr_in address;
		
	char buffer[1025]; //data buffer of 1K
		
	//set of socket descriptors
	fd_set readfds;
		
	char message[]= "Hello from server";
	
	//initialise all client_socket[] to 0 so not checked
	for (i = 0; i < max_clients; i++)
	{
		client_socket[i] = 0;
	}
		
	//create a master socket
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	//set master socket to allow multiple connections ,
	//this is just a good habit, it will work without this
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
		sizeof(opt)) < 0 )
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	
	//type of socket created
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );

	// mynote: htons: converts 32-bit (4-byte) quantities from host byte order to network byte order
		
	//bind the socket to localhost port 8888
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", PORT);
		
	//try to specify maximum of 3 pending connections for the master socket
	if (listen(master_socket, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
		
	//accept the incoming connection
	addrlen = sizeof(address);
	puts("Waiting for connections ...");

	int num_clients = 0;

	// // wait for all players to connect
	// while (num_clients < max_clients){

	// }

	
		
	while(TRUE) {
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
			
		//add child sockets to set
		for ( i = 0 ; i < max_clients ; i++)
		{
			//socket descriptor
			sd = client_socket[i];
				
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
				
			//highest file descriptor number, need it for the select function
			if(sd > max_sd)
				max_sd = sd;
		}
	
		//wait for an activity on one of the sockets , timeout is NULL ,
		//so wait indefinitely
		activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
	
		if ((activity < 0) && (errno!=EINTR))
		{
			printf("select error");
		}
			
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds))
		{
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			//inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , 
				ntohs(address.sin_port));
		
			//send new connection greeting message
			// 1st send 
			if( send(new_socket, message, strlen(message), 0) != strlen(message)) {
				perror("send");
			}	
				
			//add new socket to array of sockets
			int id;
			for (id = 0; id < max_clients; id++) {
				//if position is empty
				if( client_socket[id] == 0 ) {
					client_socket[id] = new_socket;
					printf("Adding to list of sockets as %d\n" , id);
					break;
				}
			}

			sleep(3);

			// send the first question
			cur_question_index[id] = 0;
			char *question = serialized_questions[0];
			if (send(new_socket, question, strlen(question), 0) != strlen(question)){
				perror("send");
			}
        
            while (true) {	
				memset(buffer, 0, sizeof(buffer));
                valread = read(new_socket, buffer, 1024);

                if (valread == 0){
                    // some disconnected
                    getpeername(new_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Host disconnected, ip %s, port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    // close the socket and mark as 0 in list for reuse
                    close(new_socket);
                    client_socket[id] = 0;
                    cur_question_index[id] = -1;
                    break;
                }
                else if (valread < 0){
                    perror("read");
                    break;
                }
                else{
                    // set the string terminating NULL byte on the end of the data read
                    buffer[valread] = '\0';
                    printf("Client %d answer: %s\n", id, buffer);

                    if (strcmp(buffer, answer[cur_question_index[id]]) == 0){
                        printf("Correct answer\n");
						cur_question_index[id] += 1;

						// finished all questions
                        if (cur_question_index[id] == num_question){
                            printf("Client %d finished the quiz\n", i);
                            char message[] = "You finished the quiz";
                            if (send(new_socket, message, strlen(message), 0) != strlen(message)){
                                perror("send");
                            }
                            close(new_socket);
                            client_socket[id] = 0;
                            cur_question_index[id] = -1;
                            break;
                        }
						// send next question
                        else{
                            char *question = serialized_questions[cur_question_index[id]];
                            if (send(new_socket, question, strlen(question), 0) != strlen(question)){
                                perror("send");
                            }
	
                        }
                    }
					else if (strcmp(buffer, "move") == 0){
						cout << "Client " << id << " pass his turn" << endl;
						break;
					}
                    else{
                        printf("Wrong answer\n");
                        char message[] = "Wrong answer";
                        if (send(new_socket, message, strlen(message), 0) != strlen(message)){
                            perror("send");
                        }
                    }
                }
            }
		}
			
		// else if there are some IO operation on some other sockets
		for (i = 0; i < max_clients; i++)
		{
			sd = client_socket[i];
				
			if (FD_ISSET( sd , &readfds))
			{
				//Check if it was for closing , and also read the
				//incoming message
				if ((valread = read( sd , buffer, 1024)) == 0)
				{
					//Somebody disconnected , get his details and print
					getpeername(sd , (struct sockaddr*)&address , \
						(socklen_t*)&addrlen);
					printf("Host disconnected , ip %s , port %d \n" ,
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
						
					//Close the socket and mark as 0 in list for reuse
					close( sd );
					client_socket[i] = 0;
				}
					
				//Echo back the message that came in
				else
				{
					//set the string terminating NULL byte on the end
					//of the data read
					buffer[valread] = '\0';
					cout << buffer <<endl;
					// send(sd , buffer , strlen(buffer) , 0 );
				}
			}
		}
	}
		
	return 0;
}
