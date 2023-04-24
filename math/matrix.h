#ifndef MATRIX_H
#define MATRIX_H

template <typename T, uint64_t X, uint64_t Y>
struct matrix{
    T raw_values[X][Y];

    void print(){
        for( decltype(X) x = 0; x < X; x++ ){
            std::cout << "|";
            for( decltype(Y) y = 0; y < Y-1; y++ ){
                std::cout << raw_values[x][y] << ",";
            }
            std::cout << raw_values[x][Y-1];
            std::cout << "|\n";
        }
    }

    friend std::ostream& operator << ( std::ostream& os, const matrix<T,X,Y>& serializeMe ){
        for( decltype(X) x = 0; x < X-1; x++ ){
            std::cout << "|";
            for( decltype(Y) y = 0; y < Y-1; y++ ){
                std::cout << serializeMe.raw_values[x][y] << ",";
            }
            std::cout << serializeMe.raw_values[x][Y-1];
            std::cout << "|\n";
        }
        os << "|";
        for( decltype(Y) y = 0; y < Y-1; y++ )
            std::cout << serializeMe.raw_values[X-1][y] << ",";
        std::cout << serializeMe.raw_values[X-1][Y-1];
        os << "|";
        return os;
    }
};

// negation element wise
template <typename T, uint64_t X, uint64_t Y>
matrix<T,X,Y> operator - ( const matrix<T,X,Y>& A ){

    matrix<T,X,Y> retMe{};

    for( decltype(X) x = 0; x < X; x++ )
        for( decltype(Y) y = 0; y < Y; y++ )
            retMe.raw_values[x][y] = -(A.raw_values[x][y]);

    return retMe;
}

template <typename T, uint64_t X, uint64_t Y>
matrix<T,X,Y> operator + ( const matrix<T,X,Y>& A, const matrix<T,X,Y>& B ){

    matrix<T,X,Y> retMe{};

    for( decltype(X) x = 0; x < X; x++ )
        for( decltype(Y) y = 0; y < Y; y++ )
            retMe.raw_values[x][y] = A.raw_values[x][y] + B.raw_values[x][y];

    return retMe;
}

template <typename T, uint64_t X, uint64_t Y>
matrix<T,X,Y> operator - ( const matrix<T,X,Y>& A, const matrix<T,X,Y>& B ){

    matrix<T,X,Y> retMe{};

    for( decltype(X) x = 0; x < X; x++ )
        for( decltype(Y) y = 0; y < Y; y++ )
            retMe.raw_values[x][y] = A.raw_values[x][y] - B.raw_values[x][y];

    return retMe;
}

//matrix multiplication
template <typename T, uint64_t X, uint64_t Y, uint64_t Z>
matrix<T,X,Z> operator * ( matrix<T,X,Y> A, matrix<T,Y,Z> B ){
    matrix<T,X,Z> retMe;

    for( decltype(X) x = 0; x < X; x++ ){
        for( decltype(Z) z = 0; z < Z; z++ ){
            retMe.raw_values[x][z] = T{};//default construct all fields;
        }
    }

    constexpr bool has_plus_and_eq = requires(T& t) { t = t + t; };
    constexpr bool has_plus_eq = requires(const T& t) { t += t; };
    for( decltype(X) x = 0; x < X; x++ ){
        for( decltype(Z) z = 0; z < Z; z++ ){
            for( decltype(Y) y = 0; y < Y; y++ ){
                static_assert( (has_plus_and_eq || has_plus_eq) , "type does support addition" );
                if constexpr ( has_plus_eq )
                    retMe.raw_values[x][z] += A.raw_values[x][y]*B.raw_values[y][z];
                else if constexpr ( has_plus_and_eq )
                    retMe.raw_values[x][z] = retMe.raw_values[x][z] + A.raw_values[x][y]*B.raw_values[y][z];
            }
        }
    }

    return retMe;
}

//scalar multiplication
template< typename T, uint64_t X, uint64_t Y >
matrix<T,X,Y> operator * ( const matrix<T,X,Y>& A, const T& B ){
    matrix<T,X,Y> retMe;
    for( decltype(X) x = 0; x < X; x++ ){
        for( decltype(Y) y = 0; y < Y; y++ ){
            retMe.raw_values[x][y] = A.raw_values[x][y]*B;//default construct all fields;
        }
    }
    return retMe;
}

//scalar multiplication
template< typename T, uint64_t X, uint64_t Y >
matrix<T,X,Y> operator * ( const T& A, const matrix<T,X,Y>& B ){
    matrix<T,X,Y> retMe;
    for( decltype(X) x = 0; x < X; x++ ){
        for( decltype(Y) y = 0; y < Y; y++ ){
            retMe.raw_values[x][y] = A*B.raw_values[x][y];//default construct all fields;
        }
    }
    return retMe;
}

#endif//MATRIX_H
