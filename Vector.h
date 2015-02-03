//
//  Vector.h
//  project1a
//
//  Created by zhiyuan zou on 1/28/15.
//  Copyright (c) 2015 zhiyuan zou. All rights reserved.
//
#pragma once
#ifndef _Vector_h
#define _Vector_h
using std::cout;
using std::endl;
namespace epl{
    
    template <typename T>
    class vector {
    private:
        T * start,*first,*end,*p_end;
    public:
        /*******/
        /*additional methods for test, should delete before submit*/
        void printv(void){
            cout<<"The size of the vector is "<<size()<<endl;
            T* current=first;
            while(current!=end){
            cout<<*current<<", ";
                current++;
            }
            cout<<endl;
        }
        /*******/
        vector(void){
            start=(T*)::operator new(8*sizeof(T));
            first=start;
            end=first;
            p_end=start+8;
        }
        vector(vector&& rhs){
            uint64_t n=rhs.size();
            T* newstart=(T*)::operator new(n*sizeof(T));
            T* newend=newstart;
            for(uint64_t i=0;i<n;i++){
                new(newend) T(std::move(rhs[i]));
                
                newend++;
            }
            //if(n!=0) ::operator delete(start); // if n==0, the second time it will crash, not sure why
            start=newstart;
            first=start;
            end=newend;
            p_end=end;
        }
        vector(const vector& rhs){
            copy(rhs);
        }
        explicit vector(uint64_t n){
            if(n==0) vector();
            else{
                start=(T*)::operator new(n*sizeof(T));
                T* temp=start;
                end=start+n;
                while(temp!=end){
                    new(temp) T{};
                    temp++;
                }
                first=start;
                p_end=end;
            }
        }
        uint64_t size(void) const{
            return end-first;
        }
        T& operator[](uint64_t i){
            if(i>=size()){throw std::out_of_range{"Index out of boundary!"};}
            else{
                return *(first+i);
            }
        }
        const T& operator[](uint64_t i) const{
            if(i>=size()){throw std::out_of_range{"Index out of boundary!"};}
            else{
                return *(first+i);
            }
        }
        void push_back(const T& ele){
            if(end!=p_end){
                new(end) T{ele};
                end++;
            }else if(first!=start){
                T* temp=first;
                T* newend=start;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
           //         temp->~T();
                    newend++;
                    temp++;
                }
                new(newend) T{ele};
                newend++;
                end=newend;
                first=start;
            }else{
                uint64_t n=2*size();
                T* newstart=(T*)::operator new(n*sizeof(T));
                T* temp=first;
                T* newend=start;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
            //        temp->~T();
                    newend++;
                    temp++;
                }
                new(newend) T{ele};
                destroy();
                newend++;
                end=newend;
                start=newstart;
                p_end=start+n;
                first=start;
            }
        }
        void push_back(T&& ele){
            if(end!=p_end){
                new(end) T{std::move_if_noexcept(ele)};
                end++;
            }else if(first!=start){
                T* temp=first;
                T* newend=start;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
                    //         temp->~T();
                    newend++;
                    temp++;
                }
                new(newend) T{std::move_if_noexcept(ele)};
                newend++;
                end=newend;
                first=start;
            }else{
                uint64_t n=2*size();
                T* newstart=(T*)::operator new(n*sizeof(T));
                T* temp=first;
                T* newend=start;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
                    //        temp->~T();
                    newend++;
                    temp++;
                }
                new(newend) T{std::move_if_noexcept(ele)};
                destroy();
                newend++;
                end=newend;
                start=newstart;
                p_end=start+n;
                first=start;
            }
        }
        void pop_back(void){
            if(size()==0) {throw std::out_of_range{"The Vector is empty!"};}
            else{
                end--;
                end->~T();
            }
        }
        void pop_front(void){
            if(size()==0) {throw std::out_of_range{"The Vector is empty!"};}
            else{
                first->T::~T();
                first++;
            }
        }
        
        void push_front(T&& ele){
            if(first!=start){
                first--;
                new(first) T{std::move_if_noexcept(ele)};
            }else if(end!=p_end){
                T* temp=end-1;
                T* newend=p_end-1;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
                    //         temp->~T();
                    newend--;
                    temp--;
                }
                new(newend) T{std::move_if_noexcept(ele)};
                first=newend;
                end=p_end;
            }
            else{
                uint64_t n=2*(p_end-start);
                uint64_t i;
                T* newstart=(T*)::operator new(n*sizeof(T));
                T* newend=newstart+n-1;
                T* temp=end-1;
                for(i=0;i<size();i++){
                    new(newend) T{std::move(*temp)};
                    newend--;temp--;
                }
                new(newend) T{std::move_if_noexcept(ele)};
                ::operator delete(start);
                start=newstart;
                first=newend;
                p_end=start+n;
                end=p_end;
            }
        }
        
        void push_front(const T& ele){
            if(first!=start){
                first--;
                new(first) T{ele};
                
            }else if(end!=p_end){
                T* temp=end-1;
                T* newend=p_end-1;
                for(uint64_t i=0;i<size();i++){
                    new(newend) T{std::move_if_noexcept(*temp)};
                    //         temp->~T();
                    newend--;
                    temp--;
                }
                new(newend) T{ele};
                first=newend;
                end=p_end;
            }
            else{
                uint64_t n=2*(p_end-start);
                uint64_t i;
                T* newstart=(T*)::operator new(n*sizeof(T));
                T* newend=newstart+n-1;
                T* temp=end-1;
                for(i=0;i<size();i++){
                    new(newend) T{std::move(*temp)};
                    newend--;temp--;
                }
                new(newend) T{ele};
                ::operator delete(start);
                start=newstart;
                first=newend;
                p_end=start+n;
                end=p_end;
            }
        }
        ~vector(){destroy();}
    private:
        void copy(const vector& rhs){
            uint64_t n=rhs.size();
            start=(T*)::operator new(n*sizeof(T));
            first=start;
            p_end=start+n;
            end=first;
            for(uint64_t i=0;i<n;i++){
                new(end) T(rhs[i]);
                end++;
            }
        }
        void destroy(void){
         
            while(first!=end){
                first->~T();
                first++;
            }
            ::operator delete(start);
        }
    };
    
} //namespace epl

#endif /* _Vector_h */
