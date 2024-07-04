#include "client.hpp"

void TCP::Client::setDefault(){
    inet_pton(AF_INET, this->m_serverName.c_str(), &this->m_socketInfo.sin_addr);
}

TCP::Client::Client(): Socket(){
    setDefault();
}

TCP::Client::Client(int& t_port): Socket(t_port){
    setDefault();
}

TCP::Client::Client(std::string& t_serverName, const int& t_port): 
Socket(t_port), m_serverName(t_serverName){
    setDefault();
}

void TCP::Client::showMessage(const std::string& t_message){
    if(this->m_onMessage != nullptr){
        std::lock_guard<std::mutex> guard(this->m_mutex);
        this->m_onMessage(t_message);
    }
}

void TCP::Client::setOnMessage(const std::function<void(const std::string&)> &t_function){
    m_onMessage = t_function;
}

void TCP::Client::createConnection(){
    this->createSocket();
    int val = connect(this->m_socketFd, (sockaddr*)(&this->m_socketInfo), this->m_addressLen);
    check(val, CONNECT_ERROR, "Failed to connect to server");
}

void TCP::Client::writeMessage(){
    std::string message{};
    while(this->m_state == STATE::OPEN){
        std::getline(std::cin, message);
        this->sendMessage(this->m_socketFd, message);
    }
}

void TCP::Client::receiveMessage(){
    this->m_thread = std::move(std::thread([=]{
        int len{};
        char messageBuffer[this->getMessageSize()];
        
        while((len = recv(this->m_socketFd, messageBuffer, this->getMessageSize(), 0)) > 0){
            messageBuffer[len] = '\0';
            showMessage(messageBuffer);
            std::memset(messageBuffer, '\0', sizeof(messageBuffer));
        }
        this->m_state = STATE::CLOSED;
    }));
}

void TCP::Client::connectServer(){
    this->createConnection();
    showMessage("Connected Successfully.");
    this->receiveMessage();
    this->writeMessage();
}

void TCP::Client::closeConnection(){
    this->closeSocket();
    if(this->m_thread.joinable()) this->m_thread.detach(); // detach used to kill the thread
}

TCP::Client::~Client(){
    this->closeConnection();
}

