#ifndef SERVER_H
#define SERVER_H
#include "libraries.h"
#include "message.h"
#include "instruction.h"
#include "dataelement.h"

class Server
{
private:
    std::string name;
    bool terminate_server = false;
    bool send_to_client = false;
    bool rcv_from_client = false;
    bool server_rcv = false;
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[256] = "";
    ssize_t r;
    int n;
    struct sockaddr_in serv_addr, cli_addr;
    Message msg_for_client, msg_from_client;

public:
    Server(std::string alias = "Server");
    ~Server();
    int Init(int portno);
    void Send(const Message& msg_for_client);
    const Message Receive();
    void SendProgramVector(const std::vector<Instruction> V, int size);
    const std::vector<Instruction> ReceiveProgramVector();
    void SendDataVector(const std::vector<DataElement> V, int size);
    const std::vector<DataElement> ReceiveDataVector();
    void Terminate();

};

#endif