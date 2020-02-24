#include "client.h"

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

    // while(!terminate_client)
    // {
    // 	if(send_to_server)
    // 	{
    // 		n = write(sockfd,&msg_for_server, sizeof(msg_for_server));
    // 		send_to_server = false;
    // 		if (n < 0)
    // 			printf("ERROR writing to client socket in Alice!\n");
    // 	}
    // 	if(rcv_from_server)
    // 	{
    // 		n = read(sockfd, &msg_from_server, sizeof(msg_from_server));
    // 		if (n < 0)
    // 			printf("ERROR reading from client socket in Alice!\n");
    // 		else{

    // 			client_rcv = true;
    // 			rcv_from_server = false;
    // 		}
    // 	}
    // }
    // Message temp;
    // for(int i=0; i<10; i++)
    // {
    //     Send(temp);
    //     Receive();
    // }
    // char *hello = "Hello from client";
    // send(sockfd , hello , strlen(hello) , 0 );
    // printf("%s: Hello message sent\n", name.c_str());
    // int valread = read( sockfd , buffer, 256);
    // printf("%s\n",buffer );

    // close(sockfd);
}

void Client::Send(const Message &msg)
{
    // static int count = 1;
    // std::string hello = "Hello from client\t";
    // hello.append(std::to_string(count));
    // send(sockfd, hello.c_str(), strlen(hello.c_str()), 0);
    // printf("%s: Hello message sent\n", name.c_str());
    // ++count;
    msg_for_server = msg;
    n = send(sockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
    // else
    //     printf("Client sent: scalar1 %ld scalar2 %ld\n", msg_for_server.scalar1, msg_for_server.scalar2);
}

const Message Client::Receive()
{
    // int valread = read(sockfd, buffer, 256);
    // printf("%s recvd: %s\n",name.c_str(), buffer);

    Message msg;
    n = recv(sockfd, &msg, sizeof(msg), 0);
    if (n < 0)
        printf("ERROR reading from client socket in Alice!\n");
    else
    {
        msg_from_server = msg;
        // printf("Client rcvd: scalar1 %ld scalar2 %ld\n", msg_from_server.scalar1, msg_from_server.scalar2);
    }
    return msg;
}

void Client::SendProgramVector(std::vector<Instruction> V, int size)
{
    n = send(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
    // else
    //     printf("Client sent: size = %d\n", size);
    for (int i = 0; i < size; i++)
    {
        // Instruction temp = V[i];
        n = send(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in client!\n");
        // else
        //     printf("Client sent: V[%d] = %ld %ld %ld %ld %ld\n", i, 
        //     V[i].val[0], V[i].val[1],V[i].val[2],V[i].val[3], V[i].val[4] );
    }
}

const std::vector<Instruction> Client::ReceiveProgramVector()
{
    int size;
    n = recv(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Client reading from socket!\n");
    // else
    //     printf("Client rcvd: size %d \n", size);
    std::vector<Instruction> V(size);
    // Instruction temp;
    for (int i = 0; i < size; i++)
    {
        n = recv(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Client reading from socket!\n");
        // else
        //     printf("Client rcvd: V[%d] = %ld %ld %ld %ld %ld\n", i,
        //            V[i].val[0], V[i].val[1], V[i].val[2], V[i].val[3], V[i].val[4]);
    }
    return V;
}

void Client::SendDataVector(std::vector<DataElement> V, int size)
{
    n = send(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR writing to socket in client!\n");
    // else
    //     printf("Client sent: size = %d\n", size);
    for (int i = 0; i < size; i++)
    {
        n = send(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR writing to socket in client!\n");
        // else
        //     printf("Client sent: V[%d] = %ld %ld\n", i, 
        //     V[i].val[0], V[i].val[1]);
    }
}

const std::vector<DataElement> Client::ReceiveDataVector()
{
    int size;
    n = recv(sockfd, &size, sizeof(size), 0);
    if (n < 0)
        printf("ERROR in Client reading from socket!\n");
    // else
    //     printf("Client rcvd: size %d \n", size);
    std::vector<DataElement> V(size);
    for (int i = 0; i < size; i++)
    {
        n = recv(sockfd, &V[i], sizeof(V[i]), 0);
        if (n < 0)
            printf("ERROR in Client reading from socket!\n");
        // else
        //     printf("Client rcvd: V[%d] = %ld %ld\n", i,
        //            V[i].val[0], V[i].val[1]);
    }
    return V;
}


Client::~Client()
{
    close(sockfd);
}