#ifndef socket_hpp
#define socket_hpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string> 
#include <cstring>
#include <iostream>

#define SERVERPORT      8000
#define SOCKETERROR     -1
#define SENDERROR       -1


namespace TCP{

    enum class STATE {
		OPEN,
		CLOSED
	};

    class Socket{
    protected:
        uint16_t m_port           =    SERVERPORT;
        uint16_t m_messageSize    =    4096;
        uint16_t m_addressLen     =    0;
        int16_t  m_socketFd       =    -1;
        struct sockaddr_in m_socketInfo;
        STATE m_state;
        
        void setSocket();
        void createSocket();
        void sendMessage(const int& t_socketFd, const std::string& t_message);
        void closeSocket();

    public:
        Socket();
        Socket(const int& t_port);
       
        virtual ~Socket();

        // setters
        void setSocket(const sockaddr_in& t_socket);
        void setSocketFd(const int& t_socketFd);
        void setPort(const int& t_port);
        void setMessageSize(const int& t_messageSize);
        void setState(const STATE& t_state);
        
        // getters
        sockaddr_in getSocket() const;
        int16_t getSocketFd() const;
        uint16_t getPort() const;
        uint16_t getMessageSize() const;
        STATE getState() const;
        void check(const int& val,  const int& errorType, const std::string& errorMessage);
    };
}

#endif