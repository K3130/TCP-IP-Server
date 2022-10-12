#include "Server.h"



Server::Server()
{
	Init(1024, 768, "TCP/IP Server", ImVec4(0.45f, 0.55f, 0.60f, 1.00f));
}

void Server::setPort(const std::string &aPort)
{ 
    m_port = aPort;
}

void Server::UpdateGraphicScene()
{
    if (m_window_chat)
    {
        ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y - 0.0f), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
        ImGui::SetNextWindowSize(ImVec2(600, 300), 0);
        ImGui::Begin("chat", &m_window_chat, ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoTitleBar);

        ImGui::InputTextMultiline("ImputLine", &m_bufer_chat, ImVec2(-FLT_MIN, 250), ImGuiInputTextFlags_ReadOnly, NULL, NULL);
        //--
        ImGui::SetCursorPos(ImVec2(5.0f, 275.0f));
        ImGui::Text("Enter:");
        //--
        ImGui::SetCursorPos(ImVec2(50.0f, 270.0f));
        ImGui::PushItemWidth(470);
        ImGui::InputText(" ", &m_bufer_message);
        //--
        ImGui::SetCursorPos(ImVec2(525.0f, 270.0f));
        if (ImGui::Button("Send", ImVec2(75, 19)))
        {
            m_bufer_chat += m_bufer_message + "\n\r";
            m_server->send_message_all_clients("[SERVER] " + m_bufer_message + "\n\r");
            m_bufer_message.clear();
        }
        //--

        ImGui::End();
    } 
    
    if (m_window_settings)
    {
        ImGui::SetNextWindowPos(ImVec2(830.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(200, 500), 0);
        ImGui::Begin("settins", &m_window_settings, ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("App average %.3f", 1000.0f / ImGui::GetIO().Framerate);
        ImGui::Text("ms/frame (%.1f FPS)", ImGui::GetIO().Framerate);
        //--
        ImGui::Text("Status:");
        ImGui::SameLine();
        
        if (m_is_server_running) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 100, 100, 255));
        }
        ImGui::Text(m_is_server_running ? "Running" : "Stop");
        ImGui::PopStyleColor();
        //--
        


        if (ImGui::Button("Start server", ImVec2(90, 19)))
        {
            setPort(m_port);
            startServer();
        }
        ImGui::SameLine();
        m_port = "60000";
        ImGui::InputText(" ", &m_port);

        if (ImGui::Button("Stop server", ImVec2(90, 19)))
        {
            stopServer();
        }
        if (ImGui::Button("Exit", ImVec2(90, 19)))
        {
            exitProgram();
        }
        //--

        //--
        ImGui::End();
    }

    if (m_window_users)
    {
        ImGui::SetNextWindowPos(ImVec2(500.0f, 0.0f));
        ImGui::SetNextWindowSize(ImVec2(200, 400), 0);

        ImGui::Begin("Online", &m_window_users);
        ImGui::PopStyleColor();

        if(!m_server->getClients().empty())
        {
            for (auto client : m_server->getClients())
            {
                ImGui::Text(std::to_string(client->get_id()).c_str());
                //--

            }
        }
        
         
   
        ImGui::End();
    }

    if (m_warning_message)
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(350, 120), 0);
        ImGui::Begin("Error", &m_warning_message, ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoTitleBar);
        //--
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(m_warning_text.c_str()).x) * 0.5f);
        ImGui::Text(m_warning_text.c_str());
        //--
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 75) / 2);
        if (ImGui::Button("OK", ImVec2(75, 20)))
            m_warning_message = false;
        //--
        ImGui::End();
    }
}


void Server::startServer()
{
    if (m_is_server_running)
    {
        m_warning_text = "Server is allready running!";
        m_warning_message = true;
    }
    else 
    {
        m_server = std::make_shared<CustomServer>(std::stoi(m_port));
        m_is_server_running = m_server->start_server();

        m_bufer_chat += m_server->get_error() + '\n';

        m_window_users = true;

    }
    
}

void Server::stopServer()
{   
    m_server->send_message_all_clients("[SERVER] The server will be stopped\n\r");
    m_server->stop_server();
    m_is_server_running = false;

    m_window_users = false;
}

void Server::exitProgram()
{
    if (m_is_server_running)
        stopServer();
    Shutdown();
    exit(0);
}



