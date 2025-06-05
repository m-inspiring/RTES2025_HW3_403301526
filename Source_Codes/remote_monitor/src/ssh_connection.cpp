#include "ssh_connection.h"
#include <iostream>

SSHConnection::SSHConnection(const std::string& host, const std::string& user, const std::string& password)
    : host(host), user(user), password(password), session(nullptr) {}

SSHConnection::~SSHConnection() {
    if (session) {
        ssh_disconnect(session);
        ssh_free(session);
    }
}

bool SSHConnection::connect() {
    session = ssh_new();
    if (!session) {
        std::cerr << "Error: Failed to create SSH session." << std::endl;
        return false;
    }

    ssh_options_set(session, SSH_OPTIONS_HOST, host.c_str());
    ssh_options_set(session, SSH_OPTIONS_USER, user.c_str());

    int rc = ssh_connect(session);
    if (rc != SSH_OK) {
        std::cerr << "Error connecting to host: " << ssh_get_error(session) << std::endl;
        ssh_free(session);
        session = nullptr;
        return false;
    }

    rc = ssh_userauth_password(session, nullptr, password.c_str());
    if (rc != SSH_AUTH_SUCCESS) {
        std::cerr << "Error authenticating: " << ssh_get_error(session) << std::endl;
        ssh_disconnect(session);
        ssh_free(session);
        session = nullptr;
        return false;
    }

    std::cout << "SSH connection established and authenticated successfully!" << std::endl;
    return true;
}

std::string SSHConnection::executeCommand(const std::string& command) {
    ssh_channel channel = ssh_channel_new(session);
    if (!channel) {
        std::cerr << "Error: Failed to create SSH channel." << std::endl;
        return "";
    }

    int rc = ssh_channel_open_session(channel);
    if (rc != SSH_OK) {
        std::cerr << "Error opening channel: " << ssh_get_error(session) << std::endl;
        ssh_channel_free(channel);
        return "";
    }

    rc = ssh_channel_request_exec(channel, command.c_str());
    if (rc != SSH_OK) {
        std::cerr << "Error executing command: " << ssh_get_error(session) << std::endl;
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        return "";
    }

    char buffer[256];
    std::string result;
    int nbytes;
    while ((nbytes = ssh_channel_read(channel, buffer, sizeof(buffer), 0)) > 0) {
        result.append(buffer, nbytes);
    }

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    return result;
}
