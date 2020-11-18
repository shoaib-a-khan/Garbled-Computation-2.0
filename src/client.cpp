#include "../include/client.h"

Client::Client(std::string alias)
{
    name = alias;
}

void Client::Init(char *ip, int portno)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening client socket in %s!\n", name.c_str());
    server = gethostbyname(ip);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host(Server is not live)!\n");
        //exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        printf("ERROR connecting %s to Server!\n", name.c_str());
    
}

void Client::Send(const Message &msg)
{
    msg_for_server = msg;
    n = send(sockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
}

const Message Client::Receive()
{
    Message msg;
    n = recv(sockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR reading from client socket in Alice!\n");
    else
        msg_from_server = msg;
  
    return msg;
}

void Client::SendProgramVector(std::vector<Instruction> V, int size)
{
    n = send(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
  
    for (int i = 0; i < size; i++)
    {
        n = send(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in client!\n");
    }
}

const std::vector<Instruction> Client::ReceiveProgramVector()
{
    int size;
    n = recv(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Client reading from socket!\n");
    std::vector<Instruction> V(size);
    for (int i = 0; i < size; i++)
    {
        n = recv(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Client reading from socket!\n");
    }
    return V;
}

void Client::SendDataVector(std::vector<DataElement> V, int size)
{
    n = send(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
   
    for (int i = 0; i < size; i++)
    {
        n = send(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in client!\n");
    }
}

const std::vector<DataElement> Client::ReceiveDataVector()
{
    int size;
    n = recv(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Client reading from socket!\n");
 
    std::vector<DataElement> V(size);
    for (int i = 0; i < size; i++)
    {
        n = recv(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Client reading from socket!\n");
    }
    return V;
}


Client::~Client()
{
    close(sockfd);
}