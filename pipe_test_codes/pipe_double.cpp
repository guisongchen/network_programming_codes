#include <iostream>
#include <unistd.h>

int main() {
    char message[] = "message writes by child";
    char answer[] = "message writes by parent.";
    char buf[30];
    
    pid_t pid;
    int fds1[2], fds2[2];
    
    pipe(fds1);
    pipe(fds2);
    pid = fork();
    
    if (pid == 0) {
        std::cout << "child process running.. " << std::endl;
        write(fds1[1], message, sizeof(message));
        std::cout << "child process finish writing to fds1.. " << std::endl;
        
        read(fds2[0], buf, 30);
        std::cout << "child proc output from fds2: " << buf << std::endl;
        
    } else {
        std::cout << "parent process running.. " << std::endl;
        read(fds1[0], buf, 30);
        std::cout << "parent proc output from fds1: " << buf << std::endl;
        
        write(fds2[1], answer, sizeof(answer));
        std::cout << "parent process finish writing to fds2.. " << std::endl;
        sleep(2);
    }
    
    return 0;
}
