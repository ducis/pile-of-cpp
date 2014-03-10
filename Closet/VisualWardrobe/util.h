#pragma once 
#include <cassert>
template<typename return_type> bool Failed( return_type ret );


#define RETURN_IF_FAIL( call , ... ) if(Failed( call )) { assert(0); return __VA_ARGS__; }

#define BREAK_IF_FAIL( call , ... ) if(Failed( call )) { assert(0); return __VA_ARGS__; }