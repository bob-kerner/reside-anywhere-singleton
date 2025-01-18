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

#ifndef NDOF_LOGGING_RESOURCE_HPP
#define NDOF_LOGGING_RESOURCE_HPP

#include <memory_resource>

// Example resource that tracks how much memory is allocated and deleted.
struct LoggingResource : std::pmr::memory_resource {
private:
    std::pmr::memory_resource* wrapped_resource_ptr;
    static std::size_t bytes_allocated;

public:
    LoggingResource ( std::pmr::memory_resource* p) ;
    ~LoggingResource () = default;

    void* do_allocate ( std::size_t bytes, std::size_t alignment )              override;
    void  do_deallocate ( void* p, std::size_t bytes, std::size_t alignment )   override;
    bool  do_is_equal ( const std::pmr::memory_resource& other ) const noexcept override;

};

#endif
