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
