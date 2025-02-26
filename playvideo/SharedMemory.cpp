#include "SharedMemory.h"

SharedMemory::SharedMemory() {
    // Open or create a shared drive    
    //O_CREAT: If shared memory does not exist, create a new one.
    //O_RDWR: Open memory with read and write permissions.
    //0666: read and write
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    // Set shared memory size
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate failed");
        exit(1);
    }

    // Map shared memory to the process's memory area
    //0 Kernel chooses the mapping address itself
    // SHM_SIZE Shared memory size
    //PROT_READ | PROT_WRITE can be read and write
    //MAP_SHARED Other processes may see the change
    //shm_fd File descriptor of SHM
    //offset 0 Map from the beginning of shared memory
    shm_ptr = static_cast<int *>(mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
}

SharedMemory::~SharedMemory() {
    munmap(shm_ptr, SHM_SIZE); // Free the memory mapping
    close(shm_fd);             // Close shared drive
}

void SharedMemory::writeTime(int time) {
    *shm_ptr = time;  // Record time into Shared Memory
}

int SharedMemory::readTime() {
    return *shm_ptr;  // Read time from Shared Memory
}
