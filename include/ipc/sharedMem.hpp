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
    NONE,
    CPU,
    MEMORY,
    DISK,
    NODETRACE,
    PUBLISHERTRACE,
    SUBSCRIBERTRACE,
    SERVICETRACE,
    CLIENTTRACE,
    TIMERTRACE,
    STATEMACHINEINITTRACE,
    STATETRANSITIONTRACE
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

struct ResponseHeader {
    ResponseType    type;
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

struct Response {
    ResponseHeader          header;
    union {
        NumericalResponse   numerical;
        TextualResponse     textual;
    };
};

struct TraceHeader {
    MessageType type;

    TraceHeader (MessageType type) { this->type = type; }
};

struct Node {
    MAKE_STRING (name);
    MAKE_STRING (nspace);
    u_int64_t   handle;
    pid_t       pid;
    u_int32_t   stateChangeTime;    
};

struct Publisher {
    MAKE_STRING (topicName);
    u_int64_t   nodeHandle;
};

struct Subscriber {
    MAKE_STRING (topicName);
    u_int64_t   nodeHandle;
};

struct Service {
    MAKE_STRING (name);
    u_int64_t   nodeHandle;
};

struct Client {
    MAKE_STRING (srvName);
    u_int64_t   nodeHandle;
};

struct Timer {
    u_int64_t   nodeHandle;
    u_int32_t   frequency;
};

struct LifecycleSMInit {
    u_int64_t   nodeHandle;
    u_int64_t   stateMachine;
};

enum LifeCycleState {
    INVALID,
    UNCONFIGURED,
    INACTIVE,
    ACTIVE,
    FINALIZED,
};

struct LifecycleTransition {
    u_int64_t       stateMachine;
    LifeCycleState  state;
};

struct TraceMessage {
    TraceHeader     header;
    union {
        Node                node;
        Publisher           publisher;
        Subscriber          subscriber;
        Service             service;
        Client              client;
        Timer               timer;
        LifecycleSMInit     smInit;
        LifecycleTransition lcTrans;
    };

    TraceMessage (MessageType type) : header (type) {}
};

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
    SHMChannel(const char* name, bool create = false);
    ~SHMChannel();

    void send(const T& message);
    bool receive(T& out_message, bool wait = true);

private:
    void init_shared_buffer();

    const char* shm_name;
    int shm_fd;
    SharedBuffer<T>* buffer;
    bool created;
};

}
