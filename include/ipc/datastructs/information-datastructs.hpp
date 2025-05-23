#pragma once

#include "ipc/common.hpp"
#include "ipc/datastructs/sharedMem-datastructs.hpp"

#include <chrono>


#define MSG_TYPE_NODE_REQUEST GET_COUNTER
struct NodeRequest
{
  primaryKey_t primaryKey;
  bool updates;
};

#define MSG_TYPE_NODE_ALIVE_UPDATE GET_COUNTER
struct NodeAliveUpdate
{
  primaryKey_t primaryKey;
  bool alive;
  time_t aliveChangeTime;
  int32_t bootCount;
  pid_t pid;
};

#define MSG_TYPE_NODE_PUBLISHES_TO_UPDATE GET_COUNTER
struct NodePublishersToUpdate
{
  primaryKey_t  primaryKey;
  primaryKey_t  publishesTo;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE GET_COUNTER
struct NodeSubscribersToUpdate
{
  primaryKey_t  primaryKey;
  primaryKey_t  subscribesTo;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_ISSERVERFOR_UPDATE GET_COUNTER
struct NodeIsServerForUpdate
{
  primaryKey_t  primaryKey;
  MAKE_STRING(srvName);
  primaryKey_t  clientNodeId;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_ISCLIENTOF_UPDATE GET_COUNTER
struct NodeIsClientOfUpdate
{
  primaryKey_t  primaryKey;
  MAKE_STRING(srvName);
  primaryKey_t  serverNodeId;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_ISACTIONSERVERFOR_UPDATE GET_COUNTER
struct NodeIsActionServerForUpdate
{
  primaryKey_t  primaryKey;
  MAKE_STRING(srvName);
  primaryKey_t  actionclientNodeId;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_ISACTIONCLIENTOF_UPDATE GET_COUNTER
struct NodeIsActionClientOfUpdate
{
  primaryKey_t  primaryKey;
  MAKE_STRING(srvName);
  primaryKey_t  actionserverNodeId;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_TIMERTO_UPDATE GET_COUNTER
struct NodeTimerToUpdate
{
  primaryKey_t  primaryKey;
  u_int32_t     frequency;
  bool          isUpdate = true;
};

#define MSG_TYPE_NODE_STATE_UPDATE GET_COUNTER
struct NodeStateUpdate
{
  primaryKey_t      primaryKey;
  sharedMem::State  state;
  bool              isUpdate = true;
};

#define MSG_TYPE_NODE_RESPONSE GET_COUNTER
struct NodeResponse
{
  primaryKey_t primaryKey;
  MAKE_STRING(name);
  MAKE_STRING(pkgName);
  bool alive;
  time_t aliveChangeTime;
  uint32_t bootCount;
  pid_t pid;

  size_t nrOfInitialUpdates;
};


#define MSG_TYPE_TOPIC_REQUEST GET_COUNTER
struct TopicRequest
{
  primaryKey_t primaryKey;
  double targetFrequency;
  bool updates;
  bool continuous;
};

#define MSG_TYPE_TOPIC_PUBLISHERS_UPDATE GET_COUNTER
struct TopicPublishersUpdate
{
  primaryKey_t primaryKey;
  primaryKey_t publisher;
  bool isUpdate = true;
};

#define MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE GET_COUNTER
struct TopicSubscribersUpdate
{
  primaryKey_t primaryKey;
  primaryKey_t subscriber;
  bool isUpdate = true;
};

#define MSG_TYPE_TOPIC_RESPONSE GET_COUNTER
struct TopicResponse
{
  primaryKey_t primaryKey;
  MAKE_STRING(name);
  MAKE_STRING(type);
  sharedMemoryLocation_t sharedMemoryLocation;
  double dataFrequency;

  size_t nrOfInitialUpdates;
};

struct TopicDataStreamObject
{
  primaryKey_t primaryKey;
  double bandwidth;
  double frequency;
};


#define MSG_TYPE_PROCESS_REQUEST GET_COUNTER
struct ProcessRequest
{
  pid_t primaryKey; // pid of requested process (NOT of requesting process)
  bool updates;
  bool continuous;
};

#define MSG_TYPE_PROCESS_CHILDREN_UPDATE GET_COUNTER
struct ProcessChildrenUpdate
{
  pid_t pid;
  pid_t child;
  bool isUpdate = true;
};

#define MSG_TYPE_PROCESS_RESPONSE GET_COUNTER
struct ProcessResponse
{
  pid_t pid;
  MAKE_STRING(name);
  sharedMemoryLocation_t sharedMemoryLocation;
  double dataFrequency;

  size_t nrOfInitialUpdates;
};

struct ProcessDataStreamObject
{
  pid_t pid;
  double cpuUsage;
  double ramUsage;
  double discUsage;
};

struct ProcessAccumulatedDataStreamObject
{
  pid_t pid;
  ProcessDataStreamObject normal, accumulated;
};

#define GETSINGLEATTRIBUTE_REQUEST GET_COUNTER
struct SingleAttributesRequest
{
  primaryKey_t  primaryKey;
  AttributeName attribute;
  bool          continuous;
};

#define GETSINGLEATTRIBUTE_RESPONSE GET_COUNTER
struct SingleAttributesResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

#define GETAGGREGATEDATTRIBUTE_REQUEST GET_COUNTER
struct AggregatedAttributesRequest
{
  struct {
    primaryKey_t  primaryKey;
    Tree          tree;
  } rootedTree1, rootedTree2;

  AttributeName attribute;
  BinOperation  binOperation;
  bool          continuous;
};

#define GETAGGREGATEDATTRIBUTE_RESPONSE GET_COUNTER
struct AggregatedAttributesResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

#define GETCUSTOMATTRIBUTE_REQUEST GET_COUNTER
struct CustomAttributesRequest
{
  MAKE_STRING_ARRAY(query);
  bool              continuous;
};

#define GETCUSTOMATTRIBUTE_RESPONSE GET_COUNTER
struct CustomAttributesResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

#define GETAGGREGATEDMEMBER_REQUEST GET_COUNTER
struct AggregatedMemberRequest
{
  struct {
    primaryKey_t  primaryKey;
    Tree          tree;
  } rootedTree1, rootedTree2;

  BinOperation  binOperation;
  bool          continuous;
};

#define GETAGGREGATEDMEMBER_RESPONSE GET_COUNTER
struct AggregatedMemberResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

#define GETCUSTOMMEMBER_REQUEST GET_COUNTER
struct CustomMemberRequest
{
  MAKE_STRING_ARRAY(query);
  bool              continuous;
};

#define GETCUSTOMMEMBER_RESPONSE GET_COUNTER
struct CustomMemberResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

#define GETSHMADDRESS_REQUEST GET_COUNTER
struct SHMAddressRequest
{
  primaryKey_t primaryKey;
};

#define GETSHMADDRESS_RESPONSE GET_COUNTER
struct SHMAddressResponse
{
  requestId_t requestID;
  MAKE_STRING (memAddress);
};

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

    TraceMessage (MessageType type) : header (type) {}
};