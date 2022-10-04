cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

add_executable(Server "main.cpp" "Server.h" "Server.cpp"
				"../Graphic/Graphic/Graphic.hpp"
				"../Networking/net_client.h" "../Networking/net_common.h" "../Networking/net_connection.h" "../Networking/net_message.h" "../Networking/net_server.h" "../Networking/net_tsqueue.h" "../Networking/olc_net.h")


find_package(glad CONFIG REQUIRED)
target_link_libraries(Server PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Server PRIVATE glfw)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Server PRIVATE imgui::imgui)

find_package(asio CONFIG REQUIRED)
target_link_libraries(Server PRIVATE asio::asio)