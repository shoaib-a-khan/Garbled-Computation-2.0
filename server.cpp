// Server side C/C++ program to demonstrate Socket programming
#include "server.h"

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

    // while (!terminate_server)
    // {
    //     if (send_to_client)
    //     {
    //         n = write(newsockfd, &msg_for_client, sizeof(msg_for_client));
    //         send_to_client = false;
    //         if (n < 0)
    //             printf("ERROR in Server writing to Client's socket!\n");
    //     }
    //     if (rcv_from_client)
    //     {
    //         n = read(newsockfd, &msg_from_client, sizeof(msg_from_client));
    //         if (n < 0)
    //             printf("ERROR in Server reading from Client's socket!\n");
    //         else
    //         {
    //             server_rcv = true;
    //             rcv_from_client = false;
    //         }
    //     }

    //     //sleep(1);
    // }
    // Message temp;
    // for(int i=0; i<10; i++)
    // {
    //     Receive();
    //     Send(temp);
    // }

    // char *hello = "Hello from server";
    // int valread = read(newsockfd, buffer, 256);
    // printf("%s\n", buffer);
    // send(newsockfd, hello, strlen(hello), 0);
    // printf("%s: Hello message sent\n", name.c_str());

    // close(newsockfd);
    // close(sockfd);

    return 0;
}

void Server::Send(const Message &msg)
{
    // static int count = 1;
    // std::string hello = "Hello from server\t";
    // hello.append(std::to_string(count));
    // send(newsockfd, hello.c_str(), strlen(hello.c_str()), 0);
    // printf("%s: Hello message sent\n", name.c_str());
    // ++count;

    msg_for_client = msg;
    n = send(newsockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR in Server writing to Client's socket!\n");
    // else
    //     printf("Server sent: scalar1 %ld scalar2 %ld\n", msg_for_client.scalar1, msg_for_client.scalar2);
}
const Message Server::Receive()
{
    // int valread = read(newsockfd, buffer, 256);
    // printf("%s rcvd: %s\n", name.c_str(), buffer);

    Message msg;
    n = recv(newsockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");
    else
    {
        msg_from_client = msg;
        // printf("Server rcvd: scalar1 %ld scalar2 %ld\n", msg_from_client.scalar1, msg_from_client.scalar2);
    }
    return msg;
}

void Server::SendProgramVector(std::vector<Instruction> V, int size)
{
    n = send(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in server!\n");
    // else
    //     printf("Server sent: size = %d\n", size);
    for (int i = 0; i < size; i++)
    {
        // Instruction temp = V[i];
        n = send(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in server!\n");
        // else
        //     printf("Server sent: V[%d] = %ld %ld %ld %ld %ld\n", i, 
        //     V[i].val[0], V[i].val[1],V[i].val[2],V[i].val[3], V[i].val[4] );
    }
}

const std::vector<Instruction> Server::ReceiveProgramVector()
{
    int size;
    n = recv(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");
    // else
    //     printf("Server rcvd: size %d \n", size);
    std::vector<Instruction> V(size);
    // Instruction temp;
    for (int i = 0; i < size; i++)
    {
        n = recv(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Server reading from Client's socket!\n");
        // else
        //     printf("Server rcvd: V[%d] = %ld %ld %ld %ld %ld\n", i,
        //            V[i].val[0], V[i].val[1], V[i].val[2], V[i].val[3], V[i].val[4]);
    }
    return V;
}

void Server::SendDataVector(std::vector<DataElement> V, int size)
{
    n = send(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in server!\n");
    // else
    //     printf("Server sent: size = %d\n", size);
    for (int i = 0; i < size; i++)
    {
        n = send(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in server!\n");
        // else
        //     printf("Server sent: V[%d] = %ld %ld\n", i, 
        //     V[i].val[0], V[i].val[1]);
    }
}

const std::vector<DataElement> Server::ReceiveDataVector()
{
    int size;
    n = recv(newsockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Server reading from Client's socket!\n");
    // else
    //     printf("Server rcvd: size %d \n", size);
    std::vector<DataElement> V(size);
    for (int i = 0; i < size; i++)
    {
        n = recv(newsockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Server reading from Client's socket!\n");
        // else
        //     printf("Server rcvd: V[%d] = %ld %ld\n", i,
        //            V[i].val[0], V[i].val[1]);
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
