#include "wifiserver.hpp"

rpisocket::WiFiServer::WiFiServer(int port)
{
    int opt = 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd <= 0)
        std::invalid_argument("error opening socket!\n");

    // Forcefully attaching socket to the port
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    this->port = port;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    //bind socket to the port
    if (bind(sockfd, (struct sockaddr *) &server, sizeof(server)) < 0){
        std::invalid_argument("error binding server\n");
    }
}

const std::vector<std::string>& rpisocket::WiFiServer::getBuffer() const
{
    std::vector<std::string> buf;

    return buf;
}

void rpisocket::WiFiServer::sendMsgToClient(sockaddr_in client, const std::string& msg) const
{

}
