#ifndef RING_H
#define RING_H

#include <cstring>
#include <stdexcept>

template<typename T>
struct ring{

    ring(){
        bufferStart = nullptr;
        _capacity = 0;

        startPos = 0;
        _size = 0;
    }

    ring(uint32_t size){
        bufferStart = new T[size];
        _capacity = size;

        startPos = 0;
        _size = 0;
    }

    ~ring(){
        delete[] bufferStart;//only free bufferStart since it points to the entire raw buffer
    }

    T& operator [](unsigned int pos){

        //if access wraps, ie. element would be past end of raw buffer
        if(startPos+pos < _capacity){
            return bufferStart[startPos+pos];
        }
        else
        {
            return bufferStart[startPos+pos-_capacity];
        }
    }

    T& at(unsigned int pos){

        if( pos < _capacity && pos >= 0 ){
            if(startPos+pos < _capacity){
                return bufferStart[startPos+pos];
            }
            else
            {
                return bufferStart[startPos+pos-_capacity];
            }
        }
        else
        {
            throw std::out_of_range("Access out of range");
        }
    }

    void push_back(T value){

        if(startPos+_size < _capacity){
            bufferStart[startPos+_size] = value;
        }
        else
        {
            bufferStart[startPos+_size-_capacity] = value;
        }

        if( _size == _capacity){
            //delete first element, add one
            startPos++;
            if(startPos >= _capacity)
                startPos -= _capacity;
        }
        else
        {
            //keep first element, add one
            _size++;
        }
    }

    unsigned int size(){
        return _size;
    }

    void resize(unsigned int newCap){
        if( newCap == _capacity )//same size, do nothing
        {
            return;
        }

        T* nwBuffer = new T[newCap];

        if( newCap > _capacity )//grow
        {

            if( startPos + _size <= _capacity )//no wrap
            {
                std::memcpy( nwBuffer, bufferStart+startPos, _size*sizeof(T) );
            }
            else// w/ wrapping
            {
                std::memcpy( nwBuffer, bufferStart+startPos, (_capacity-startPos)*sizeof(T) );
                std::memcpy( nwBuffer+_capacity-startPos, bufferStart, (_size-_capacity+startPos)*sizeof(T) );
            }
        }
        else//shrink
        {
            if( _size <= newCap )//no lost elements
            {
                if( startPos + _size <= _capacity )//no wrap
                {
                    std::memcpy( nwBuffer, bufferStart+startPos, _size*sizeof(T) );
                }
                else// w/ wrapping
                {
                    std::memcpy( nwBuffer, bufferStart+startPos, (_capacity-startPos)*sizeof(T) );
                    std::memcpy( nwBuffer+_capacity-startPos, bufferStart, (_size-_capacity+startPos)*sizeof(T) );
                }
            }
            else//lost elements
            {
                _size = newCap;
                if( startPos + _size <= _capacity )//no wrap
                {
                    std::memcpy( nwBuffer, bufferStart+startPos, _size*sizeof(T) );
                }
                else// w/ wrapping
                {
                    std::memcpy( nwBuffer, bufferStart+startPos, (_capacity-startPos)*sizeof(T) );
                    std::memcpy( nwBuffer+_capacity-startPos, bufferStart, (_size-_capacity+startPos)*sizeof(T) );
                }
            }
        }

        delete[] bufferStart;
        bufferStart = nwBuffer;
        startPos = 0;
        _capacity = newCap;

    }

    void pop(){
        _size--;
        startPos++;
        if(startPos == _capacity)
            startPos -= _capacity;
    }

    private:
    T *bufferStart;//contiguous mem start
    uint32_t _capacity;
    uint32_t _size;
    uint32_t startPos;

};

#endif//RING_H
