#ifndef DSMSERVER_H
#define DSMSERVER_H

#include <iostream>
#include <string>
#include <tuple>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

#include "Lock.h"

using namespace boost::interprocess;

typedef std::tuple<managed_shared_memory::handle_t, int> Buffer;
typedef allocator<Buffer, managed_shared_memory::segment_manager> SharedBufferAllocator;
typedef map<std::string, Buffer, SharedBufferAllocator> BufferMap;

typedef std::tuple<std::string, std::string, std::string> BufferDefinition;
typedef allocator<BufferDefinition, managed_shared_memory::segment_manager> SharedBufferDefinitionAllocator;
typedef vector<BufferDefinition, SharedBufferDefinitionAllocator> BufferDefinitionVector;

class DSMServer {
    public:
        DSMServer(std::string name);
        ~DSMServer();

        void start();

        void dump();

    private:
        std::string _name;
        managed_shared_memory _segment;
        Lock *_lock;

        const SharedBufferDefinitionAllocator _sharedBufferDefinitionAllocator;
        BufferDefinitionVector *_bufferDefinitions;

        const SharedBufferAllocator _sharedBufferAllocator;
        BufferMap *_bufferMap;
};

#endif //DSMSERVER_H