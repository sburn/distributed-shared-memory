#include "../include/DSMClient.h"

DSMClient::DSMClient(std::string name) : DSMBase(name) {
    initialize();
    start();
}

DSMClient::~DSMClient() {

}

void DSMClient::initialize() {
    //this should read from a config file or something, but for testing, just set some bufs
    registerLocalBuffer("name0", "pass0", 18);
    registerLocalBuffer("name0", "pass0", 17);
    registerLocalBuffer("name1", "pass0", 20);
    registerRemoteBuffer("name0", "pass1", "ipaddr0");
    registerRemoteBuffer("name0", "pass1", "ipaddr0");
    registerRemoteBuffer("name1", "pass1", "ipaddr0");
}

void DSMClient::start() {
    scoped_lock<interprocess_upgradable_mutex> lock(_lock->mutex);
    _lock->isReady = true;
    _lock->ready.notify_one();
    if (_lock->isReady) {
        _lock->ready.wait(lock);
    }
    _lock->isReady = true;
    _lock->ready.notify_one();
}

bool DSMClient::registerLocalBuffer(std::string name, std::string pass, int length) {
    scoped_lock<interprocess_upgradable_mutex> lock(_lock->mutex);
    if (_localBufferNames.find(name) == _localBufferNames.end()) {
        _localBufferNames.insert(name);
        _localBufferDefinitions->push_back(std::make_tuple(name, pass, length));
        _lock->localModified = true;
        return true;
    }
    return false;
}

std::string DSMClient::registerRemoteBuffer(std::string name, std::string pass, std::string ipaddr) {
    scoped_lock<interprocess_upgradable_mutex> lock(_lock->mutex);
    if (_remoteBufferNames.find(name) == _remoteBufferNames.end()) {
        _remoteBufferNames.insert(name);
        _remoteBufferDefinitions->push_back(std::make_tuple(name, pass, ipaddr));
        _lock->remoteModified = true;
        return ipaddr+"_"+name;
    }
    return "";
}