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
struct StandardSingleAttributesRequest
{
  primaryKey_t primaryKey;
  Attribute attribute;
  bool continuous;
};

#define GETSINGLEATTRIBUTE_RESPONSE GET_COUNTER
struct StandardSingleAttributesResponse
{
  primaryKey_t primaryKey;
  double value;
};