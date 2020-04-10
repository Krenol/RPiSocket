#include "rpisocket/rpisocket.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <map>

static std::map<int, std::string> statusMap = {{200, "OK"}, {201, "Created"}, {202, "Accepted"}, {400, "Bad Request"}};

std::string createRestMsg(int statusCode, std::string contentType, std::string charset, std::string msg) {
    std::time_t cur_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto time_str = std::to_string(cur_time);
    auto statusMsg = statusMap.count(statusCode) ? statusMap.at(statusCode) : "";
    auto response = "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMsg + "\n";
    response += "Server: RPi\nContent-Type: " + contentType + "; charset=" + charset + "\n";
    response += "Date: " + time_str + "\nKeep-Alive: timeout=5, max=1000\nConnection: Keep-Alive\nContent-Length: ";
    response += std::to_string(msg.length()) + "\n\n" + msg; 
    return response;
}


int main() {
    int port = 8888;
    std::cout << "creating wifi server on port " << port << std::endl;
    rpisocket::WiFiServer server(port);
    std::cout << "waiting for connection..." << std::endl;
    server.connect();
    std::cout << "got connection :)" << std::endl;
    std::cout << "connected client: " << server.getConnectedClient() << std::endl;
    std::string data = "<h1>Test Msg</h1>";
    
    server.writeBytes(createRestMsg(200, "text/html", "UTF-8", data));
    std::cout << "msg sent!" << std::endl << std::endl;

}