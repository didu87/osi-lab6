//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include "zmq.hpp"
#include <string>
#include <iostream>

using namespace std;
 
int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
 
    cout << "Connecting to hello world server…" << endl;
    socket.connect ("tcp://localhost:5555");
 
    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		string s;
		getline(cin, s, '\n');
        zmq::message_t request(s.length());
        memcpy ((void *) request.data(), s.c_str(), s.length());
        socket.send (request);
 
        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
		string res(static_cast<char*>(reply.data()),reply.size());
        cout << "Received reply: " << res << endl;
    }
    return 0;
}
