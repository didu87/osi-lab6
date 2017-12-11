#include "zmq.hpp"
#include <string>
#include <iostream>

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

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
 
    cout << "Connecting to server..." << endl;
    socket.connect ("tcp://localhost:5555");
 
    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
		string s;
		getline(cin, s, '\n');
 		send_msg(socket, s);
 
        //  Get the reply.
		string res = wait_msg(socket);
        cout << "Received reply: " << res << endl;
    }
    return 0;
}
