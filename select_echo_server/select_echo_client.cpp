#include <iostream>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

const int BUFF_SIZE = 1024;
void error_handing(char* message);
void write_process(int sock, char* message);
void read_process(int sock, char* message);

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
    
    
    memset(&serv_addr, 0, sizeof(serv_addr)); // init serv_addr as 0
    serv_addr.sin_family = AF_INET; // address sin_family  
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  // sin_addr is struct, s_addr is type name
    serv_addr.sin_port = htons(atoi(argv[2])); // get port
    
    // ------------creat connect-----------------------
    if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handing("connect() error");
    } else {
        std::cout << "connect success..." << std::endl;
    }
    
    // ------------i/o in same process-------------------------------
    /*
    while (1) {
        
        std::cout << "input message, q to quit" << std::endl;
        std::cin.getline(message,BUFF_SIZE);
        
        // fputs("input message, q to quit:\n", stdout);
        // fgets(message, BUFF_SIZE, stdin);
        
        
        if (!strcmp(message, "q"))
            break;
        
        // use strlen to write exact length from stdin, NOT use BUFF_SIZE here
        // since each char is i byte, sizeof and strlen may have same results
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUFF_SIZE-1);
        if (str_len == -1) {
            error_handing("read() error");
        }
        
        // message[str_len] = 0;
        std::string str = message;
        std::cout << "message from server: " << str << std::endl;
    }
    */
    
    
    // ----------i/o separate using multi-process-------------------
    int pid = fork();
    if (pid == 0) {
        write_process(sock, message);
    } else {
        read_process(sock, message);
    }
    
    close(sock);
    return 0;
}

void error_handing(char* message) {
    std::cerr << message << std::endl;
    exit(1);
}

void read_process(int sock, char* message) {
    std::cout << "read from server.." << std::endl;
    while (1) {
        int str_len = read(sock, message, BUFF_SIZE);
        if (str_len == 0)
            return;
        
        
        // set stop maker at indice str_len when output, since 0 works as '\0' here
        message[str_len] = 0;
        std::cout << "Message from server: " << message << std::endl;
    }
}

void write_process(int sock, char* message) {
    std::cout << "write to server, please input:" << std::endl;
    while (1) {
        std::cin.getline(message, BUFF_SIZE);
        if (!strcmp(message, "q")) {
            // write happends in child process, get a copy of sock from fork
            // need to shut down the copy in child process
            shutdown(sock, SHUT_WR);
            return;
        }
        
        write(sock, message, strlen(message));
    }
}


