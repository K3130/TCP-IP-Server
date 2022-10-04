#include "Server.h"



Server::Server(uint16_t aPort) : m_port(aPort)
{
	Init(800, 600, "TCP/IP Server", ImVec4(0.45f, 0.55f, 0.60f, 1.00f));
}

void Server::UpdateGraphicScene()
{
    if (m_window_chat)
    {
        //ImGui::GetBackgroundDrawList()->AddCircle({ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f}, 10, ImColor{ 30, 255, 0, 255 }, 12, 2);
        ImGui::SetNextWindowPos(ImVec2(0.0f, ImGui::GetIO().DisplaySize.y - 0.0f), ImGuiCond_Always, ImVec2(0.0f, 1.0f));
        ImGui::SetNextWindowSize(ImVec2(600, 300), 0);
        ImGui::Begin("chat", &m_window_chat, ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoTitleBar);

        //ImGui::InputTextMultiline("ImputLine", &m_bufferText, ImVec2(-FLT_MIN, 250), ImGuiInputTextFlags_ReadOnly, NULL, NULL);
        //--
        ImGui::SetCursorPos(ImVec2(5.0f, 275.0f));
        ImGui::Text("Enter:");
        //--
        ImGui::SetCursorPos(ImVec2(50.0f, 270.0f));
        ImGui::PushItemWidth(470);
        //ImGui::InputText(" ", &m_message);
        //--
        ImGui::SetCursorPos(ImVec2(525.0f, 270.0f));
        if (ImGui::Button("Send", ImVec2(75, 19)))
        {
           // m_bufferText += m_message + '\n';

            //m_server.sendData(&m_message, m_message.size());
            //m_message.clear();
            //m_server.sendMessage(10000,m_message);
            //m_server.clear();
        }
        ImGui::End();
    }
}



