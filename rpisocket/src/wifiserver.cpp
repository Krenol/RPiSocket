#include "wifiserver.hpp"

rpisocket::WiFiServer::WiFiServer(int port)
{
    int opt = 1;
    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_ <= 0){
        std::invalid_argument("error opening socket!\n");
    }

    // Forcefully attaching socket to the port
    if (setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    this->port_ = port;
    server_.sin_family = AF_INET;
    server_.sin_addr.s_addr = INADDR_ANY;
    server_.sin_port = htons(port);

    //bind socket to the port
    if (bind(sock_, (struct sockaddr *) &server_, sizeof(server_)) < 0){
        std::invalid_argument("error binding server\n");
    }
}


rpisocket::WiFiServer::~WiFiServer() {
    //close the connection
    close(sock_);
    close(newsock_);
}

bool rpisocket::WiFiServer::connect() const {
    std::lock_guard<std::mutex> guard(mtx_);
    connected_ = false;
    listen(sock_ , 3);
     
    int c = sizeof(struct sockaddr_in);
    newsock_ = accept(sock_, (struct sockaddr *)&client_, (socklen_t*)&c);
    if (newsock_ < 0)
    {
        perror("accept failed");
    }
    connected_ = true;
}

bool rpisocket::WiFiServer::hasConnection() const {
    return connected_;
}

std::string rpisocket::WiFiServer::readBytes() const {
    if(!connected_){
        return NOTCONNECTED;
    }
    char buf[1024] = { 0 };
    std::lock_guard<std::mutex> guard(mtx_);
    int bytes_read = read(newsock_, buf, sizeof(buf));

    if(bytes_read > 0){
        return (std::string)buf;
    }
    if(bytes_read == -1){
        connected_ = false;
    }
    return NODATA;
}

int rpisocket::WiFiServer::writeBytes(const std::string& msg) const {
    int status = -1;
    if(!connected_){
        return status; 
    } 
    std::lock_guard<std::mutex> guard(mtx_);
    status = write(newsock_, msg.c_str(), msg.length());
    if(status == -1){
        connected_ = false;
    }
    return status;
}

std::string rpisocket::WiFiServer::getConnectedClient() const {
    return "";
}
