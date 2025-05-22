#include "ipc/sharedMem.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <iostream>


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

template<>
void printResponse<NumericalResponse>(const Response& response) {
    if (response.header.type != ResponseType::NUMERICAL) {
        std::cerr << "Error: Response is not of type NUMERICAL\n";
        return;
    }

    const NumericalResponse& nr = response.numerical;
    std::cout << "NumericalResponse:\tNumber: " << nr.number << "  Total: " << nr.total << "  Value: " << nr.value << std::endl;
}

template<>
void printResponse<TextualResponse>(const Response& response) {
    if (response.header.type != ResponseType::TEXTUAL) {
        std::cerr << "Error: Response is not of type TEXTUAL\n";
        return;
    }

    const TextualResponse& tr = response.textual;
    std::cout << "TextualResponse:\tNumber: " << tr.number << "  Total: " << tr.total << "  Value: " << tr.line << std::endl;
}


template<typename T>
SHMChannel<T>::SHMChannel(const char* name, bool create)
    : shm_name(name), shm_fd(-1), buffer(nullptr), created(create)
{
    if (create) {
        shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(SharedBuffer<T>));
    } else {
        shm_fd = shm_open(shm_name, O_RDWR, 0666);
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
SHMChannel<T>::~SHMChannel() {
    munmap(buffer, sizeof(SharedBuffer<T>));
    close(shm_fd);
    if (created) {
        shm_unlink(shm_name);
    }
}

template<typename T>
void SHMChannel<T>::send(const T& message) {
    sem_wait(&buffer->sem_space_available);
    pthread_mutex_lock(&buffer->mutex);
    buffer->messages[buffer->head % MAX_MESSAGES] = message;
    buffer->head++;
    pthread_mutex_unlock(&buffer->mutex);
    sem_post(&buffer->sem_data_available);
}

template<typename T>
bool SHMChannel<T>::receive(T& out_message, bool wait) {
    if (wait) {
        sem_wait(&buffer->sem_data_available);
    } else {
        if (sem_trywait(&buffer->sem_data_available) == -1)
            return false;
    }

    pthread_mutex_lock(&buffer->mutex);
    out_message = buffer->messages[buffer->tail % MAX_MESSAGES];
    buffer->tail++;
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