#ifndef __final_ptr_h__
#define __final_ptr_h__


#include<atomic>
#include<memory>
#include<mutex>

namespace su{
	template<typename T>
	//a final shared pointer,every instance can only assigned once except creating!
	class final_ptr{
		public:
			final_ptr<T>();
			final_ptr<T>(T* ptr);
			final_ptr<T>(const final_ptr<T>&);
			final_ptr<T>& operator=(const final_ptr<T>&);
			final_ptr<T>& operator=(T* ptr);
			~final_ptr<T>();

		private:
			T* ptr;
			std::atomic<int>* __ref_count;
			std::atomic<bool> assigned;
	};

	template<typename T>
    final_ptr<T>::final_ptr(){
        ptr=nullptr;
		__ref_count=new std::atomic<int>(1);
		assigned=false;
    }

    template<typename T>
    final_ptr<T>::final_ptr(T* ptr){
        __ref_count=new std::atomic<int>(1);
        if(ptr!=nullptr){
		    assigned=true;
        }else{
            assigned=false;
        }
    }

    template<typename T>
    final_ptr<T>::final_ptr(const final_ptr<T>& other){
        if(other.assigned.load()){
            other.__ref_count->fetch_add(1);
			assigned=true;
        }
        ptr=other.ptr;
        __ref_count=other.__ref_count;
    }

    template<typename T>
    final_ptr<T>& final_ptr<T>::operator=(const final_ptr<T>& other){
        if(!assigned.load()){
            throw std::runtime_error("Try to assigned value to a instance" 
                "which is protected by final ptr!");
        }
        if(other.assigned.load()){
            other.__ref_count->fetch_add(1);
			assigned=true;
        }
        ptr=other.ptr;
        __ref_count=other.__ref_count;
    }

	template<typename T>
    final_ptr<T>& final_ptr<T>::operator=(T* ptr){
        if(!assigned.load()){
            throw std::runtime_error("Try to assigned value to a instance" 
                "which is protected by final ptr!");
        }
        this->ptr=ptr;
        __ref_count->store(1);
    }

    template<typename T>
    final_ptr<T>::~final_ptr(){
        if(!assigned.load()){
            delete __ref_count;
            return;
        }
        if(__ref_count->fetch_sub(1)==1){
            delete __ref_count;
            delete ptr;
        }
    }
};

#endif