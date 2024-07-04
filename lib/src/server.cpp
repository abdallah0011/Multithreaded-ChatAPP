#include "server.hpp"


void TCP::Server::setDefault(){
    this->m_socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
}

TCP::Server::Server(): Socket(){
    setDefault();
}

TCP::Server::Server(const int& t_port): Socket(t_port){
    setDefault();
}

TCP::Server::Server(const int& t_port, const int& t_connectionSize): 
Socket(t_port), m_connectionSize(t_connectionSize){
    setDefault();
}

void TCP::Server::setOnMessage(const std::function<void(const std::string&)> &t_function){
        m_onMessage = t_function;
}

void TCP::Server::showMessage(const std::string& t_message){
    if(this->m_onMessage != nullptr){
        std::lock_guard<std::mutex> guard(this->m_mutex);
        this->m_onMessage(t_message);
    }
}

void TCP::Server::runServer(){
    this->createServer();
    this->bindServer();
    this->listenServer();
    this->handleRequest();
}

void TCP::Server::createServer(){
    const int opt = 1;
    this->createSocket();
    int val = setsockopt(this->m_socketFd, SOL_SOCKET, SO_REUSEADDR, (const char *) &opt, sizeof(opt));
    check(val, SOCKOPT_ERROR, "Setsockopt error");
}

void TCP::Server::bindServer(){
    int val = bind(this->m_socketFd, (sockaddr*)(&this->m_socketInfo), this->m_addressLen);
    check(val, BIND_ERROR, "Binding Failed");
}

void TCP::Server::listenServer(){
    int val = listen(this->m_socketFd, this->m_connectionSize);
    check(val, LISTEN_ERROR, "Listening Failed");
}

void TCP::Server::handleRequest(){ 
    showMessage("Server is running...");
    showMessage(std::string(inet_ntoa(this->m_socketInfo.sin_addr)) + ":" + std::to_string(ntohs(this->m_socketInfo.sin_port)));

    while(true){
        // create client_socket
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(this->m_socketFd, (sockaddr*)(&client_addr), &client_len);
        check(client_socket, ACCEPT_ERROR, "Acception Error" );

        // Server is full 
        if(this->m_nodeSize + 1 > this->m_connectionSize){
            sendMessage(client_socket, "Server is FULL");
            close(client_socket);
        }
        // else
        this->m_nodeSize++;

        // create client
        std::shared_ptr<TCP::Socket> client = std::make_shared<TCP::Socket>();
        client->setSocket(client_addr);
        client->setSocketFd(client_socket);
        client->setState(STATE::OPEN);

        std::string client_ip(inet_ntoa(client_addr.sin_addr));
        std::string client_port(std::to_string(ntohs(client_addr.sin_port)));

        // new client thread
        this->m_threads.push_back(std::thread([=]{
            int len{};
            char messageBuffer[this->getMessageSize()];
            while((len = recv(client->getSocketFd(), messageBuffer, this->getMessageSize(), 0)) > 0){
                messageBuffer[len] = '\0';
                std::string message(client_ip + ":" + client_port + "> " + messageBuffer);
                this->sendMessageAll(message, client->getSocketFd());
                std::memset(messageBuffer, '\0', sizeof(messageBuffer));
            }
            this->m_clients.erase(std::remove(this->m_clients.begin(), this->m_clients.end(), client));
            this->showMessage(client_ip + ":" + client_port + " disconnected");
        }));

        this->showMessage(client_ip + ":" + client_port + " connected");
        this->m_clients.push_back(client); 
    }
}

void TCP::Server::sendMessageClients(const std::string& t_message){
    for(auto &client: m_clients){
        sendMessage(client->getSocketFd(), t_message);
    }
}

void TCP::Server::sendMessageAll(const std::string& t_message, const int& t_socket){
    for(auto &client: m_clients){
        if(client->getSocketFd() != t_socket)
            sendMessage(client->getSocketFd(), t_message);
    }
}

void TCP::Server::joinThreads(){
    for(auto &thread: m_threads){
        if(thread.joinable())
            thread.detach();
    }
}

TCP::Server::~Server(){
    this->closeServer();
}

void TCP::Server::closeServer(){
    sendMessageClients("Server is closed.");
    this->closeSocket();
    this->joinThreads(); // this is might be a bug
    this->m_clients.clear();
    this->m_threads.clear();
}



