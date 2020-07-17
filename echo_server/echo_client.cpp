#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

const int BUFF_SIZE = 1024;
void error_handing(std::string mes);

int main(int argc, char** argv) {
    
    // std::cout << "this is a message from client..." << std::endl;
    
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP> <port>" << std::endl;
        exit(1);
    }
    
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUFF_SIZE];
    int str_len;
    
    // -----------create socket----------------------
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        error_handing("socket() error");
    }
    
    // init serv_addr as 0
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    // address sin_family
    serv_addr.sin_family = AF_INET;
    
    // sin_addr is a struct, s_addr is a type same as return value of inet_addr
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    // get port
    serv_addr.sin_port = htons(atoi(argv[2]));
    
    // ------------creat connect-----------------------
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handing("connect() error");
    } else {
        std::cout << "connect success..." << std::endl;
    }
    
    while (1) {
        
        std::cout << "input message, q to quit" << std::endl;
        std::cin.getline(message,BUFF_SIZE);
        
        // fputs("input message, q to quit:\n", stdout);
        // fgets(message, BUFF_SIZE, stdin);
        
        
        if (!strcmp(message, "q"))
            break;
        
        // use strlen to write exact length from stdin, NOT use BUFF_SIZE here
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUFF_SIZE-1);
        if (str_len == -1) {
            error_handing("read() error");
        }
        
        // message[str_len] = 0;
        std::string str = message;
        std::cout << "message from server: " << str << std::endl;
    }
    
    close(sock);
    return 0;
}

void error_handing(std::string str) {
    std::cerr << str << std::endl;
    exit(1);
}
