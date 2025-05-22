#pragma once

#include "ipc/common.hpp"

#include <semaphore.h>
#include <pthread.h>
#include <chrono>
#include <cstddef>
#include <string>
#include <array>
#include <iostream>

#define MAX_MESSAGES 128


namespace sharedMem {

enum class MessageType {
    CPU, MEMORY, DISK
};

struct InputValue {
    std::chrono::nanoseconds timestamp;
    primaryKey_t primaryKey;
    long double value;
    MessageType type;
};

enum ResponseType {
    NUMERICAL,
    TEXTUAL,
};

struct Header {
    ResponseType    type;
    size_t          payloadSize;
};

struct NumericalResponse {
    size_t number;
    size_t total;
    double value;
};

struct TextualResponse {
    size_t number;
    size_t total;
    MAKE_STRING(line);
};

union Union_Response {
    NumericalResponse   numerical;
    TextualResponse     textual;
};

struct Response {
    Header          header;
    Union_Response  payload;
};


std::string parseShmName(pid_t pid, requestId_t requestID);

template<typename T>
void printResponse(const Response& response) {
    std::cerr << "No specialization of printResponse for the requested type.\n";
}
template<>
void printResponse<NumericalResponse>(const Response& response);
template<>
void printResponse<TextualResponse>(const Response& response);


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
    SHMChannel(const char* name, bool create = false);
    ~SHMChannel();

    void send(const T& message);
    bool receive(T& out_message);

private:
    void init_shared_buffer();

    const char* shm_name;
    int shm_fd;
    SharedBuffer<T>* buffer;
    bool created;
};

}
