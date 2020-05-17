#include "rpisocket/rpisocket.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>
#include <string>
#include <cstdlib>


int main() {
    int port;
    try{
        auto env_var = getenv("SOCKET_PORT");
        if(env_var){
            std::cout << "env_var: " << env_var << std::endl;
            port = std::stoi(env_var); 
        } else {
            std::cout << "env_var: NULL!!" << std::endl;
            port = 8888;
        }
        
    } catch(...){
        port = 8888;
    }
    
    int number = 1;
    //rpisocket::BTServer server;
    rpisocket::WiFiServer server(port);
    std::string data;
    std::cout << "created socket on port " << port << " with ip address " << server.getServerIp() << std::endl;
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