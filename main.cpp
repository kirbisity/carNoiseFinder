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

bool send_message(int sock, string message) {
	char buffer[1024] = {0}; 
	const char* signal = message.c_str();
    send(sock, signal, strlen(signal), 0);
    int valread = read( sock , buffer, 1024);
    if (!(valread != -1 && strcmp(buffer,"OK!\n") == 0)) {
    	return false;
    }
    return true;
}

bool close_connection(int sock) {
	if (!send_message(sock, "Close\n")) {
    	printf("graph.py cannot be stopped or is already stopped\n");
    	return false;
	}
    return true;
}

void send_one_graph(int sock, string name, vector<string>& data) {
    if (!send_message(sock, "OneGraph\n"))
    	printf("Graph cannot be drawn, make sure graph.py is running in the background.\n");
    if (!send_message(sock, name))
    	printf("Graph cannot be drawn, make sure graph.py is running in the background.\n");
    else {
    	vector<string>::const_iterator it = data.begin();
    	while (it != data.end()){
    		if (!send_message(sock, *it)) {
    			cout<<"Graph cannot be drawn, connection interuppeted";
    			break;
    		}
    		++it;
    	}
    	if (!send_message(sock, "EndGraph\n"))
    		printf("Graph cannot be drawn, make sure graph.py is running in the background.\n");
    }
    return;
}

bool check_input_num(string text) {
	std::string::const_iterator it = text.begin();
    while (it != text.end() && std::isdigit(*it)) ++it;
    if (!text.empty() && it == text.end()) {
    	return true;
    }
    else if (text == "") {
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

bool search_one_car(vector<Car>& cars, Car& rcar) {
	string input;
	getline(cin, input);
	vector<Car> results = search_car(cars, input);
	if (results.size() == 0) {
		cout<<"no car with the name was found"<<endl;
		return false;
	}
	else {
		int num = 1;
		for (vector<Car>::iterator i = results.begin(); i != results.end(); ++i) {
			cout << num << ": " << i->print_name() << endl;
		    ++num;
		}
		cout << "Enter the number ("<<1<<"-"<<num-1<< ") of the car you want" << endl;
		input = get_input_num();
		int id = atoi(input.c_str());
		if (id >= num || id < 1) {
			cout << "No car is numbered " << id << endl;
			return false;
		}
		rcar = results[id - 1];
		return true;
	}
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
		cout << "Enter \"da\" to draw noise/speed graph of a car" << endl;
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
			cout << "Enter the name of the car you want to search" << endl;
			Car rcar;
			if (search_one_car(cars, rcar)) {

				cout << "Enter the \"xxx\"km/h to print the noise data of " << rcar.print_name() << endl;

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
			send_message(sock, "DrawGraph\n");
			input = "Y";
			while (input == "Y" || input == "y") {
				cout << "Enter the name of the car you want to draw a graph for" << endl;
				Car rcar;
				if (search_one_car(cars, rcar)) {
					vector<string> speeddata = rcar.get_speed_graph(250, 10);
					send_one_graph(sock, rcar.print_name(), speeddata);
				}
				else {
					cout << "No search results" << endl;
				}
				cout << "Type Y to add another car or any other key to continue" << endl;
				getline(cin, input);
			}
			send_message(sock, "End\n");
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