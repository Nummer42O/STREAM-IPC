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
    for (size_t i = 1; i < address.length(); ++i) output += static_cast<char>('a' + (address[i] - '0'));

    return output;
}


template<typename T>
SHMChannel<T>::SHMChannel(std::string name, bool create)
    : shm_name(std::move(name)), shm_fd(-1), buffer(nullptr), created(create)
{
    if (create) {
        shm_fd = shm_open(shm_name.c_str(), O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(SharedBuffer<T>));
    } else {
        shm_fd = shm_open(shm_name.c_str(), O_RDWR, 0666);
    }

    if (shm_fd == -1)
        throw std::runtime_error("Failed to open shared memory");

    void* ptr = mmap(nullptr, sizeof(SharedBuffer<T>),
                     PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    buffer = static_cast<SharedBuffer<T>*>(ptr);

    if (create) {
        init_shared_buffer();
    }
}

template<typename T>
SHMChannel<T>::SHMChannel(SHMChannel &&other)
{
  if (!other.buffer)
  {
    this->buffer = nullptr;
    return;
  }

  this->shm_name = std::move(other.shm_name);
  this->shm_fd = other.shm_fd;
  this->buffer = other.buffer;
  this->created = other.created;

  //! NOTE: Workaround so the destructor does not unmap the memory
  other.buffer = nullptr;
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
void SHMChannel<T>::send(const T& message) {
    assert(buffer);

    sem_wait(&buffer->sem_space_available);
    pthread_mutex_lock(&buffer->mutex);
    buffer->messages[buffer->head % MAX_MESSAGES] = message;
    buffer->head++;

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
    out_message = buffer->messages[buffer->tail % MAX_MESSAGES];
    buffer->tail++;

    #ifdef DEBUG
        std::cerr << "[RECV] head=" << buffer->head << " tail=" << buffer->tail << std::endl;
    #endif

    pthread_mutex_unlock(&buffer->mutex);
    sem_post(&buffer->sem_space_available);
    return true;
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

}

template class sharedMem::SHMChannel<sharedMem::Response>;
template class sharedMem::SHMChannel<sharedMem::InputValue>;
template class sharedMem::SHMChannel<sharedMem::TraceMessage>;