#ifndef CLIENT_H
#define CLIENT_H
#include "libraries.h"
#include "message.h"
#include "instruction.h"
#include "dataelement.h"

class Client
{
private:
    std::string name;
    bool terminate_client = false;
    bool send_to_server = false;
    bool rcv_from_server = false;
    bool client_rcv = false;
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256] = "";
    Message msg_for_server, msg_from_server;

public:
    Client(std::string alias = "Client");
    ~Client();
    void Init(char *ip, int portno);
    void Send(const Message& msg_for_client);
    const Message Receive();
    void SendProgramVector(const std::vector<Instruction> V, int size);
    const std::vector<Instruction> ReceiveProgramVector();
    void SendDataVector(const std::vector<DataElement> V, int size);
    const std::vector<DataElement> ReceiveDataVector();    
    void Terminate();

};

#endif