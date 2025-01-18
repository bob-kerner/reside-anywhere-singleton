//-------------------------------------------------------------------------------------------------------------
// Copyright Bob Kerner, 2025. 
// Contact: (727) 560 - 0408. rekerner-at-gmail.com. https://www.linkedin.com/in/bkerner/
// Original Github: https://github.com/bob-kerner/reside-anywhere-singleton.git
//-------------------------------------------------------------------------------------------------------------
// Custom MIT License with Attribution Requirement

// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//   documentation files (the "Software"), to deal in the Software without restriction, including without 
//   limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
//   the Software, and to permit persons to whom the Software is furnished to do so, subject to the following 
//   conditions:
//   
//   -- The above copyright notice and this permission notice shall be included in all copies or substantial 
//         portions of the Software.
//
//   -- Any modifications to the Software, including derivative works, must include clear attribution to the 
//         original author, as well as a link to the original repository or source, located at: 
//         https://github.com/bob-kerner/reside-anywhere-singleton.git

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
//   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO 
//   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER 
//   IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR 
//   THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-------------------------------------------------------------------------------------------------------------

#include "logging_resource.hpp"
#include <print>
#include <format>

LoggingResource::LoggingResource(std::pmr::memory_resource* p): wrapped_resource_ptr(p){}

void* LoggingResource::do_allocate( std::size_t bytes, std::size_t alignment ) {
    bytes_allocated += bytes;
    std::print("Allocating...   Number of bytes: {}, Alignment size: {}, Bytes allocated: {}\n", bytes, alignment,bytes_allocated);
    return wrapped_resource_ptr->allocate(bytes,alignment); 
}

void LoggingResource::do_deallocate( void* p, std::size_t bytes, std::size_t alignment ) {
    bytes_allocated -= bytes;
    std::print("Deallocating... Number of bytes: {}, Alignment size: {}, Bytes allocated: {}\n", bytes, alignment, bytes_allocated);
    return wrapped_resource_ptr->deallocate(p, bytes,alignment);
}

bool LoggingResource::do_is_equal( const std::pmr::memory_resource& other ) const noexcept {
    return &other == wrapped_resource_ptr;
}
 

std::size_t LoggingResource::bytes_allocated = 0;
