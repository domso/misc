#ifndef shared_queue_h
#define shared_queue_h


#include <queue>
#include <mutex>
#include "time.h"
#include "semaphore.h"
#include "assert.h"



namespace network {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    template <typename queue_data_type>
// thread-safe queue
    class shared_queue {
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        public:
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // constructor
            // -> numPriority: number of priorities (>0)
            shared_queue(int numPriority = 1) : queues_(numPriority), queuePopCount_(numPriority, 0), numPriority_(numPriority), blockSize_(0) {
                assert(numPriority > 0);
                sem_init(&semaphore_, 0, 0);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // no copy constructor
            shared_queue(const shared_queue& that) = delete;
            ~shared_queue() {
                sem_destroy(&semaphore_);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // check queue for new data
            // returns true on success
            // decrement internal data-counter (semaphore!)
            bool checkForData() {
                return sem_trywait(&semaphore_) == 0;
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // wait sec2wait seconds for new data
            // returns true on success
            // decrement internal data-counter (semaphore!)
            bool waitForData(const int sec2wait) {

                timespec ts;
                if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                    return false;
                }
                ts.tv_sec += sec2wait;
                return sem_timedwait(&semaphore_, &ts) == 0;

            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // after calling waitForData
            queue_data_type getData() {
                int tmp, i;
                std::lock_guard<std::mutex> lock(mutex_);
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                if (blockSize_ == 0) {
                    blockSize_ = 0;
                    tmp = -1;
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    for (i = 0 ; i < numPriority_ ; i++) {
                        if (tmp == -1 && queues_[i].size() != 0) {
                            tmp = i;
                        }
                        queuePopCount_[i] = queues_[i].size();
                        blockSize_ += queuePopCount_[i];
                    }
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    lastIndex_ = 0;
                    queue_data_type data = queues_[tmp].front();
                    queues_[tmp].pop();
                    queuePopCount_[tmp]--;
                    blockSize_--;
                    return data;
                    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                } else {
                    while (lastIndex_ < numPriority_) {
                        if (queuePopCount_[lastIndex_] > 0) {
                            queuePopCount_[lastIndex_]--;
                            blockSize_--;
                            queue_data_type data = queues_[lastIndex_].front();
                            queues_[lastIndex_].pop();
                            return data;
                        }
                        lastIndex_++;
                    }
                }
                //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                // we should never reached this point

            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // add new data with highest priority to queue
            // increments internal data-counter (semaphore!)
            void push(const queue_data_type data) {
                mutex_.lock();
                queues_[numPriority_ - 1].push(data);
                mutex_.unlock();
                sem_post(&semaphore_);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // add new data to queue
            // increments internal data-counter (semaphore!)
            void push(const queue_data_type data, int priority) {
                mutex_.lock();
                queues_[priority].push(data);
                mutex_.unlock();
                sem_post(&semaphore_);
            }
            //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            // set number of priorities
            // WARNING! this will delete all data in the queue
            // thread-safety is very risky!
            void setPriority(int numPriority) {
                mutex_.lock();
                queues_.clear();
                queues_.resize(numPriority);
                lastIndex_ = 0;
                blockSize_ = 0;

                do {
                    sem_trywait(&semaphore_);
                } while (errno != EAGAIN);


                mutex_.unlock();
            }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        private:
            // internal queues
            std::vector<std::queue<queue_data_type>> queues_;
            // last priority index
            int lastIndex_;
            // size of one block
            int blockSize_;
            // number of pop-data from queue with pririoty
            std::vector<int> queuePopCount_;
            // number if priorities = queues.size()
            int numPriority_;
            // mutex for critical-section
            std::mutex mutex_;
            // semaphore for waitForData()
            sem_t semaphore_;


    };
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}


#endif

