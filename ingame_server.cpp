
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
#include <vector>
#include <string>
#include "const.h"
#include "utils.cpp"
#include <thread>
#include <atomic>
#include <functional>
#include <cstdio>
#include <chrono>
#include <ctime>


using namespace std;	

#define TRUE 1
#define FALSE 0
#define PORT 8080
	
const int N  = 1; // maximum number of clients
const int num_question = 4; // maximum number of questions
const int TIME_LIMIT = 30; // time out for each question

class Timer {
	public:
	Timer(int socket){
		this->socket = socket;
	}
	~Timer(){
		if (is_running){
			stop();
		}
	}
	
	
	typedef function<void(int)> Timeout;
	typedef chrono::milliseconds Interval;

	void start(int time_limit, const Interval &interval, const Timeout &timeout){
		is_running = true;
		timer_thread = std::thread([this, time_limit, interval, timeout](){
			int count = 0;
			while (is_running){
				cout << count << endl;
				this_thread::sleep_for(interval);
				count += 1;
				if (count >= time_limit) {
					timeout(socket);
					break;
				}
			}
		});
	}
	void stop(){
		cout << "Timer stopped" << endl;
		is_running = false;
		timer_thread.join();
	}

	
	private:
	int socket;
	atomic_bool is_running{};
	std::thread timer_thread{};

};

int main(int argc , char *argv[])
{	
	// random num_question 
	vector<string> serialized_questions {"a|1: What is the capital of United State? | Washington DC | Ho Chi Minh | Haiphong | Da Nang",
                                "a|2: What is the capital of Japan? | Tokyo | Osaka | Kyoto | Yokohama",
                                "a|3: What is the capital of China? | Beijing | Shanghai | Guangzhou | Shenzhen",
                                "a|4: What is the capital of India? | New Delhi | Mumbai | Kolkata | Chennai"}; 
	vector<string> answer{"A", "A", "A", "A"};
	// vector<string> serialized_questions;
	// vector<string> answer;

	// random_M_questions(serialized_questions, answer, num_question);

	 // track the current question index on each client, index = -1 means the client is disconnected
    int cur_question_index = 0;

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

	// ====================== wait for all players to connect ========================
	while (num_clients < max_clients){
		//clear the socket set
		FD_ZERO(&readfds);
	
		//add master socket to set
		FD_SET(master_socket, &readfds);
		
		//If something happened on the master socket ,
		//then its an incoming connection
		if (FD_ISSET(master_socket, &readfds)) {
			if ((new_socket = accept(master_socket,
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}
			
			// inform user of socket number - used in send and receive commands
			printf("New connection , socket fd is %d , ip is : %s , port : %d\n" , new_socket , inet_ntoa(address.sin_addr) , 
				ntohs(address.sin_port));

			// if( send(new_socket, message, strlen(message), 0) != strlen(message)) {
			// 	perror("send");
			// }

			// cout << "Welcome message sent" << endl;
			//add new socket to array of sockets
			for (int i = 0; i < max_clients; i++) {
				//if position is empty
				if( client_socket[i] == 0 ) {
					client_socket[i] = new_socket;
					printf("Adding to list of sockets as %d\n" , i);
					break;
				}
			}
		}

		// TODO: implement if a client disconnects
		num_clients ++;
	}

	cout << "Start game" << endl;

	// ============================== start the game =================================
	int id = 0; //current client
	int socket;

	
	while (true){
		socket = client_socket[id];
		Timer timer(socket);
		
		// // read message notifying that the client is ready to receive the question
		// memset(buffer, 0, sizeof(buffer));
		// valread = recv(socket, buffer, 1024, 0);
		// cout << valread << endl;
		// cout << buffer << endl;

		// // send the first question
		// string question = serialized_questions[0];
		// if (send(socket, question.c_str(), question.length(), 0) != question.length())
		// 	perror("send");

		// cout << "First question sent: " << question << endl;

		while (true) {	

			timer.start(15, chrono::milliseconds(1000), [](int s) {
				cout << "Time out" << endl;
				char buffer[2];
				buffer[0] = TIME_OUT;
				buffer[1] = '\0';
				if (send(s, buffer ,2 , 0) != 2)
					perror("send");
				close(s);
			});


			cout << "HUhuhuh" << endl;

			memset(buffer, 0, sizeof(buffer));
			valread = recv(socket, buffer, 1024, 0);
			timer.stop(); 


			
			cout << "Message received!" << endl;

			// if time out, notify the client and move to the next client
			// if (!timer.is_running){
			// 	cout << "Time out" << endl;
			// 	buffer[0] = TIME_OUT;
			// 	buffer[1] = '\0';
			// 	if (send(socket, buffer ,2 , 0) != 2)
			// 		perror("send");

			// 	// TODO: move to next client
			// 	break;
			// }
			
			if (valread == 0){
				// some disconnected
				getpeername(socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
				printf("Host disconnected, ip %s, port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

				// close the socket and mark as 0 in list for reuse
				close(socket);
				client_socket[id] = 0;
				cur_question_index += 1;
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

				if (buffer[0] == ANSWER && buffer[1] == answer[cur_question_index][0]){
					printf("Correct answer\n");
					cur_question_index += 1;

					// finished all questions
					if (cur_question_index == num_question){
						printf("Client %d finished the quiz\n", i);
						char message[2];
						message[0] = WIN_GAME;
						message[1] = '\0';
						if (send(socket, message, strlen(message), 0) != strlen(message)){
							perror("send");
						}
						close(socket);
						client_socket[id] = 0;
						return 0;
					}
					// send next question
					else{
						string question = serialized_questions[cur_question_index];
						if (send(socket, question.c_str(), question.length(), 0) != question.length())
							perror("send");
						cout << "Result and next question sent!" << endl;

					}
				}
				else if (buffer[0] == MOVE){
					cout << "Client " << id << " pass his turn" << endl;
					id += 1;
					if (id == max_clients){
						cout << "Game finished" << endl;
						return 0;
					}
					break;
				}
				else{
					printf("Wrong answer\n");
					char message[2];
					message[0] = WRONG_ANSWER;
					message[1] = '\0';
					if (send(socket, message, strlen(message), 0) != strlen(message)){
						perror("send");
					}
				}
			}
		}

		break;
	
	}
	
		
	return 0;

}