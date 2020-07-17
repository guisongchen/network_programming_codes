#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <string.h>

const int BUFF_SIZE = 30;

int main() {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUFF_SIZE];
    struct timeval timeout;
    
    FD_ZERO(&reads);  // set all bits zero
    FD_SET(0, &reads); // set 0 bit to 1, where 0 is stdin
    
    while(1) {

        temps = reads; // backup reads before select change it
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        std::cout << "please input, or will time-out after 5 seconds.." << std::endl;
        result = select(1, &temps, 0, 0, &timeout);
        
        if (result == -1) {
            std::cerr << "select() error.." << std::endl;
            break;
        } else if (result == 0) {
            std::cout << "time-out!" << std::endl;
        } else {
            if (FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUFF_SIZE); // 0 is fd of stdin
                
                if (!strcmp(buf, "q\n"))
                    break;
                
                buf[str_len] = 0; // set stop maker
                std::cout << "message from console: " << buf;
            }
        }
        
    }
    
    return 0;
}