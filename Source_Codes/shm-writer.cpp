#include <iostream>
#include <fcntl.h>      
#include <sys/stat.h>   
#include <sys/mman.h>   
#include <unistd.h>     
#include <cstring>      
#include <cerrno>       
#include <string>       
#include <algorithm>    

#define SHM_SIZE 10
const char* SHM_NAME = "/my_shm";

int main() {
    const size_t meta_size  = sizeof(size_t) * 2;
    const size_t total_size = meta_size + SHM_SIZE;
    bool created = false;

    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd >= 0) {
        created = true;
    } else if (errno == EEXIST) {
        fd = shm_open(SHM_NAME, O_RDWR, 0);
        if (fd < 0) {
            std::cerr << "shm_open existing: " << strerror(errno) << "\n";
            return 1;
        }
    } else {
        std::cerr << "shm_open create: " << strerror(errno) << "\n";
        return 1;
    }

    if (created) {
        if (ftruncate(fd, total_size) < 0) {
            std::cerr << "ftruncate: " << strerror(errno) << "\n";
            close(fd);
            shm_unlink(SHM_NAME);
            return 1;
        }
    }

    void* addr = mmap(nullptr, total_size,
                      PROT_READ | PROT_WRITE,
                      MAP_SHARED,
                      fd, 0);
    if (addr == MAP_FAILED) {
        std::cerr << "mmap: " << strerror(errno) << "\n";
        close(fd);
        if (created) shm_unlink(SHM_NAME);
        return 1;
    }
    close(fd);

    size_t* write_index_shm = static_cast<size_t*>(addr);
    size_t* last_len_shm    = static_cast<size_t*>(addr) + 1;
    char* buffer          = reinterpret_cast<char*>(addr) + meta_size;

    if (created) {
        *write_index_shm = 0;
        *last_len_shm    = 0;
        std::memset(buffer, ' ', SHM_SIZE);
    }

    std::cout << "Writer started. Type a line, press ENTER to write.\n"
              << "The circular buffer holds " << SHM_SIZE << " chars.\n"
              << "Press Ctrl+D or type an empty line then Enter to quit.\n";

    std::string line;
    while (std::getline(std::cin, line) && !line.empty()) {
        size_t current_input_len = line.size();

        for (size_t i = 0; i < current_input_len; ++i) {
            buffer[*write_index_shm] = line[i];
            *write_index_shm = (*write_index_shm + 1) % SHM_SIZE;
        }

        *last_len_shm = std::min(current_input_len, static_cast<size_t>(SHM_SIZE));
        
        std::cout << "[Wrote " << current_input_len 
                  << " chars. Next write pos (write_index)=" << *write_index_shm << "]\n";
    }

    if (munmap(addr, total_size) == -1) {
        std::cerr << "munmap: " << strerror(errno) << "\n";
    }
    
    std::cout << "Writer finished.\n";
    return 0;
}