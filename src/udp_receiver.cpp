#include "network/udp_receiver.h"
#include "assert.h"
#include "signal.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver() : dataPTR_(nullptr) {
    dataPTR_ = new udp_receiver_data();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver(const network::udp_receiver& that) {
    dataPTR_ = that.dataPTR_;
    sem_post(& (dataPTR_->semaphore));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver(network::udp_receiver::udp_receiver_data* dataPTR) {
    assert(dataPTR != nullptr);
    dataPTR_ = dataPTR;
    sem_post(& (dataPTR_->semaphore));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::~udp_receiver() {
    sem_wait(& (dataPTR_->semaphore));
    int tmp = 0;
    sem_getvalue(& (dataPTR_->semaphore), &tmp);
    if (tmp <= 0) {
        delete dataPTR_;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::udp_receiver::operator= (const network::udp_receiver that) {
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
void network::udp_receiver::init(const network::udp_socket socket, void (*const work_callbackFunction)(ip_pkg&, const udp_socket&, const void* addPtr), const udp_receiver::udp_receiver_init_param* const parameters) const {
    dataPTR_->init(socket, work_callbackFunction, parameters);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::udp_receiver::stop() {
    dataPTR_->numThreads.store(0);
    if (dataPTR_->root_recv_thread.joinable()) {
        dataPTR_->root_recv_thread.join();
    }
}
int network::udp_receiver::getThreadNum() {
    return dataPTR_->numThreads.load();
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool network::udp_receiver::setThreadNum(int num) {
    if ((dataPTR_->param.minThread != -1 && dataPTR_->param.minThread > num) || (dataPTR_->param.maxThread != -1 && dataPTR_->param.maxThread < num)) {
        return false;
    }
    dataPTR_->numThreads.store(num);
    return true;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver_data::udp_receiver_data() {
    sem_init(&semaphore, 0, 1);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver_data::~udp_receiver_data() {
    sem_destroy(&semaphore);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network::udp_receiver::udp_receiver_init_param::udp_receiver_init_param() {
    bufferSize = NETWORK_UDP_RECEIVER_INIT_PARAM_BUFFERSIZE;
    sec2wait = NETWORK_UDP_RECEIVER_INIT_PARAM_SEC2WAIT;
    minThread = NETWORK_UDP_RECEIVER_INIT_PARAM_MINTHREAD;
    maxThread = NETWORK_UDP_RECEIVER_INIT_PARAM_MAXTHREAD;
    numThread = NETWORK_UDP_RECEIVER_INIT_PARAM_NUMTHREAD;
    addPtr = nullptr;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::udp_receiver::udp_receiver_data::init(const network::udp_socket skt, void (*const work_cbFunction)(ip_pkg&, const udp_socket&, const void* addPtr), const udp_receiver::udp_receiver_init_param* const parameters) {
    socket = skt;
    work_callbackFunction = work_cbFunction;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (parameters != nullptr) {
        // set config
        param = *parameters;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    buffer.resize(param.bufferSize, '\0');
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    numThreads.store(param.numThread);
    currentThreads.store(0);
    socket.setTimeout(param.sec2wait);
    root_recv_thread = std::thread(recvThread, this, 0);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void network::udp_receiver::udp_receiver_data::recvThread(network::udp_receiver::udp_receiver_data* receiver, int threadID) {
    int recvBytes = 0;
    udp_receiver recv(receiver);
    int numThreads;
    std::thread next;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ip_pkg pkg;
    if (receiver->socket.getFamily() == AF_INET) {
        ipv4_pkg pkg_v4(receiver->param.bufferSize, nullptr);
        pkg = pkg_v4;
    } else {
        ipv6_pkg pkg_v6(receiver->param.bufferSize, nullptr);
        pkg = pkg_v6;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    while (true) {
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if (threadID == receiver->currentThreads.load()) {
            numThreads = receiver->numThreads.load();
            if (threadID >= numThreads) {
                break;
            }
            if (threadID + 1 < numThreads) {
                
                receiver->currentThreads++;
                
                if (next.joinable()) {
                    next.join();
                }
                next = std::thread(recvThread, receiver, threadID + 1);
            }
        }
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        if ((recvBytes = receiver->socket.recv(pkg.getAddr(), pkg.getData())) > 0) {
            pkg.setLength(recvBytes);
            pkg.getAddr().update();
            receiver->work_callbackFunction(pkg, receiver->socket, receiver->param.addPtr);
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (next.joinable()) {
        next.join();
    }
    receiver->currentThreads--;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
