#include <iostream>
#include <unistd.h>

int main() {
    char message[] = "message writes by child";
    char answer[] = "message writes by parent.";
    char buf[30];
    
    pid_t pid;
    int fds[2];
    
    pipe(fds);
    pid = fork();
    
    if (pid == 0) {
        std::cout << "child process running.. " << std::endl;
        write(fds[1], message, sizeof(message));
        std::cout << "child process finish writing.. " << std::endl;
        
        sleep(2);
        read(fds[0], buf, 30);
        std::cout << "child proc output: " << buf << std::endl;
        
    } else {
        std::cout << "parent process running.. " << std::endl;
        read(fds[0], buf, 30);
        std::cout << "parent proc output: " << buf << std::endl;
        
        write(fds[1], answer, sizeof(answer));
        std::cout << "parent process finish writing.. " << std::endl;
        sleep(3);
    }
    
    return 0;
}
