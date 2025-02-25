#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <fcntl.h>      // O_CREAT, O_RDWR
#include <sys/mman.h>   // shm_open, mmap
#include <unistd.h>     // ftruncate, close
#include <iostream>
#include <cstring>

#define SHM_NAME "/video_time_shm"  // Shared memory name
#define SHM_SIZE sizeof(int)        // Store only 1 int value

class SharedMemory {
public:
    SharedMemory();
    ~SharedMemory();
    void writeTime(int time); // Write the time value into Shared Memory
    int readTime();           // Read time value from Shared Memory

private:
    int shm_fd;
    int *shm_ptr;
};

#endif // SHAREDMEMORY_H
