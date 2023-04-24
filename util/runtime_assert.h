#ifndef RUNTIME_ASSERT_H
#define RUNTIME_ASSERT_H

#include <string>
#include <map>
#include <cstdlib>
#include <iostream>


inline void crash(){ std::exit(0); }
inline std::map<uint64_t,void(*)(void)> callback_map = {{0,crash}};

inline void runtime_assert(bool cond, std::string message, uint64_t channel){
    if(cond)
        return;
    std::cout << message;
    if( callback_map.contains(channel) )
        callback_map[channel]();
}

inline void runtime_assert(bool cond, std::string message){
    runtime_assert( cond, message, 0 );
}

inline void runtime_assert_set_callback(uint64_t channel, void(*callback)(void) ){
    callback_map[channel] = callback;//not thread safe, should change before use in multi-threads
}

#endif//RUNTIME_ASSERT_H
