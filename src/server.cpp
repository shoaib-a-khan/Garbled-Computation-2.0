#include "../include/server.h"

Server::Server(std::string alias)
{
    name = alias;
}

int Server::Init(int portno)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        printf("ERROR opening server socket in Alice!\n");
    int enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
        printf("setsockopt(SO_REUSEADDR) failed");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        printf("ERROR on binding server socket in Alice!\n");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                       (struct sockaddr *)&cli_addr,
                       &clilen);
    if (newsockfd < 0)
    {
        printf("ERROR on accept at Alice's Server!\n");
        return 1;
    }

    return 0;
}

void Server::Send(const Message &msg)
{
    msg_for_client = msg;
    n = send(newsockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR in Server writing to Client's socket!\n");
}
const Message Server::Receive()
{
    Message msg;
    n = recv(newsockfd, &msg, sizeof(msg), 0);
    
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");
    else
        msg_from_client = msg;

    return msg;
}

void Server::SendProgramVector(std::vector<Instruction> V, int size)
{
    n = send(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in server!\n");

    for (int i = 0; i < size; i++)
    {
        n = send(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in server!\n");
    }
}

const std::vector<Instruction> Server::ReceiveProgramVector()
{
    int size;
    n = recv(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");
    
    std::vector<Instruction> V(size);

    for (int i = 0; i < size; i++)
    {
        n = recv(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Server reading from Client's socket!\n");
    }
    return V;
}

void Server::SendDataVector(std::vector<DataElement> V, int size)
{
    n = send(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in server!\n");

    for (int i = 0; i < size; i++)
    {
        n = send(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in server!\n");
    }
}

const std::vector<DataElement> Server::ReceiveDataVector()
{
    int size;
    n = recv(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");

    std::vector<DataElement> V(size);
    for (int i = 0; i < size; i++)
    {
        n = recv(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Server reading from Client's socket!\n");
    }
    return V;
}

void Server::Terminate()
{
    terminate_server = true;
    close(newsockfd);
    close(sockfd);
}

Server::~Server()
{
    Terminate();
}
