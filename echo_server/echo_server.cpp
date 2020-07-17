#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

const int BUFF_SIZE = 1024;

void error_handing(std::string str);

int main(int argc, char** argv) {
    
    // std::cout << "this is a message from server..." << std::endl;
    
    int serv_sock;
    int clnt_sock;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    
    char message[BUFF_SIZE];
    
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handing("socket() error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handing("bind() error");
    }
    
    if (listen(serv_sock, 5) == -1) {
        error_handing("listen() error");
    }
    
    clnt_addr_size = sizeof(clnt_addr);
    ssize_t str_len = 0;
    for (int i = 0; i < 5; ++i) {
        
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            error_handing("accept() error");
        } else {
            std::cout << "connect client " << i+1 << std::endl;
        }
        
        // read from client and write back to client
        
        while ((str_len = read(clnt_sock, message, BUFF_SIZE)) != 0) {
            write(clnt_sock, message, str_len);
        }
        
        close(clnt_sock);
    }
    close(serv_sock);
    
    return 0;
}

void error_handing(std::string str) {
    std::cerr << str << std::endl;
    exit(1);
}


