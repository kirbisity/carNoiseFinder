#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <unistd.h>

/* in current directory */
#include "Car.h"
#include "parser.h"

using namespace std;

#define NOISEENTRY 6
#define PORT 8081

/* g++ main.cpp parser.cpp Car.cpp Polynomial.cpp -o main.o */

int connect_tcp() {
	int sock = 0;
	struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    }
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        printf("\nConnection Failed \n"); 
        return -1; 
    }
    return sock;
}

bool close_connection(int sock) {
	char *close = "Close\n";
	char buffer[1024] = {0};
	send(sock , close , strlen(close) , 0 );
	int valread = read( sock , buffer, 1024);
    if (valread == -1) {
    	printf("graph.py cannot be stopped or is already stopped\n");
    	return false;
    }
    else if (strcmp(buffer,"OK!\n") == 0) {
    	return true;
    }
    else {
    	return false;
    }
}

void send_graph(int sock, vector<Car>& cars) {
	char *graph = "Graph\n"; 
    char buffer[1024] = {0}; 
    send(sock , graph , strlen(graph) , 0 );
    int valread = read( sock , buffer, 1024);
    if (valread == -1) {
    	printf("Graph cannot be drawn, make sure graph.py is running in the background.\n");
    }
    else if (strcmp(buffer,"OK!\n") == 0) {
    	return;
    }
    else {
    	printf("Graph cannot be drawn.\n");
    }
}

bool check_input_num(string text) {
	std::string::const_iterator it = text.begin();
    while (it != text.end() && std::isdigit(*it)) ++it;
    if (!text.empty() && it == text.end()) {
    	return true;
    }
    else if (strcmp(text, "") == 0) {
    	return false;
    }
    else {
    	cout << "Incorrect format, enter the number only" << endl;
    	return false;
    }
}

string get_input_num(void) {
	string text = "";
	while (!check_input_num(text)) {
		getline(cin, text);
	}
	return text;
}

int main() {
	//string command = "python3 graph.py ";
    //system(command.c_str());



	int sock = connect_tcp();
	vector<Car> cars;
	vector<double> speedentries;
	ifstream data;
	data.open("data.txt");
	ofstream output;
	output.open ("output.txt");
	if(!data) {
	    cout << "Cannot open input file" << endl;
	    return 1;
	}
	if(!output) {
	    cout << "Cannot open output file" << endl;
	    return 1;
	}
	string line;
	if (data) {
		getline(data, line);
		speedentries = parse_tabs(line);
	}
	
	while (data) {
		getline(data, line);
		vector<string> items = split(line);
		vector<string>::const_iterator i = items.end()-1;
		if ((items.size() > NOISEENTRY) && (atof(i->c_str()) != 0)) {
			vector<string> name;
			int year;
			vector<double> noises;
			int count = 0;
			for (i = items.end()-1; i != items.begin()-1; --i) {
				if (count < NOISEENTRY) {
					double dbvalue = atof(i->c_str());
					noises.push_back(dbvalue);
				}
				else if (count == NOISEENTRY) {
					year = atoi(i->c_str());
				}
				else {
					name.push_back(*i);
				}
				count++;
	    	}
	    	reverse(name.begin(), name.end());
	    	reverse(noises.begin(), noises.end());
			Car car(name, year, speedentries, noises);
			car.guess_displacement();
			car.noises_curve_fitting();
			cars.push_back(car);
		}
	}
	
	while (1) {
		string input = "";
		cout << "Enter \"s\" to search for your car" << endl;
		cout << "Enter \"xxx\"km/h to list noise data sorted by noise at the desired speed" << endl;
		cout << "Enter \"ed\" to list noise data sorted by engine displacement" << endl;
		cout << "Enter \"da\" to draw noise graph of all cars" << endl;
		cout << "Enter \"q\" to quit" << endl;
		getline(cin, input);
		if (input == "ed") {
			cout << "Enter \"0\" to list noises by engine displacement increasing order" << endl;
			cout << "Enter \"1\" to list noises by engine displacement decreasing order" << endl;
			getline(cin, input);
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_engine_up);
			}
			else {
				sort(cars.begin(), cars.end(), compare_engine_down);
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    if (i->displacement != 0) {
			    	cout << i->print();
			    	output << i->print();
			    }
			}
		}
		else if ((atof(input.c_str())!=0) || (input=="0") || (input=="0.0")) {
			double speed = atof(input.c_str());
			if (speed < 0) {
				cout << "speed should not be negative" << endl;
				continue;
			}
			if (speed > 400) {
				cout << "speed should not be above 400km/h" << endl;
				continue;
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    i->guess_noise(speed);
			}
			cout << "Enter \"0\" to list noises at "<< input <<"km/h increasing order" << endl;
			cout << "Enter \"1\" to list noises at "<< input <<"km/h decreasing order" << endl;
			getline(cin, input);
			if (input == "0") {
				sort(cars.begin(), cars.end(), compare_noise_up);
			}
			else if (input == "1") {
				sort(cars.begin(), cars.end(), compare_noise_down);
			}
			else {
				continue;
			}
			for (vector<Car>::iterator i = cars.begin(); i != cars.end(); ++i) {
			    cout << i->print();
			    output << i->print();
			    i->target_noise=0;
			}
			cout<<"predicted noises are at "<<speed<<" km/h"<<endl;
		}
		else if (input == "s") {
			cout << "Enter the name of the car" << endl;
			getline(cin, input);
			vector<Car> results = search_car(cars, input);
			if (results.size() == 0) {
				cout<<"no car with the name was found"<<endl;
			}
			else {
				int num = 1;
				for (vector<Car>::iterator i = results.begin(); i != results.end(); ++i) {
					cout << num << ": " << i->print_name() << endl;
				    ++num;
				}
				cout << "Enter the number of the car you want" << endl;
				input = get_input_num();
				int id = atoi(input.c_str());
				if (id >= num || id < 1) {
					cout << "No car is numbered" << id << endl;
					continue;
				}

				Car& rcar = results[id - 1];

				cout << "Enter the \"xxx\"km/h to print the noise data of" << rcar.print_name() << endl;

				input = get_input_num();
				double speed = atof(input.c_str());
				if (speed < 0) {
					cout << "speed should not be negative" << endl;
					continue;
				}
				if (speed > 400) {
					cout << "speed should not be above 400km/h" << endl;
					continue;
				}
				rcar.guess_noise(speed);
				cout << rcar.print();
				rcar.target_noise=0;
				cout<<"predicted noises are at "<<speed<<" km/h"<<endl;
			}
		}
		else if (input == "da") {
			cout << "Drawing graphs..." << input << endl;
			send_graph(sock, cars);
		}
		else if (input == "q") {
			break;
		}
		else {
			cout << "Unknown command: " << input << endl;
		}
		cout<<endl;
		cout << "Enter to continue" << endl;
		getline(cin, input);
		if (input == "q") {
			break;
		}
	}
	close_connection(sock);
	data.close();
	output.close();
	return 0;
}