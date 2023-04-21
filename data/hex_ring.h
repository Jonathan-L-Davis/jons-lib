#ifndef HEX_RING_H
#define HEX_RING_H

#include <cstring>
#include <stdexcept>

template<typename T>
struct hex_ring{

    hex_ring(){
        bufferStart = nullptr;
        _capacity = 0;

        startPos = 0;
        _size = 0;
    }

    hex_ring(uint32_t layers){

        unsigned int size = 3*(layers*layers+layers)+1;

        bufferStart = new T[size];
        _capacity = size;
    }

    ~hex_ring(){
        delete[] bufferStart;//only free bufferStart since it points to the entire raw buffer
    }

    T& at( unsigned int layer, unsigned int position ){

        if( layer < 0 )
            throw std::out_of_range("Access out of range");
        if( position >= 6*layer )
            throw std::out_of_range("Access out of range");

        signed int lower_layer = layer-1;
        unsigned int cell = 3*(lower_layer*lower_layer+lower_layer)+position;

        if( cell < _capacity && cell >= 0 ){
            return bufferStart[startPos+cell];
        }
        else
        {
            throw std::out_of_range("Access out of range");
        }
    }

    unsigned int size(){
        return _size;
    }

    T *bufferStart;//contiguous mem start
    uint32_t _capacity;
    uint32_t _size;
    uint32_t startPos;

    T* operator [] ( unsigned int layer){

        unsigned int lower_layer = layer - 1;

        return bufferStart+3*(lower_layer*lower_layer+lower_layer);
    }

};

#endif//HEX_RING_H
