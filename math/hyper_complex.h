#ifndef HYPERCOMPLEX_H
#define HYPERCOMPLEX_H

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <bit>

template <typename T, uint64_t dim>
struct hyper_complex;

using real = hyper_complex<float,1>;
using complex = hyper_complex<float,2>;
using quaternion = hyper_complex<float,4>;
using octonion = hyper_complex<float,8>;
using sedenion = hyper_complex<float,16>;

template< typename T, uint64_t dimension >
struct hyper_complex {
    static_assert( std::popcount(dimension) == 1 , "Dimension Invalid");
    T raw_values[dimension];

    void print(){
        std::cout << "<";
        for(decltype(dimension) i = 0; i < dimension-1; i++)
            std::cout << raw_values[i] << ",";
        std::cout << raw_values[dimension-1];
        std::cout << ">\n";
    }

    friend std::ostream& operator << ( std::ostream& os, const hyper_complex<T,dimension>& serializeMe ){
        os << "<";
        for(decltype(dimension) i = 0; i < dimension-1; i++)
            os << serializeMe.raw_values[i] << ",";
        os << serializeMe.raw_values[dimension-1];
        os << ">";
        return os;
    }

    /**
        we don't follow the cayley dickson definition strictly.
        we assumme conjugation only negates all the imaginary parts.
        this is true for the pure cayley-dickson construction, but
        may not be true for a type that is partially cayley-dickson constructed
        and partially not. ie hyper_complex<matrix<float,3,3>,2> or worse

        should be correct regardless, but ye be warned.
    **/
    hyper_complex<T,dimension> conj() const{
        hyper_complex<T,dimension> retMe;

        // needed for nested hyper_complex objects. otherwise template hell
        constexpr bool has_conj = requires(const T& t) { t.conj(); };
        if constexpr ( has_conj )// case for nested complex objects, otherwise we miss values
            retMe.raw_values[0] = raw_values[0].conj();
        else
            retMe.raw_values[0] = raw_values[0];// trivial conj case for real component

        static_assert( requires(const T& t) { -t; }, "negation not supported");
        for( decltype(dimension) i = 1; i < dimension; i++)
            retMe.raw_values[i] = -raw_values[i];
        return retMe;
    }

    hyper_complex<T,(dimension>>1)> lower() const{
        hyper_complex<T,(dimension>>1)> retMe{};
        for( decltype(dimension) i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = raw_values[i];
        return retMe;
    };

    hyper_complex<T,(dimension>>1)> upper() const{
        hyper_complex<T,(dimension>>1)> retMe{};
        for( decltype(dimension) i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = raw_values[(dimension>>1)+i];
        return retMe;
    };

};

// addition element wise
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator + ( const hyper_complex<T,dimension>& A, const hyper_complex<T,dimension>& B ){

    hyper_complex<T,dimension> retMe{};

    for( decltype(dimension) i = 0; i < dimension; i++ )
        retMe.raw_values[i] = A.raw_values[i] + B.raw_values[i];

    return retMe;
}

// subtraction element wise
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator - ( const hyper_complex<T,dimension>& A, const hyper_complex<T,dimension>& B ){

    hyper_complex<T,dimension> retMe{};

    for( decltype(dimension) i = 0; i < dimension; i++ )
        retMe.raw_values[i] = A.raw_values[i] - B.raw_values[i];

    return retMe;
}

// negation element wise
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator - ( const hyper_complex<T,dimension>& A ){

    hyper_complex<T,dimension> retMe{};

    for( decltype(dimension) i = 0; i < dimension; i++ )
        retMe.raw_values[i] = -(A.raw_values[i]);

    return retMe;
}

// cayley-dickson multiplication
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator * ( const hyper_complex<T,dimension>& A, const hyper_complex<T,dimension>& B ){

    if constexpr ( dimension == 1 ){
        return hyper_complex<T,dimension>{A.raw_values[0]*B.raw_values[0]};
    }
    else{
        hyper_complex<T,dimension> retMe{};

        hyper_complex<T,(dimension>>1)> a = A.lower();
        hyper_complex<T,(dimension>>1)> b = A.upper();
        hyper_complex<T,(dimension>>1)> c = B.lower();
        hyper_complex<T,(dimension>>1)> d = B.upper();

        hyper_complex<T,(dimension>>1)> lower_part = (a*c-d.conj()*b);
        hyper_complex<T,(dimension>>1)> upper_part = (d*a+b*c.conj());

        for( decltype(dimension) i = 0; i < dimension>>1; i++)
            retMe.raw_values[i] = lower_part.raw_values[i];
        for( decltype(dimension) i = 0; i < dimension>>1; i++)
            retMe.raw_values[i+(dimension>>1)] = upper_part.raw_values[i];

        return retMe;
    }

}

//scalar multiplication
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator * ( const hyper_complex<T,dimension>& A, T B ){
    hyper_complex<T,dimension> retMe;
    for( decltype(dimension) i = 0; i < dimension; i++){
        retMe.raw_values[i] = A.raw_values[i]*B;
    }
    return retMe;
}

//scalar multiplication
template< typename T, uint64_t dimension >
hyper_complex<T,dimension> operator * ( T A, const hyper_complex<T,dimension>& B ){
    hyper_complex<T,dimension> retMe;
    for( decltype(dimension) i = 0; i < dimension; i++){
        retMe.raw_values[i] = B.raw_values[i]*A;
    }
    return retMe;
}

#endif//hyper_complex_H
