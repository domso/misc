#ifndef udp_receiver_h
#define udp_receiver_h

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "ip_addr.h"
#include "ip_pkg.h"
#include "udp_socket.h"
#include "shared_queue.h"

#include "run_lock.h"


namespace network {
    template<typename IP_ADDR_TYPE,  typename HEADER_TYPE, typename DATA_TYPE>
    class udp_receiver {

    public:
        
        udp_receiver(const udp_socket<IP_ADDR_TYPE>& skt,int bufferSize) : skt_(skt),  msg_buffer_(bufferSize){
            
        }
        
        ~udp_receiver() {
            
        }
        
        void set_skt(const udp_socket<IP_ADDR_TYPE>& skt) {
            skt_ = skt;
        }
        
        void resizeBuffer(int bufferSize) {
            msg_buffer_.resize(bufferSize);
        }
        
        std::vector<char> msg_buffer_;
        udp_socket<IP_ADDR_TYPE>& skt_;
    };

}


#define NETWORK_UDP_RECEIVER_INIT_PARAM_BUFFERSIZE 1500
#define NETWORK_UDP_RECEIVER_INIT_PARAM_SEC2WAIT 0.1
#define NETWORK_UDP_RECEIVER_INIT_PARAM_MINTHREAD -1
#define NETWORK_UDP_RECEIVER_INIT_PARAM_MAXTHREAD -1
#define NETWORK_UDP_RECEIVER_INIT_PARAM_NUMTHREAD 4


namespace network {
    class udp_receiver {
        
        
        
        
        
    };
}


/*
namespace network {
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // smartPointer-callback-receiver for udp_socket
    class udp_receiver {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        public:
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // default constructor
            udp_receiver();
            // copy constructor
            udp_receiver(const udp_receiver& that);
            // default destructor
            ~udp_receiver();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // assignment for smartPointer
            void operator =(const udp_receiver that);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // additional parameters for init()  
                //int bufferSize; // size of the recv-buffer   
                //int sec2wait; // timeout for blocking calls                          
                //int maxThread; // maximum number of threads               
                //int minThread; // minimum number of threads     
                //int numThread; // default/start number of threads
                //void* addPtr // additional pointer, which will be passed to work_callbackFunction (default nullptr)


            struct udp_receiver_init_param {
                // default constructor
                // sets all default parameters
                udp_receiver_init_param();
                // size of the recv-buffer
                int bufferSize;
                // timeout for blocking calls
                float sec2wait;
                // maximum number of threads
                int maxThread;
                // minimum number of threads
                int minThread;
                // default/start number of threads
                int numThread;
                // additional pointer, which will be passed to work_callbackFunction
                void* addPtr;

            };
            // parameters:
            // -> socket: the udp_socket for receiving
            // -> work_callbackFunction: function which will be executed with incoming packages
            // -> udp_receiver_init_param: optional parameters
            void init(const network::udp_socket socket, void (*const work_callbackFunction)(ip_pkg&, const udp_socket&, const void* addPtr),const udp_receiver::udp_receiver_init_param* const parameters) const;
            // stops all threads
            // -> udp_receiver_data will NOT be freed until stop() is called!
            void stop();
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // gets current number of threads
            int getThreadNum();
            // set number of threads
            // -> create/delete threads
            // -> asynchron
            // -> return false if minThread > num and minThread != -1
            // -> return false if axThread < num and maxThread != -1
            // -> return true otherwise
            bool setThreadNum(int num);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        private:
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // data-struct for smartPointer
            struct udp_receiver_data {
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                udp_receiver_data();
                ~udp_receiver_data();
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // set all parameters
                void init(const network::udp_socket skt, void (*const work_cbFunction)(ip_pkg&, const udp_socket&, const void* addPtr),const udp_receiver::udp_receiver_init_param* const parameters);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // receiver-thread
                static void recvThread(network::udp_receiver::udp_receiver_data* receiver, int threadID);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // semaphore for smartPointer
                sem_t semaphore;
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // UDP-socket (=smartPointer!)
                udp_socket socket;
                // thread-handle for root-recv-threads
                std::thread root_recv_thread;
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // buffer for socket.recv()
                std::vector<char> buffer;
                // callback for incoming data (executed by workThread)
                void (*work_callbackFunction)(ip_pkg&, const udp_socket&, const void*);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                udp_receiver_init_param param;
                std::atomic<int> currentThreads;
                std::atomic<int> numThreads;

            };
            // private constructor to create a new smartPointer-Object from the internal ptr
            udp_receiver(network::udp_receiver::udp_receiver_data* dataPTR);
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // ptr to the internal data
            udp_receiver_data* dataPTR_;

    };
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


*/

#endif

