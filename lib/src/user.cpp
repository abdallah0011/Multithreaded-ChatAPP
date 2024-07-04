#include "user.hpp"

std::unique_ptr<TCP::Server> UserFactory::createServer(){
    int port{}, clientsSize{};
    std::cout << "Port: ";
    std::cin >> port;
    std::cout <<"Number of Clients: ";
    std::cin >> clientsSize;
    return std::make_unique<TCP::Server>(port, clientsSize); 
}

std::unique_ptr<TCP::Client> UserFactory::createClient(){
	std::string server = "localhost";
	std::cout << "Server: ";
	std::getline(std::cin, server);
	
	int port{};
    std::cout << "Port: ";
	std::cin >> port;
    return std::make_unique<TCP::Client>(server, port);
}