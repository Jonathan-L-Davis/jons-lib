#ifndef HEX_VEC_H
#define HEX_VEC_H

#include <cstring>
#include <stdexcept>

template<typename T>
struct hex_vec{

    hex_vec(){
        bufferStart = nullptr;
        _layers = 0;
    }

    hex_vec(uint32_t layers){

        if( layers != 0){
            unsigned int size = 3*(layers*layers+layers)+1;

            bufferStart = new T[size];
            _layers = layers;
            return;
        }
        _layers = 0;
        bufferStart = nullptr;
    }

    ~hex_vec(){
        delete[] bufferStart;
    }

    T* operator [] ( unsigned int layer){

        unsigned int lower_layer = layer - 1;
        if( layer == 0 )
            return bufferStart;
        return bufferStart+(3*(lower_layer*lower_layer+lower_layer)+1);
    }

    T& at( unsigned int layer, unsigned int position ){

        if( layer < 0 )
            throw std::out_of_range("Access out of range");
        if( position >= 6*layer )
            throw std::out_of_range("Access out of range");

        signed int lower_layer = layer-1;
        unsigned int cell = 3*(lower_layer*lower_layer+lower_layer)+position;

        if( cell < _layers && cell >= 0 ){
            return bufferStart[position+cell];
        }
        else
        {
            throw std::out_of_range("Access out of range");
        }
    }

    unsigned int size(){
        uint32_t layers = _layers-1;
        return _layers?(3*(layers*layers+layers)+1):0;
    }

    unsigned int layers(){
        return _layers;
    }

    unsigned int layer_size( unsigned int layer){
        if( layer > _layers )
            return 0;
        return (layer)?6*(layer):1;
    }

    void resize(unsigned int new_layers){

        if( new_layers == 0 ){
            _layers = 0;
            delete[] bufferStart;
            bufferStart = nullptr;
            return;
        }

        if( new_layers == _layers )//same size, do nothing
        {
            return;
        }

        new_layers--;

        T* nwBuffer = new T[3*(new_layers*new_layers+new_layers)+1];

        if( bufferStart == nullptr ){
            bufferStart = nwBuffer;
            _layers = new_layers+1;
            return;
        }

        if( new_layers > _layers )//grow
        {
            std::memcpy( nwBuffer, bufferStart, (3*(_layers*_layers+_layers)+1)*sizeof(T) );
        }
        else//shrink
        {
            std::memcpy( nwBuffer, bufferStart, (3*(new_layers*new_layers+new_layers)+1)*sizeof(T) );
        }

        delete[] bufferStart;
        bufferStart = nwBuffer;
        _layers = new_layers+1;

    }

    private:
    T *bufferStart;
    uint32_t _layers;

};

#endif//HEX_VEC_H
