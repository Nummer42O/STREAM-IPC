#pragma once

#include "ipc/common.hpp"


struct NodeRequest
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  bool updates;
};

struct NodeAliveUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  bool alive;
  time_t aliveChangeTime;
  int32_t bootCount;
  pid_t pid;
};

struct NodePublishesToUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, publishesTo);
};

struct NodeSubscribesToUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, subscribesTo);
};

struct NodeServicesUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  struct Service
  {
    MAKE_STRING(name);
    primaryKey_t nodeId;
  } services[MAX_ARRAY_SIZE];
};

struct NodeClientsUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, clients);
};

struct NodeResponse
{
  MAKE_MSG_TYPE;

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


struct TopicRequest
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  double targetFrequency;
  bool updates;
  bool continuous;
};

struct TopicPublishersUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, publishers);
};

struct TopicSubscribersUpdate
{
  MAKE_MSG_TYPE;

  primaryKey_t primaryKey;
  MAKE_ARRAY(primaryKey_t, subscribers);
};

struct TopicResponse
{
  MAKE_MSG_TYPE;

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


struct ProcessRequest
{
  MAKE_MSG_TYPE;

  pid_t pid; // primaryKey
  bool updates;
  enum class ContinuousType
  {
    ON, ACCUMULATED, OFF
  } continuous;
};

struct ProcessChildrenUpdate
{
  MAKE_MSG_TYPE;

  pid_t pid;
  MAKE_ARRAY(pid_t, children);
};

struct ProcessResponse
{
  MAKE_MSG_TYPE;

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
