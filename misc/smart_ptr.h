#ifndef smart_ptr_h
#define smart_ptr_h
#include <iostream>
#include <atomic>

template<typename T>
class smart_ptr;

class smart_ptr_ref {

public:
    smart_ptr_ref() : refNum_(0) {

    }
    virtual ~smart_ptr_ref() {

    }
    smart_ptr_ref(const smart_ptr_ref& that) {

    }

    void operator = (const smart_ptr_ref& that) {

    }
    void add_reference() {
        refNum_++;
    }

    bool del_reference() {
        if (!refNum_--) {
            delete this;
            return true;
        }
        return false;
    }
    smart_ptr_ref* operator&() = delete;

private:
    std::atomic<int> refNum_;
};

template<typename T>
class smart_ptr : public smart_ptr_ref {
public:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    smart_ptr() : dataPTR_(nullptr) {

    }
    // copy constructor
    smart_ptr(const smart_ptr& that) {
        dataPTR_ = that.dataPTR_;
        dataPTR_->add_reference();
    }
    smart_ptr(T* that) {
        dataPTR_ = that;
        //dataPTR_->add_reference();
    }
    // default destructor
    ~smart_ptr() {
        if (dataPTR_ != nullptr) {
            if (dataPTR_->del_reference()) {
                dataPTR_ = nullptr;
            }
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // assignment for smartPointer
    void operator =(T* that) {
        if (dataPTR_ != nullptr) {
            if (dataPTR_->del_reference()) {
                dataPTR_ = nullptr;
            }
        }
        dataPTR_ = that;
        dataPTR_->add_reference();
    }



    smart_ptr<T>* operator&() = delete;

    T& operator*() {
        return *dataPTR_;
    }
    T* operator->() {
        return dataPTR_;
    }

protected:
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // ptr to internal data
    T* dataPTR_;
};


#endif




