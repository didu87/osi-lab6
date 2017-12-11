#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

#include "common.cpp"

int menu()
{
	cout<<"| 0-Exit | 1-Amount | 2-Put | 3-Get | 4-Transfer client | 5-Transfer bank |"<<endl;
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
	string port=string("tcp://localhost:")+string(argv[1]);
	string key(argv[2]);

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
 
    socket.connect(port);
	cout<<"Client '"<<key<<"' connect to bank on "<<port<<endl;
 
	int op=menu();
	while(op)
	{
		string request, dest_key, dest_bank;
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
				cin>>dest_key; 
				cout<<"sum to transfer: "; cin>>sum;
				request = string("transfer_client ") + int_to_str(sum) + string(" ") + key + string(" ") + dest_key;
			break;
			case 5:
				cout<<"destination bank port: ";
				cin>>dest_bank; 
				cout<<"sum to transfer: "; cin>>sum;
				request = string("transfer_bank ") + int_to_str(sum) + string(" ") + key + string(" ") + dest_bank;
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
