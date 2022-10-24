#include "Server.h"
#include "triangle.hpp"
#include <filesystem>


Server::Server()
{
	Init(1024, 768, "TCP/IP Server");
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
            if (m_is_server_running && !m_server->getClients().empty())
            {
                m_bufer_chat += m_bufer_message + "\n\r";
                m_server->send_message_all_clients("[SERVER] " + m_bufer_message + "\n\r");
                m_bufer_message.clear();
            }
            else
            {
                m_warning_text = "There's nobody here.";
                m_warning_message = true;
                m_bufer_message.clear();
            }
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

void Server::OnRender()
{
    const unsigned int width = 1024;
    const unsigned int height = 768;

    GLfloat vertices[] =
    { //vertices                /normals               /texture coord  //
     -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
         
    };

    GLuint indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTex;\n"
    "out vec3 color;\n"
    "out vec2 texCoord;\n"
    "uniform float scale;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
    "   color = aColor;\n"
    "   texCoord = aTex;\n"
    "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 color;\n"
    "in vec2 texCoord;\n"
    "uniform sampler2D tex0;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(tex0, texCoord);\n"
    "}\n\0";

    Shader shaderProgram(vertexShaderSource, fragmentShaderSource);
    
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
    std::string parentDir = std::filesystem::current_path().std::filesystem::path::parent_path().string();
    Texture brickTex((parentDir + "\\texture.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(shaderProgram, "tex0", 0);
    

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    shaderProgram.Activate();

    
    double crntTime = glfwGetTime();
    if (crntTime - m_prevTime >= 1 / 60)
    {
        m_rotation += 0.5f;
        m_prevTime = crntTime;
    }

    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    
    model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    //position
    view = glm::translate(view, glm::vec3(-0.3f, -0.1f, -2.0f));

    proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

    
    int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

    glUniform1f(uniID, 0.5f);
    brickTex.Bind();
    VAO1.Bind();
    
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
    
    
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    brickTex.Delete();
    shaderProgram.Delete();

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
    if (m_is_server_running)
    {
        m_server->send_message_all_clients("[SERVER] The server will be stopped\n\r");
        m_server->stop_server();
        m_is_server_running = false;
        m_window_users = false;
    }
    else
    {
        m_warning_text = "Server not running!";
        m_warning_message = true;
    }
}
    

void Server::exitProgram()
{
    if (m_is_server_running)
        stopServer();
    Shutdown();
    exit(0);
}







