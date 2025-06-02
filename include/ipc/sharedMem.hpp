#pragma once

#include "ipc/common.hpp"
#include "ipc/datastructs/sharedMem-datastructs.hpp"

#include <semaphore.h>
#include <pthread.h>
#include <chrono>
#include <cstddef>
#include <string>
#include <array>
#include <iostream>

#define MAX_MESSAGES 128
// #define DEBUG

namespace sharedMem {

std::string composeShmName(pid_t pid, requestId_t requestID);

template<typename T>
struct SharedBuffer {
    std::array<T, MAX_MESSAGES> messages;
    size_t head;
    size_t tail;
    sem_t sem_data_available;
    sem_t sem_space_available;
    pthread_mutex_t mutex;
};

template<typename T>
class SHMChannel {
public:
    inline SHMChannel(): buffer(nullptr) {}
    SHMChannel(std::string name, bool create = false);
    ~SHMChannel();

    void send(const T& message);
    bool receive(T& out_message, bool wait = true);

private:
    void init_shared_buffer();

    std::string shm_name;
    int shm_fd;
    SharedBuffer<T>* buffer;
    bool created;
};

}
