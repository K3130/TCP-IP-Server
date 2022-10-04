#include "../Networking/olc_net.h"
#include "../Graphic/Graphic/Graphic.hpp"


class Server : public Graphic::ImGuiWindowOpengl<Server>
{
public:
	Server() = default;
	Server(uint16_t aPort);
	void UpdateGraphicScene();
private:
	uint16_t m_port;
private: 
	bool m_window_chat = true;

};