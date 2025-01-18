//-------------------------------------------------------------------------------------------------------------
// Copyright Bob Kerner, 2025. (727) 560 - 0408. rekerner-at-gmail.com. https://www.linkedin.com/in/bkerner/
// Original Github: 
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
//         [Insert URL Here].

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
//   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO 
//   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER 
//   IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR 
//   THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-------------------------------------------------------------------------------------------------------------

#include <mutex>
#include <memory>
#include <memory_resource>
#include <functional>
#include <type_traits>
#include <iostream>
#include <format>
 
namespace ndof { 

    // TODO: create an allocator-aware array wrapper.

    //------------------------------------------------------------------------------------------------------------------------
    // A helper that will be used to determine whether two types'
    //   allocators are compatible.
    
    template<typename T, typename U>
    concept CopyConstructible = requires (T t){
        U(t); 
    };

    //------------------------------------------------------------------------------------------------------------------------
    // If a type intended as a singleton is "allocator-aware", it must
    //   have a constructor that follows one of the follow two patterns:
    //    
    //   1.) The type defines a constructor that takes an std::allocator_arg_t 
    //         as its first argument and an Allocator as it's second
    //         parameter.
    //
    //   2.) The type defines a constructor that takes an allocator as its 
    //         last argument.
    //
    // An allocator-aware type also defines an alias for allocator_type.

    // As with std::vector and other containers.
    template<typename T, typename Alloc, typename ...Args>
    concept HasTaggedAllocatorConstructor = requires (Alloc alloc, Args ...args) {
        T(std::allocator_arg, alloc, args...);
    };

    // As with std::tuple, std::function and others.
    template<typename T, typename Alloc, typename ...Args>
    concept HasTrailingAllocatorConstructor = requires (Args ...args, Alloc alloc) {
        T(args..., alloc);
    };

    // Check to see if type T has an allocator_type alias, i.e., it may be a
    //  allocator-aware type.  We'll also need to find one of the constructors 
    //  as defined above.
    template<typename T>
    concept DefinesAllocatorType = requires {
        typename T::allocator_type;
    };

    //------------------------------------------------------------------------------------------------------------------------
    // This is the default singleton configuration, whose members
    //   defined below can each be optionally specialized.
    //   If no specialization exists, an empty argument list is
    //   used to create the singleton object instance.  The 
    //   allocator to be used for singleton's of type T can be 
    //   specialized also, but if not specialized, the standard
    //   allocator will be used.

    struct SingletonConfiguration{

        // Can be specialized for any type intended as a singleton.
        template<typename T>
        static auto get_allocator();

        template<typename T>
        using Alloc = decltype(SingletonConfiguration::get_allocator<T>());

        // When specialized, return a tuple of parameters to be 
        template<typename T>
        static auto get_constructor_parameters(Alloc<T>& alloc);

    };

    // For convenience.
    template<typename T>
    using SingletonAlloc = typename SingletonConfiguration::Alloc<T>;


    // If a type does not specialize an allocator for the singleton to use,
    //   the allocator_type alias of T is used to construct an allocator if defined, 
    //   otherwise return a polymorphic allocator that is constructed with whatever
    //   is set as the default memory resource, std::new_delete_resource, by default.

    template<typename T>
    auto SingletonConfiguration::get_allocator () {
        // If no specialization exists, just return the standard allocator.
        return std::allocator<T>{};
    }

    template<typename T> 
    auto SingletonConfiguration::get_constructor_parameters(SingletonAlloc<T>&){
        // If no constructor arguments are provided through specialization,
        //   just create an empty set of arguments.
        return std::tuple{};
    }

    //------------------------------------------------------------------------------------------------------------------------
    template<typename T >
    struct Singleton  {
        // TODO: wrap a c-style array with an array<T,N>.
        static_assert(!std::is_array_v<T>, "C-style arrays cannot be used as singletons.");
    public:
        static T& instance();

    private:
        using Alloc = decltype(SingletonConfiguration::get_allocator<T>());
        
        static std::once_flag once;
        static Alloc allocator;
        
        // Will be called by a std::unique_ptr managing the object instance
        //   after main exits.
        static void deleter(T* ptr) {

            // Don't call destructor if pointer is null.
            if (!ptr) { return; }

            // Because we called placement new, we have to call the destructor manually.
            ptr->~T();

            // And now we have to give back the bytes manually.
            // Note: Depending on the allocator, the bytes may not actually be returned.
            std::allocator_traits<Alloc>::deallocate(allocator,ptr,1);
        }

        using InstancePtr = std::unique_ptr<T,void(*)(T*)>;
        static InstancePtr single_instance_ptr;

        template<typename ...A>
        static void instantiate_object(A&&... args) {

            // A pass through to the underlying memory resource, 
            //   if the allocator is an std::polymorphic_allocator<T>.
            void* raw_bytes = std::allocator_traits<Alloc>::allocate(allocator,1);
            single_instance_ptr.reset(new (raw_bytes) T(args...));
            
        }

        // If the type T is not an allocator-aware type, or the allocator of the type
        //  is not compatible with the allocator of the singleton, just pass the configured args.
        template<typename ...Args>
        static auto add_allocator_to_parameters() {
            // Does not modify the configured arguments.
            return SingletonConfiguration::get_constructor_parameters<T>(allocator);
        } 

        // If the type is an allocator-aware type with an appropriate constructor,
        //   modify the argument list to accomodate the allocator and/or allocator tag.
        template<typename ...Args>
        requires ((DefinesAllocatorType<T>) 
                    && (CopyConstructible<Alloc,typename T::allocator_type>))
        static auto add_allocator_to_parameters() {

            // If the type T has an allocator_type alias declared, try to
            //   pass an appropriately constructed allocator to the instance of T.        

            
            // T(args..., Alloc())
            if constexpr (HasTrailingAllocatorConstructor<T,Alloc,Args...>) {
                // Piece-wise construct the argument list to add an allocator.
                return std::tuple_cat(
                    SingletonConfiguration::get_constructor_parameters<T>(allocator),
                    std::tuple{ allocator }
                );
            }

            // T(std::allocator_arg, Alloc(), args...)
            else if constexpr (HasTaggedAllocatorConstructor<T,Alloc,Args...>) {
                // Piece-wise construct the argument list to add an allocator_arg tag and an allocator.
                return std::tuple_cat(
                    std::tuple{
                        std::allocator_arg,
                        allocator
                    }, 
                    SingletonConfiguration::get_constructor_parameters(allocator)
                );
            }  
    
        };

    };

    template<typename T>
    T& Singleton<T>::instance() {

        // Ensure that the initialization of the T object only happens once.
        std::call_once(once,[&] {
            std::apply(
                // The constructor of T is private, we have to call it from our class, 
                //   which is a friend.
                [&](auto&& ... args) {    
                    instantiate_object(std::forward<decltype(args)>(args)...);
                },

                // Decorate the stored constructor parameters with an allocator and/or tag if
                //   if necessary.
                add_allocator_to_parameters()
            );
        });
        return *(single_instance_ptr);

    }

    template<typename T> 
    typename Singleton<T>::InstancePtr
    Singleton<T>::single_instance_ptr {nullptr,&Singleton<T>::deleter};

    template<typename T> 
    std::once_flag 
    Singleton<T>::once;

    template<typename T>
    typename Singleton<T>::Alloc 
    Singleton<T>::allocator { SingletonConfiguration::get_allocator<T>() };

}
