#include "network/ip_pkg.h"
/*
network::ip_pkg::ip_pkg() : dataPTR_(nullptr){

}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv4_pkg::ipv4_pkg(const std::vector<char>& buffer, const int len, const ipv4_addr* addr, const char defaultBufferValue) {
    dataPTR_ = new ipv4_pkg_data(buffer, len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv4_pkg::ipv4_pkg(const int len, const network::ipv4_addr* addr, const char defaultBufferValue) {
    dataPTR_ = new ipv4_pkg_data(len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv6_pkg::ipv6_pkg(const std::vector<char>& buffer, const int len, const ipv6_addr* addr, const char defaultBufferValue) {
    dataPTR_ = new ipv6_pkg_data(buffer, len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv6_pkg::ipv6_pkg(const int len, const network::ipv6_addr* addr, const char defaultBufferValue) {
    dataPTR_ = new ipv6_pkg_data(len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ip_pkg::ip_pkg(const network::ip_pkg& that) {
    dataPTR_ = that.dataPTR_;
    sem_post(& (dataPTR_->semaphore));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ip_pkg::~ip_pkg() {
    if (dataPTR_ != nullptr) {
        sem_wait(& (dataPTR_->semaphore));
        int tmp = 0;
        sem_getvalue(& (dataPTR_->semaphore), &tmp);
        if (tmp <= 0) {
            delete dataPTR_;
        }
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::ip_pkg::operator= (const network::ip_pkg that) {
        if (dataPTR_ != nullptr) {
        sem_wait(& (dataPTR_->semaphore));
        int tmp = 0;
        sem_getvalue(& (dataPTR_->semaphore), &tmp);
        if (tmp <= 0) {
            delete dataPTR_;
        }
    }
    dataPTR_ = that.dataPTR_;
    sem_post(& (dataPTR_->semaphore));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string network::ip_pkg::toString() const {
    return "[Package][" + std::to_string(dataPTR_->data.size()) + " Bytes]";
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::vector< char >& network::ip_pkg::getData() const {
    return dataPTR_->data;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ip_addr& network::ip_pkg::getAddr() const {
    return dataPTR_->getAddr();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
size_t network::ip_pkg::length() const {
    return dataPTR_->length_;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool network::ip_pkg::setLength(size_t length) {
    if (dataPTR_->data.size() < length) {
        return false;
    }
    dataPTR_->length_ = length;
    return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool network::ip_pkg::compare(std::string in, int startIndex, int count) {
    if (count == -1) {
        count = in.length();
    }
    for (int i = startIndex; i < startIndex + count; i++) {
        if (dataPTR_->data[i] != in[i]) {
            return false;
        }
    }
    return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::ip_pkg::ip_pkg_data::init(const std::vector< char >& buffer, const int len, const network::ip_addr* addr, const char defaultBufferValue) {
    sem_init(&semaphore, 0, 1);
    data.resize(len, defaultBufferValue);
    int startIndex = len - buffer.size();
    if (startIndex < 0) {
        startIndex = 0;
    }
    int msgLen;

    if (len > buffer.size()) {
        msgLen = buffer.size();
    } else {
        msgLen = len;
    }

    for (int i = 0; i < msgLen ; i++) {
        data[startIndex + i] = buffer[i];
    }
    if (addr != nullptr) {
            setAddr(addr);
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::ip_pkg::ip_pkg_data::init(const int len, const network::ip_addr* addr, const char defaultBufferValue) {
    sem_init(&semaphore, 0, 1);
    data.resize(len, defaultBufferValue);
    if (addr != nullptr) {
            setAddr(addr);
    }
    
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv4_pkg::ipv4_pkg_data::ipv4_pkg_data(const std::vector< char >& buffer, const int len, const network::ipv4_addr* addr, const char defaultBufferValue) {
    init(buffer, len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv4_pkg::ipv4_pkg_data::ipv4_pkg_data(const int len, const network::ipv4_addr* addr, const char defaultBufferValue) {
    init(len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv6_pkg::ipv6_pkg_data::ipv6_pkg_data(const std::vector< char >& buffer, const int len, const network::ipv6_addr* addr, const char defaultBufferValue) {
    init(buffer, len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ipv6_pkg::ipv6_pkg_data::ipv6_pkg_data(const int len, const network::ipv6_addr* addr, const char defaultBufferValue) {
    init(len, addr, defaultBufferValue);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::ip_pkg::ip_pkg_data::~ip_pkg_data() {
    sem_destroy(&semaphore);
}
network::ip_addr& network::ipv4_pkg::ipv4_pkg_data::getAddr() {
    return addr;
}
void network::ipv4_pkg::ipv4_pkg_data::setAddr(const ip_addr* addr) {
    this->addr = *static_cast<const ipv4_addr*>(addr);
}
network::ip_addr& network::ipv6_pkg::ipv6_pkg_data::getAddr() {
    return addr;
}
void network::ipv6_pkg::ipv6_pkg_data::setAddr(const network::ip_addr* addr) {
    this->addr = *static_cast<const ipv6_addr*>(addr);
}


*/