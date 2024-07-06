#ifndef server_hpp
#define server_hpp

#include "socket.hpp"
#include <vector>
#include <mutex>
#include <thread>
#include <algorithm>
#include <memory>

#define SOCKOPT_ERROR   -1
#define BIND_ERROR      -1
#define LISTEN_ERROR    -1
#define CONNECT_ERROR   -1
#define ACCEPT_ERROR   -1
#define MAXCLIENTS 128

namespace TCP{

    class Server: protected virtual Socket{
    protected:
        // Server
        uint16_t m_connectionSize = MAXCLIENTS;
        void createServer();
        void bindServer();
        void listenServer();
        void handleRequest();

        // Client
        int m_nodeSize = 0;
        std::vector<std::shared_ptr<Socket>> m_clients;

        // Message event function
        std::function<void(const std::string& t_message)> m_onMessage = nullptr;
        void showMessage(const std::string& t_message);
        
        // Message
        void sendMessageClients(const std::string& t_message);
        void sendMessageAll(const std::string& t_message, const int& t_socket);

        // thread
        std::vector<std::thread> m_threads;
        std::mutex m_mutex;
        void joinThreads();

        // set ipAddress
        void setDefault();

    public:
        Server();
        Server(const int& t_port);
        Server(const int& t_port, const int& t_connectionSize);
      
        virtual ~Server();
      
        void setOnMessage(const std::function<void(const std::string&)> &t_function);
        void runServer();
        void closeServer();
    };
}


#endif