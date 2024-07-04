#include "socket.hpp"

void TCP::Socket::setSocket(){
    this->m_addressLen = sizeof(this->m_socketInfo);
    std::memset(&this->m_socketInfo, 0, this->m_addressLen);
    this->m_socketInfo.sin_family = AF_INET;
    this->m_socketInfo.sin_port = htons(this->m_port);
}


TCP::Socket::Socket(){
    setSocket();
}

TCP::Socket::Socket(const int& t_port): m_port(t_port){
    setSocket();
}

void TCP::Socket::createSocket(){
    this->m_socketFd = socket(AF_INET, SOCK_STREAM, 0);
    check(m_socketFd, SOCKETERROR, "Failed to create socket");
    this->m_state = STATE::OPEN;
}

void TCP::Socket::sendMessage(const int& t_socketFd, const std::string &t_message){
    int valid = send(t_socketFd, t_message.c_str(), t_message.size(), 0);
    check(valid, SENDERROR, "Message failed to send");
}

void TCP::Socket::check(const int& val,  const int& errorType, const std::string& errorMessage){
    if(m_socketFd == errorType){
        throw std::runtime_error(errorMessage);
    }
}

void TCP::Socket::closeSocket(){
    if(this->m_state == STATE::CLOSED) return;
    this->m_state = STATE::CLOSED;
    close(this->m_socketFd);
}

TCP::Socket::~Socket(){
    closeSocket();
}


void TCP::Socket::setSocket(const sockaddr_in& t_socket){
    m_socketInfo = t_socket;
}
void TCP::Socket::setSocketFd(const int& t_socketFd){
    m_socketFd = t_socketFd;
}

void TCP::Socket::setPort(const int& t_port){
    m_port = t_port;
    setSocket();
}

void TCP::Socket::setMessageSize(const int& t_messageSize){
    m_messageSize = t_messageSize;
}

void TCP::Socket::setState(const STATE& t_state){
    m_state = t_state;
}

sockaddr_in TCP::Socket::getSocket() const{
    return m_socketInfo;
}

int16_t TCP::Socket::getSocketFd() const{
    return m_socketFd;
}

uint16_t TCP::Socket::getPort() const{
    return m_port;
}

uint16_t TCP::Socket::getMessageSize() const{
    return m_messageSize;
}

TCP::STATE TCP::Socket::getState() const{
    return m_state;
}
