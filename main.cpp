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

#include <singleton.hpp>
#include <the_one_true_foo.hpp>
#include <logging_resource.hpp>

#include <memory_resource>
#include <iostream>
#include <functional>
#include <memory_resource>
#include <format>
#include <print>

using namespace ndof;

namespace { 

    //------------------------------------------------------------
    // Set aside some memory in program memory.
    std::array<std::byte, 1024u * 1024u> buffer;

    // Create a memory resource from the buffer that only grows.
    //   If the memory requirements grow beyond the size, it will
    //   allocate from the heap.
    std::pmr::monotonic_buffer_resource buffer_resource(buffer.data(), buffer.size());

    // Pass this buffer to our logging wrapper resource.
    LoggingResource foo_resource(&buffer_resource);

}

//------------------------------------------------------------
// Configure the behavior of the singleton of type TheOneTrueFoo
 
// Optionally configure the singleton's constructor parameters and allocator
//    using template specialization.

// Redefine either of these methods, otherwise default behavior.
template<>
auto SingletonConfiguration::get_allocator<TheOneTrueFoo>() {
    // The singleton for TheOneTrueFoo should use a polymorphic
    //  allocator with foo_resource as its memory provider.
    
    // This result can be dynamically created, since get_allocator is
    //  only called when an instance of TheOneTrueFoo is requested.
    return std::pmr::polymorphic_allocator<TheOneTrueFoo>(&foo_resource);
};

template<>
auto  SingletonConfiguration::get_constructor_parameters<TheOneTrueFoo> (
    SingletonAlloc<TheOneTrueFoo>& ) {
    // The parameters which will be used to build the single instance
    //  of TheOneTrueFoo.  This can be dynamically generated, since this
    //  method is called lazily.
    return std::tuple {3,4.2};
} 

//------------------------------------------------------------
using PmrVecInt = std::pmr::vector<int>;

template<>
auto SingletonConfiguration::get_allocator<PmrVecInt>( ) {
    // Use a polymorphic allocator with the foo_resource. 
    //  This allocator will be supply the memory for the instance
    //  of PmrVecInt.
    return std::pmr::polymorphic_allocator<PmrVecInt>(&foo_resource);
};

//------------------------------------------------------------

int main(){
    // This resource can be queried if no allocator is supplied to 
    //    an allocator-ready class.  It represents the global default
    //    policy.
    std::pmr::set_default_resource(std::pmr::new_delete_resource());

    // Instantiate the one true Foo.
    auto& foo = Singleton<TheOneTrueFoo>::instance();

    // Go ahead and boop it.
    foo.boop();

    // Test the rebinding and passing of the polymorphic allocator, copying the 
    //   memory resource to the new allocator of the managed object.  The
    //   managed object will use that newly constructed polymorphic allocator
    //   to allocate its members, passing the allocator along to children, as
    //   required.
    auto x = Singleton<PmrVecInt>::instance();

    // If a client doesn't care about any of the overloads, this is all that is
    //   requried to create a singleton of a type:
    struct SomeType{};
    [[maybe_unused]] auto y = Singleton<SomeType>::instance();
 
    // After main exits, the destructor of the singleton will be called, in turn 
    //   calling the destructor of our foo.
    std::cout << "Main exiting.\n";
    return 0;
}
 
