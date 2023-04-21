#ifndef HYPERCOMPLEX_H
#define HYPERCOMPLEX_H

#include <cstdint>
#include <iostream>

template< uint16_t dimension >
struct hyper_complex {
    static_assert( dimension == 1 || dimension == 2 || dimension == 4 || dimension == 8 || dimension == 16 || dimension == 32 || dimension == 64 || dimension == 128 || dimension == 256 , "Dimension Invalid or greater than 256");
    float raw_values[dimension];

    void print(){
        std::cout << "<";
        for(int i = 0; i < dimension; i++)
            std::cout << raw_values[i] << ",";
        std::cout << "\b>\n";
    }

    hyper_complex<dimension> conj() const{
        hyper_complex<dimension> retMe;
        retMe.raw_values[0] = raw_values[0];
        for( int i = 1; i < dimension; i++)
            retMe.raw_values[i] = -raw_values[i];
        return retMe;
    }

    hyper_complex<(dimension>>1)> lower() const{
        hyper_complex<(dimension>>1)> retMe{};
        for( int i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = raw_values[i];
        return retMe;
    };

    hyper_complex<(dimension>>1)> upper() const{
        hyper_complex<(dimension>>1)> retMe{};
        for( int i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = raw_values[(dimension>>1)+i];
        return retMe;
    };

};

template< uint16_t dimension >
hyper_complex<dimension> operator + ( const hyper_complex<dimension>& A, const hyper_complex<dimension>& B ){

    hyper_complex<dimension> retMe{};

    for( int i = 0; i < dimension; i++ )
        retMe.raw_values[i] = A.raw_values[i] + B.raw_values[i];

    return retMe;
}

template< uint16_t dimension >
hyper_complex<dimension> operator - ( const hyper_complex<dimension>& A, const hyper_complex<dimension>& B ){

    hyper_complex<dimension> retMe{};

    for( int i = 0; i < dimension; i++ )
        retMe.raw_values[i] = A.raw_values[i] - B.raw_values[i];

    return retMe;
}

template< uint16_t dimension >
hyper_complex<dimension> operator * ( const hyper_complex<dimension>& A, const hyper_complex<dimension>& B ){

    if( dimension == 1 ){
        return hyper_complex<dimension>{A.raw_values[0]*B.raw_values[0]};
    }

    //cayley dickson construction/extension of the reals
    hyper_complex<dimension> retMe{};
    if constexpr ( dimension != 1){
        hyper_complex<(dimension>>1)> lower_part = (A.lower()*B.lower()-B.upper()*A.upper().conj());
        hyper_complex<(dimension>>1)> upper_part = (A.lower().conj()*B.upper()+B.lower()*A.upper());
        for( int i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = lower_part.raw_values[i];
        for( int i = 0; i < dimension>>1; i++)
            retMe.raw_values[i+(dimension>>1)] = upper_part.raw_values[i];
    }
    return retMe;

}

#endif//hyper_complex_H
