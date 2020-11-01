#include "rpisocket/rpisocket.hpp"
#include <iostream>
#include <chrono>
#include <ctime>
#include <functional>
#include <string>
#include <cstdlib>


int main() {
    int port = 8888;
    
    //rpisocket::BTServer server;
    rpisocket::WiFiServer server(port);
    std::cout << "created socket on port " << port << " with ip address " << server.getServerIp() << std::endl;
    srand (static_cast <unsigned> (time(0)));
    std::string msg, cur, delimeter_ = "\n";
    int buf;
    while(true){
        std::cout << "waiting for connection..." << std::endl;
        server.connect();
        std::cout << "success!" << std::endl;
        while(server.hasConnection()){
             try{
                //read incoming msg
                msg = "";
                cur = "";
                buf = 0;
                while(buf++ < msg_size_ && std::string::compare(cur, delimeter_) != 0){
                    readBytes(cur);
                    msg += cur;
                }
                
                //notify all subscribed agents
                std::cout << "read msg: " << msg << std::endl;
            } catch(...) {
                // on error stop thread
                break;
            }
        }
        server.disconnect();
    }
}