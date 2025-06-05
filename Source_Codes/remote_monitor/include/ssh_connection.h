#ifndef SSH_CONNECTION_H
#define SSH_CONNECTION_H

#include <libssh/libssh.h>
#include <string>

class SSHConnection {
public:
    SSHConnection(const std::string& host, const std::string& user, const std::string& password);
    ~SSHConnection();
    bool connect();
    std::string executeCommand(const std::string& command);

private:
    std::string host;
    std::string user;
    std::string password;
    ssh_session session;
};

#endif // SSH_CONNECTION_H
