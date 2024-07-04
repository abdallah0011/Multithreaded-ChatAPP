#include <iostream>
#include <vector>
#include <limits>
#include <csignal>
#include <string>
#include "user.hpp"

UserFactory obj;
std::unique_ptr<TCP::Server> user_server = obj.createServer();

void signalHandler(int val){
    char c;
    std::cout << "You want to close server?(Y/N)\n";
    std::cin >> c;
    if((c == 'Y' || c == 'y') && user_server != nullptr){
        exit(0);
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void printMessage(const std::string& message){
    std::cout << message << '\n';
}

int main(){
    signal(SIGINT, signalHandler);
    try{
        user_server->setOnMessage(printMessage);
        user_server->runServer();
    }catch(const std::exception& error){
        std::cout << error.what() << '\n';
    }

    return 0;
}