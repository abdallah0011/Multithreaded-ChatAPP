#include <iostream>
#include <vector>
#include <limits>
#include <csignal>
#include <string>
#include "user.hpp"

UserFactory obj;
std::unique_ptr<TCP::Client> user_client = obj.createClient();

void signalHandler(int val){
    char c;
    std::cout << "You want to close client?(Y/N)\n";
    std::cin >> c;
    if((c == 'Y' || c == 'y') && user_client != nullptr){
        user_client = nullptr;
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
        user_client->setOnMessage(printMessage);
        user_client->connectServer();
    }catch(const std::exception& error){
        std::cout << error.what() << '\n';
    }

    return 0;
}