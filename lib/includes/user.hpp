#include "socket.hpp"
#include "client.hpp"
#include "server.hpp"


class UserFactory{
public:
    UserFactory() = default;
    ~UserFactory() = default;
    std::unique_ptr<TCP::Server> createServer();
    std::unique_ptr<TCP::Client> createClient();
};