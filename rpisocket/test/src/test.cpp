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
    std::cout << "created socket on port " << port << " with ip address " << server.getServerAddress() << std::endl;
    srand (static_cast <unsigned> (time(0)));
    std::string msg, buf = "", delimiter_ = "\n";
    int msg_size_=150;
    
    while(true){
        std::cout << "waiting for connection..." << std::endl;
        server.connect();
        std::cout << "success!" << std::endl;
        std::size_t pos;
        while(server.hasConnection()){
             try{
                //read incoming msg
                while(1){
                    server.readBytes(msg);
                    buf += msg;
                    while ((pos = buf.find(delimiter_)) != std::string::npos) {
                        msg = buf.substr(0, pos);
                        std::cout << "read msg: " << msg << std::endl << std::endl;
                        buf.erase(0, pos + delimiter_.length());
                    }
                }
            } catch(...) {
                // on error stop thread
                break;
            }
        }
        server.disconnect();
    }
}