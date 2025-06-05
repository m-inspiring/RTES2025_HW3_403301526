#include <iostream>
#include <fcntl.h>      
#include <sys/stat.h>   
#include <sys/mman.h>   
#include <unistd.h>     
#include <cerrno>       
#include <cstring>      
#include <string>       

#define SHM_SIZE 10
const char* SHM_NAME = "/my_shm";

int main() {
    const size_t meta_size  = sizeof(size_t) * 2;
    const size_t total_size = meta_size + SHM_SIZE;

    int fd = shm_open(SHM_NAME, O_RDONLY, 0); 
    if (fd < 0) {
        std::cerr << "shm_open: " << strerror(errno) << "\n";
        std::cerr << "Ensure the writer process has been run first to create the shared memory.\n";
        return 1;
    }

    void* addr = mmap(nullptr, total_size,
                      PROT_READ,    
                      MAP_SHARED,   
                      fd, 0);
    if (addr == MAP_FAILED) {
        std::cerr << "mmap: " << strerror(errno) << "\n";
        close(fd);
        return 1;
    }
    close(fd); 

    size_t* write_index_ptr = static_cast<size_t*>(addr);
    size_t* last_len_ptr    = static_cast<size_t*>(addr) + 1;
    char* buffer          = reinterpret_cast<char*>(addr) + meta_size;

    std::cout << "Reader started. Press ENTER to dump raw shared memory buffer.\n"
              << "Press Ctrl+D to quit.\n";

    while (std::cin.get() == '\n') {
        size_t current_write_index = *write_index_ptr;
        size_t current_last_len    = *last_len_ptr;
        
        std::cout << "Buffer: [";
        for (size_t i = 0; i < SHM_SIZE; ++i) {
            std::cout << buffer[i];
        }
        std::cout << "]  (last_len=" << current_last_len
                  << ", write_index=" << current_write_index << ")\n";
    }

    if (munmap(addr, total_size) == -1) {
        std::cerr << "munmap: " << strerror(errno) << "\n";
    }

    std::cout << "Reader finished.\n";
    return 0;
}