#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;
 
struct Node
{
	string key;
	float value;
	Node *l, *r;
	Node(): value(0), l(0), r(0) {}
	Node(string k, float v): key(k), value(v), l(0), r(0) {}
};

class BTree
{
	Node *tree;
	Node *rappend(string key, float value, Node *&t)
	{
		if(t==0)
		{
			t = new Node(key, value);
			return t;
		}
		if(key < t->key)
			return rappend(key, value, t->l);
		if(key > t->key)
			return rappend(key, value, t->r); 
		return t;
	}
	void rprint(Node *t, int u)
	{
		if(t==0 && u==0) t=tree; 
		if(t==0) return;
		rprint(t->l,++u);
		for(int i=0;i<u;++i) cout<<"\t";
		cout<<t->key<<":"<<t->value<<endl;
		u--;
		rprint(t->r, ++u);
	}

public:
	BTree(): tree(0) {}
	float& operator[](string key) { return rappend(key, 0, tree)->value; }
	void print() { rprint(0,0); cout<<endl<<endl; }
};

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

int main () {
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
    socket.bind ("tcp://*:5555");
 
    while (true) {
		string req = wait_msg(socket);
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
		send_msg(socket, "ok");
    }
    return 0;
}

