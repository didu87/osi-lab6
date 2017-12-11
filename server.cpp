#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;
 
int main () {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");
 
    while (true) {
        zmq::message_t request;
 
        //  Wait for next request from client
        socket.recv (&request);
		string req(static_cast<char*>(request.data()), request.size());
        cout << "Received request: [";
		cout << req; 
		cout << "]" << endl;
 
        //  Do some 'work'
		stringstream iss(req);
		string key, op;
		float sum;
 		
		iss>>key>>op;
		if(op=="put")
		{
			iss>>sum;
			cout<<"put "<<sum<<" to "<<key<<endl;
		}
		if(op=="get")
		{
			iss>>sum;
			cout<<"get "<<sum<<" from "<<key<<endl;
		}
		if(op=="amount")
		{
			cout<<"amount of "<<key<<endl;
		}
 
        //  Send reply back to client
		ostringstream oss("ok");
		string rep=oss.str();
        zmq::message_t reply(rep.length());
        memcpy ((void *) reply.data (), rep.c_str(), rep.length());
        socket.send (reply);
    }
    return 0;
}

