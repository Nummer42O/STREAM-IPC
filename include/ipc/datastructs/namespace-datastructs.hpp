#pragma once

#include "ipc/common.hpp"


namespace ipc::datastructs
{

#define MSG_TYPE_NAMESPACE_REQUEST GET_COUNTER
struct NamespaceRequest {
    requestId_t id;
    MAKE_STRING(path);
    bool updates;
};

#define MSG_TYPE_NAMESPACE_RESPONSE GET_COUNTER
struct NamespaceResponse {
    MAKE_STRING_ARRAY(children);
    uint32_t nrOfChildren;
};

}
