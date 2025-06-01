#include "ipc/sharedMem.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <cassert>

namespace sharedMem {

std::string composeShmName(pid_t pid, requestId_t requestId) {
    std::string address = ("/" + std::to_string(pid) + std::to_string(requestId)).substr(0, MAX_STRING_SIZE);
    if (address.empty() || address[0] != '/') {
        throw std::invalid_argument("Input must start with '/'");
    }

    std::string output = "/";
    for (size_t i = 1; i < address.length(); ++i)
        output += static_cast<char>('a' + (address[i] - '0'));
    return output;
}

template<typename T>
SHMChannel<T>::SHMChannel(std::string name, bool create)
    : shm_name(std::move(name)), shm_fd(-1), buffer(nullptr), created(create)
{
    if (create) {
        shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
        if (shm_fd == -1) throw std::runtime_error("Failed to open shared memory");
        ftruncate(shm_fd, sizeof(SharedBuffer<T>));
    } else {
        shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
        if (shm_fd == -1) throw std::runtime_error("Failed to open shared memory");
    }

    void* ptr = mmap(nullptr, sizeof(SharedBuffer<T>),
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) throw std::runtime_error("mmap failed");

    buffer = static_cast<SharedBuffer<T>*>(ptr);

    if (create) {
        init_shared_buffer();
    }
}

template<typename T>
SHMChannel<T>::~SHMChannel() {
    if (!buffer)
        return;

    munmap(buffer, sizeof(SharedBuffer<T>));
    close(shm_fd);
    if (created) {
        shm_unlink(shm_name.c_str());
    }
}

template<typename T>
void SHMChannel<T>::init_shared_buffer() {
    buffer->head = 0;
    buffer->tail = 0;
    sem_init(&buffer->sem_data_available, 1, 0);
    sem_init(&buffer->sem_space_available, 1, MAX_MESSAGES);

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&buffer->mutex, &attr);
}

template<typename T>
void SHMChannel<T>::send(const T& message) {
    assert(buffer);
    sem_wait(&buffer->sem_space_available);

    pthread_mutex_lock(&buffer->mutex);

    size_t head = buffer->head;
    buffer->messages[head % MAX_MESSAGES] = message;
    buffer->head = head + 1;

#ifdef DEBUG
    std::cerr << "[SEND] head=" << buffer->head << " tail=" << buffer->tail << std::endl;
#endif

    pthread_mutex_unlock(&buffer->mutex);

    sem_post(&buffer->sem_data_available);
}

template<typename T>
bool SHMChannel<T>::receive(T& out_message, bool wait) {
    assert(buffer);

    if (wait) {
        sem_wait(&buffer->sem_data_available);
    } else {
        if (sem_trywait(&buffer->sem_data_available) == -1)
            return false;
    }

    pthread_mutex_lock(&buffer->mutex);

    size_t tail = buffer->tail;
    out_message = buffer->messages[tail % MAX_MESSAGES];
    buffer->tail = tail + 1;

#ifdef DEBUG
    std::cerr << "[RECV] head=" << buffer->head << " tail=" << buffer->tail << std::endl;
#endif

    pthread_mutex_unlock(&buffer->mutex);

    sem_post(&buffer->sem_space_available);

    return true;
}

}


template class sharedMem::SHMChannel<sharedMem::Response>;
template class sharedMem::SHMChannel<sharedMem::InputValue>;
template class sharedMem::SHMChannel<sharedMem::TraceMessage>;
