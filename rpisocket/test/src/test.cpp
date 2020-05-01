#include "rpisocket/rpisocket.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>
#include <string>
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

void test(const std::string& val) {

}

int main() {
    int port = 8888;
    int number = 1;
    //rpisocket::BTServer server;
    rpisocket::WiFiServer server(port);
    std::string data;
    srand (static_cast <unsigned> (time(0)));
    while(true){
        std::cout << "waiting for connection..." << std::endl;
        server.connect();
        std::cout << "success!" << std::endl;
        bool status = 1;
        while(server.hasConnection() && status){
            try{
                float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                data = "{\"distLeft\":" + std::to_string(r) + "}";
                status = server.writeBytes(data);
                std::cout << "msg: " << server.readBytes() << std::endl;
            } catch(...){
                status = 0;
            }
        }
        server.disconnect();
    }
}