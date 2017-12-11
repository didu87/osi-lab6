#include "zmq.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace std;
 
struct Node
{
	string key;
	int value;
	Node *l, *r;
	Node(): value(0), l(0), r(0) {}
	Node(string k, int v): key(k), value(v), l(0), r(0) {}
};

class BTree
{
	Node *tree;
	Node *rappend(string key, int value, Node *&t)
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
	int& operator[](string key) { return rappend(key, 0, tree)->value; }
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

int main (int argc, char **argv)
{
	if(argc<2)
	{
		cout<<"Specify the port of server!"<<endl;
		exit(1);
	}

    // Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);
	string port=string("tcp://*:")+string(argv[1]);
    socket.bind(port);
 	cout<<"Back server started on port "<<port<<endl;
 
	// Prepare binary tree of clients	
	BTree account;

    while (true) {
		string req = wait_msg(socket);
        cout << "Received request: [" << req << "]" << endl;
 
        //  Do some 'work'
		stringstream iss(req);
		string op, key;
		int sum;
 		
		iss>>op;
		if(op=="put")
		{
			iss>>sum>>key;
			cout<<"put "<<sum<<" to "<<key<<endl;
			account[key]+=sum;
			send_msg(socket, "ok");
		}
		else if(op=="get")
		{
			iss>>sum>>key;
			cout<<"get "<<sum<<" from "<<key<<endl;
			int funds=account[key];
			if(sum<=funds)
			{
				account[key]-=sum;
				cout<<"ok"<<endl;
				send_msg(socket, "ok");
			}
			else
			{
				cout<<"no enouth funds"<<endl;
				send_msg(socket, "no enouth funds");
			}
		}
		else if(op=="amount")
		{
			iss>>key;
			cout<<"amount of "<<key<<endl;
			int funds=account[key];
			stringstream oss;
			oss<<funds;
			send_msg(socket, oss.str());
		}
		else if(op=="transfer")
		{
			string dest_key;
			iss>>sum>>key>>dest_key;
			cout<<"transfer "<<sum<<" from "<<key<<" to"<<dest_key<<endl;
			int funds=account[key];
			if(sum<=funds)
			{
				account[key]-=sum;
				account[dest_key]+=sum;
				cout<<"ok"<<endl;
				send_msg(socket, "ok");
			}
			else
			{
				cout<<"no enouth funds"<<endl;
				send_msg(socket, "no enouth funds");
			}
		}
		else
		{
			send_msg(socket, "uknown request");
		}

		account.print();
    }
    return 0;
}

