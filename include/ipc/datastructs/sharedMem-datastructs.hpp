#pragma once

#include "ipc/common.hpp"

#include <chrono>


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
    time_t          timestamp;
    primaryKey_t    (primaryKey);
    double          value;
    MessageType     type;
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
    time_t      stateChangeTime;    
};

struct Publisher {
    MAKE_STRING (topicName);
    u_int64_t   nodeHandle;
    u_int64_t   publisherHandle;
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
    u_int64_t   handle;
};

struct LifecycleSMInit {
    u_int64_t   nodeHandle;
    u_int64_t   stateMachine;
};

enum State {
    INVALID,
    UNCONFIGURED,
    INACTIVE,
    ACTIVE,
    FINALIZED,
};

struct LifecycleTransition {
    u_int64_t   stateMachine;
    State       state;
    time_t      stateChangeTime;
};

struct TraceMessage {
    TraceHeader header;
    union {
        Node                node;
        Publisher           publisher;
        Subscriber          subscriber;
        Service             service;
        Client              client;
        Timer               timer;
        LifecycleSMInit     lcSmInit;
        LifecycleTransition lcTrans;
    };

    TraceMessage() : header(MessageType::NONE) {}
    TraceMessage (MessageType type) : header (type) {}
};

}