#ifndef client_hpp
#define client_hpp

#include "socket.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

#define CONNECT_ERROR -1


namespace TCP{

    class Client: protected Socket{
        protected:
            // Client
            void createConnection();
            void writeMessage();
            void receiveMessage();

            // server
            std::string m_serverName = "localhost";

            // thread
            std::thread m_thread;
            std::mutex m_mutex;

            // Message event function
            std::function<void(const std::string& t_message)> m_onMessage = nullptr;
            void showMessage(const std::string& t_message);

            void setDefault();


        public:
            Client();
            Client(int& t_port);
            Client(std::string& t_serverName, const int& t_port);
            
            virtual ~Client();

            void setOnMessage(const std::function<void(const std::string&)> &t_function);

            void connectServer();
            void closeConnection();

    };

}
#endif