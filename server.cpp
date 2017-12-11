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
		string req(static_cast<char*>(request.data()));
        cout << "Received request: [";
		cout << req; 
		cout << "]" << endl;
 
        //  Do some 'work'
		stringstream ss(req);
		int a,b;
		char c;
		ss>>a>>c>>b;
		cout<<a<<c<<b<<endl;
		int res;
		switch(c)
		{
			case '+': res=a+b; break;
			case '-': res=a-b; break;
			case '*': res=a*b; break;
			case '/': res=a/b; break;
		}
 		cout<<"result: "<<res<<endl;
        sleep(1);
 
        //  Send reply back to client
        zmq::message_t reply (20);
		ostringstream oss;
		oss<<res;
		string rep=oss.str();
        memcpy ((void *) reply.data (), rep.c_str(), rep.length());
        socket.send (reply);
    }
    return 0;
}

