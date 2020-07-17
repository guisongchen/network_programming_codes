#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/time.h>
#include <sys/select.h>

const int BUFF_SIZE = 1024;

void error_handing(char* message);

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
    
    
    // create socket to handle client requests(like a guard)
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handing("socket() error");
    }
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));
    
    // bind and listen
    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handing("bind() error");
    }
    
    if (listen(serv_sock, 5) == -1) {
        error_handing("listen() error");
    }
    
    // using I/O multiplexing to relize multi-process server
    
    // set select function
    fd_set reads, temps;
    struct timeval timeout;
    int fd_max = serv_sock + 1;
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    
    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;
        
        int result = select(fd_max, &temps, 0, 0, &timeout);
        if (result == -1) {
            error_handing("select() error");
            break;
        } else if (result == 0) {
            continue;
        }
        
        // search form 0 to fd_max to identify changement
        for (int i = 0; i < fd_max; ++i) {
            if (FD_ISSET(i, &temps)) {
                if (i == serv_sock) {
                    socklen_t clnt_addr_size = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
                    
                    FD_SET(clnt_sock, &reads);
                    fd_max = fd_max < (clnt_sock+1) ? (clnt_sock+1) : fd_max;
                    std::cout << "connect client: " << clnt_sock << std::endl;
                } else {
                    int str_len = read(i, message, BUFF_SIZE);
                    if (str_len == 0) {
                        FD_CLR(i, &reads);
                        close(i);
                        std::cout << "close client: " << i << std::endl;
                    } else {
                        write(i, message, str_len);
                    }
                }
            }
        }
        
    }
    
    close(serv_sock);
    return 0;
}

void error_handing(char* message) {
    
    // this works for char array not for digit array(int, float, double..)
    std::cerr << message << std::endl;
}


