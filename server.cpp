#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int client,server;
    int portNum = 1500;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    
    struct sockaddr_in server_addr;
    socklen_t size;
    
    client = socket(AF_INET,SOCK_STREAM,0);
    
    if(client < 0){
        std::cout << "\nError establishing socket..." << std::endl;
        exit(1);
    }
    std::cout << "\n=> Socket server has been created..." << std::endl;
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNum);
    
    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0){
        std::cout << "=> Error binding connection, the socket has already been established..." << std::endl;
        return -1;
    }
    
    size = sizeof(server_addr);
    std::cout << "=> Looking for clients" << std::endl;
    
    listen(client, 1);
    
    int clientCount = 1;
    server = accept(client, (struct sockaddr *)&server_addr, &size);
    
    if(server<0){
        std::cout << "=>Error on accepting..." << std::endl;
    }
    
    while (server > 0)
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        std::cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << std::endl;
        std::cout << "\n=> Enter # to end the connection\n" << std::endl;
        

        
        std::cout << "Client: ";
        do {
            recv(server, buffer, bufsize, 0);
            std::cout << buffer << " ";
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');
        
        do {
            std::cout << "\nServer: ";
            do {
                std::cin >> buffer;
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
            
            std::cout << "Client: ";
            do {
                recv(server, buffer, bufsize, 0);
                std::cout << buffer << " ";
                if (*buffer == '#') {
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);
        
       
        
       
        std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        std::cout << "\nGoodbye..." << std::endl;
        isExit = false;
        exit(1);
    }
}
