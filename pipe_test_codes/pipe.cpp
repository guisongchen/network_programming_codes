#include <iostream>
#include <unistd.h>

int main() {
    char message[30];
    std::cout << "please input: " << std::endl;
    std::cin.getline(message, 30);
    char buf[30];
    
    pid_t pid;
    int fds[2];
    
    pipe(fds);
    pid = fork();
    
    if (pid == 0) {
        std::cout << "child process running.. " << std::endl;
        write(fds[1], message, sizeof(message));
        std::cout << "child process finish writing.. " << std::endl;
    } else {
        std::cout << "parent process running.. " << std::endl;
        read(fds[0], buf, 30);
        std::cout << "parent process finish reading " << str_len << std::endl;
        std::cout << buf << std::endl;
    }
    
    return 0;
}
