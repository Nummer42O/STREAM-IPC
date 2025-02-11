#pragma once

#include "ipc/common.hpp"


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
struct NodePublishesToUpdate
{
  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, publishesTo);
};

#define MSG_TYPE_NODE_SUBSCRIBES_TO_UPDATE GET_COUNTER
struct NodeSubscribesToUpdate
{
  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, subscribesTo);
};

#define MSG_TYPE_NODE_SERVICES_UPDATE GET_COUNTER
struct NodeServicesUpdate
{
  primaryKey_t primaryKey;
  struct Service
  {
    MAKE_STRING(name);
    primaryKey_t nodeId;
  } services[MAX_ARRAY_SIZE];
};

#define MSG_TYPE_NODE_CLIENTS_UPDATE GET_COUNTER
struct NodeClientsUpdate
{
  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, clients);
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

  NodePublishesToUpdate publishesToInitialUpdate;
  NodeSubscribesToUpdate subscribesToInitialUpdate;
  NodeServicesUpdate servicesInitialUpdate;
  NodeClientsUpdate clientsInitialUpdate;
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
  MAKE_ARRAY(primaryKey_t, publishers);
};

#define MSG_TYPE_TOPIC_SUBSCRIBERS_UPDATE GET_COUNTER
struct TopicSubscribersUpdate
{
  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, subscribers);
};

#define MSG_TYPE_TOPIC_RESPONSE GET_COUNTER
struct TopicResponse
{
  primaryKey_t primaryKey;
  MAKE_STRING(name);
  MAKE_STRING(type);
  sharedMemoryLocation_t sharedMemoryLocation;
  double dataFrequency;

  TopicPublishersUpdate publisherInitialUpdate;
  TopicSubscribersUpdate sublisherInitialUpdate;
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
  pid_t pid; // primaryKey
  bool updates;
  enum class ContinuousType
  {
    ON, ACCUMULATED, OFF
  } continuous;
};

#define MSG_TYPE_PROCESS_CHILDREN_UPDATE GET_COUNTER
struct ProcessChildrenUpdate
{
  pid_t pid;
  MAKE_ARRAY(pid_t, children);
};

#define MSG_TYPE_PROCESS_RESPONSE GET_COUNTER
struct ProcessResponse
{
  pid_t pid;
  MAKE_STRING(name);
  sharedMemoryLocation_t sharedMemoryLocation;
  double dataFrequency;

  ProcessChildrenUpdate cildrenInitialUpdate;
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
