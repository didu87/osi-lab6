#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

void send_msg(zmq::socket_t &socket, string msg)
{
	zmq::message_t zmq_msg(msg.length());
	memcpy ((void *) zmq_msg.data(), msg.c_str(), msg.length());
	socket.send (zmq_msg);
}

string wait_msg(zmq::socket_t &socket) 
{
	zmq::message_t zmq_msg;
	socket.recv(&zmq_msg);
	string msg(static_cast<char*>(zmq_msg.data()), zmq_msg.size());
	return msg;
}

string int_to_str(int x)
{
	stringstream oss;
	oss<<x;
	return oss.str();
}

int menu()
{
	cout<<"| 0-Exit | 1-Amount | 2-Put | 3-Get | 4-Transfer |"<<endl;
	cout<<"operation: ";
	int choise;
	cin>>choise;
	return choise;
}

int main(int argc, char **argv)
{
	if(argc<3)
	{
		cout<<"Specify the port and the client name"<<endl;
		exit(1);
	}	
	string key(argv[2]);

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
 
    cout << "Connecting to server..." << endl;
    socket.connect ("tcp://localhost:5555");
 
	int op=menu();
	while(op)
	{
		string request;
		int sum;
 		switch(op)
		{
			case 1:
				request = string("amount ") + key;
			break;
			case 2:
				cout<<"sum to put: "; cin>>sum;
				request = string("put ") + int_to_str(sum) + string(" ") + key;
			break;
			case 3:
				cout<<"sum to get: "; cin>>sum;
				request = string("get ") + int_to_str(sum) + string(" ") + key;
			break;
			case 4:
				cout<<"destination account key: ";
				string dest_key; cin>>dest_key; 
				cout<<"sum to transfer: "; cin>>sum;
				request = string("transfer ") + int_to_str(sum) + string(" ") + key + string(" ") + dest_key;
			break;
		}
 		send_msg(socket,request);
 
        //  Get the reply.
		string reply = wait_msg(socket);
        cout << "Bank reply: " << reply << endl;
		op=menu();
    }
    return 0;
}
