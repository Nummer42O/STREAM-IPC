#include "ipc/sharedMem.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>


namespace sharedMem {

SHMChannel::SHMChannel(const char* name, bool create)
    : shm_name(name), shm_fd(-1), buffer(nullptr), created(create)
{
    if (create) {
        shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(SharedBuffer));
    } else {
        shm_fd = shm_open(shm_name, O_RDWR, 0666);
    }

    if (shm_fd == -1)
        throw std::runtime_error("Failed to open shared memory");

    void* ptr = mmap(nullptr, sizeof(SharedBuffer),
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    buffer = static_cast<SharedBuffer*>(ptr);

    if (create) {
        init_shared_buffer();
    }
}

SHMChannel::~SHMChannel() {
    munmap(buffer, sizeof(SharedBuffer));
    close(shm_fd);
    if (created) {
        shm_unlink(shm_name);
    }
}

void SHMChannel::send(const Value& message) {
    sem_wait(&buffer->sem_space_available);
    pthread_mutex_lock(&buffer->mutex);
    buffer->messages[buffer->head % MAX_MESSAGES] = message;
    buffer->head++;
    pthread_mutex_unlock(&buffer->mutex);
    sem_post(&buffer->sem_data_available);
}

bool SHMChannel::receive(Value& out_message) {
    if (sem_trywait(&buffer->sem_data_available) == -1)
        return false;

    pthread_mutex_lock(&buffer->mutex);
    out_message = buffer->messages[buffer->tail % MAX_MESSAGES];
    buffer->tail++;
    pthread_mutex_unlock(&buffer->mutex);
    sem_post(&buffer->sem_space_available);
    return true;
}

void SHMChannel::init_shared_buffer() {
    buffer->head = 0;
    buffer->tail = 0;
    sem_init(&buffer->sem_data_available, 1, 0);
    sem_init(&buffer->sem_space_available, 1, MAX_MESSAGES);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&buffer->mutex, &attr);
}

}