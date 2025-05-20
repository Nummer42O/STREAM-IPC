#pragma once

#include "ipc/common.hpp"

#include <semaphore.h>
#include <pthread.h>
#include <chrono>
#include <cstddef>

#define MAX_MESSAGES 128

namespace sharedMem {

enum class MessageType {
    CPU, MEMORY, DISK
};

struct Value {
    std::chrono::nanoseconds    timestamp;
    primaryKey_t                primaryKey;
    long double                 value;
    MessageType                 type;
};

struct SharedBuffer {
    Value messages[MAX_MESSAGES];
    size_t head;
    size_t tail;
    sem_t sem_data_available;
    sem_t sem_space_available;
    pthread_mutex_t mutex;
};

class SHMChannel {
public:
    SHMChannel(const char* name, bool create = false);
    ~SHMChannel();

    void send(const Value& message);
    bool receive(Value& out_message);

private:
    void init_shared_buffer();

    const char* shm_name;
    int shm_fd;
    SharedBuffer* buffer;
    bool created;
};

}
