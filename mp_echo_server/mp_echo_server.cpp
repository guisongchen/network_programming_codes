#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <wait.h>

const int BUFF_SIZE = 1024;

void error_handing(char* message);
void read_childproc(int sig);

int main(int argc, char** argv) {
    
    // std::cout << "this is a message from server..." << std::endl;
    
    int serv_sock;
    int clnt_sock;
    
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUFF_SIZE];
    
    // use signal to handle zambie process  
    pid_t pid;
    struct sigaction act;
    int state;
    
    if (argc != 2){
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        exit(1);
    }
    
    // config signal action
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);  // sa_mask and sa_flags have advanced usage, we simply set them 0 here
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);
    
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
    
    // use multi-process to create multi-server
    int client_cnt = 0;
    while (1) {
        
        // get actual length of client addr to keep addr from connect() in client
        clnt_addr_size = sizeof(clnt_addr);
        
        // clnt_sock here is the really socket of server create by accept()
        // which is used to communicate with client
        // of course the connect pass through serv_sock, the guard we created by socket()
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1) {
            error_handing("accept() error");
            continue;
        } else {
            std::cout << "connected with client " << ++client_cnt << std::endl;
        }
        
        // multi-process
        int pid, str_len;
        pid = fork();
        if (pid == -1) {
            close(clnt_sock);
            error_handing("fork() error");
            continue;
        } else if (pid == 0) {
            // child process
            
            // why we close serv_sock here?
            // serv_sock will be copied from parent process when execute fork()
            // now 2 fd point to the same socket, we need to close one of them(socket in child process)
            close(serv_sock);
            
            while ((str_len = read(clnt_sock, message, BUFF_SIZE)) > 0) {
                write(clnt_sock, message, str_len);
            }
            
            close(clnt_sock);
            std::cout << "client " << client_cnt << " disconnected..." << std::endl;
            return 0;
        } else {
            // parent process
            close(clnt_sock);
        }
        
    }
    
    close(serv_sock);
    return 0;
}

void error_handing(char* message) {
    
    // this works for char array not for digit array(int, float, double..)
    std::cerr << message << std::endl;
}

void read_childproc(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    std::cout << "remove process id: " << pid << std::endl;
}


