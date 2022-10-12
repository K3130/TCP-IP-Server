#include "../Networking/net_tcp_server.h"


class CustomServer : public net::Server
{
public:

	CustomServer(uint16_t aPort) : net::Server(aPort)
	{

	}

	virtual bool on_client_connect(std::shared_ptr<net::Session> aClient) 
	{	
		return true; 
	}

	
	//virtual void on_client_disconnect(std::shared_ptr<net::Session> aClient)
	//{

	//}

	//virtual void on_client_message(std::shared_ptr<net::Session> aClient, net::message_type &aType, std::string &aMessage) 
	//{

	//}



};
