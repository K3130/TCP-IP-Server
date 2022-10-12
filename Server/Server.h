#include "CustomServer.hpp"
#include "../Graphic/Graphic/Graphic.hpp"

class Server : public Graphic::ImGuiWindowOpengl<Server> //CRTP
{
public:
	Server();
	void setPort(const std::string &aPort);
	void UpdateGraphicScene();
private:
	std::string m_port;
	std::string m_bufer_chat;
	std::string m_bufer_message;
	std::string m_warning_text;
	std::shared_ptr<CustomServer> m_server;
private: 
	bool m_window_chat = true;
	bool m_window_settings = true;
	bool m_window_users = false;
	bool m_warning_message = false;
	bool m_is_server_running = false;
private:
	void startServer();
	void stopServer();
	void exitProgram();
};