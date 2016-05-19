#ifndef DSMBASE_H
#define DSMBASE_H

#include <string>
#include <tuple>
#include <cstdint>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>

#define SEGMENT_SIZE 65536
#define MAX_NUM_MESSAGES 10
#define MESSAGE_SIZE 32

using namespace boost::interprocess;

typedef std::tuple<managed_shared_memory::handle_t, std::uint16_t, offset_ptr<interprocess_upgradable_mutex>> Buffer;
typedef std::pair<const std::string, Buffer> MappedBuffer;
typedef allocator<MappedBuffer, managed_shared_memory::segment_manager> BufferAllocator;
typedef map<std::string, Buffer, std::less<std::string>, BufferAllocator> BufferMap;

class DSMBase {
    public:
        DSMBase(std::string name);
        virtual ~DSMBase() = 0;
    protected:
        std::string _name;
        managed_shared_memory _segment;

        message_queue _messageQueue;

        BufferMap *_localBufferMap;
        BufferMap *_remoteBufferMap;
        interprocess_upgradable_mutex* _localBufferMapLock;
        interprocess_upgradable_mutex* _remoteBufferMapLock;

        struct DSMMessage {
            uint16_t header;
            char name[26];      //makes this struct 32 bytes
            union footer {
                uint16_t size;  //max buffer size will probably be smaller than max value of 16 bit int
                uint8_t ipaddr[4];
            } footer;
        };
};

inline DSMBase::~DSMBase() {}

#endif //DSMBASE_H