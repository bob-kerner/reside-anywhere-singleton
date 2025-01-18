# //-------------------------------------------------------------------------------------------------------------
# // Copyright Bob Kerner, 2025. (727) 560 - 0408. rekerner-at-gmail.com. https://www.linkedin.com/in/bkerner/
# // Original Github: 
# //-------------------------------------------------------------------------------------------------------------
# // Custom MIT License with Attribution Requirement

# // Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
# //   documentation files (the "Software"), to deal in the Software without restriction, including without 
# //   limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of 
# //   the Software, and to permit persons to whom the Software is furnished to do so, subject to the following 
# //   conditions:
# //   
# //   -- The above copyright notice and this permission notice shall be included in all copies or substantial 
# //         portions of the Software.
# //
# //   -- Any modifications to the Software, including derivative works, must include clear attribution to the 
# //         original author, as well as a link to the original repository or source, located at: 
# //         [Insert URL Here].

# // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
# //   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT. IN NO 
# //   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES, OR OTHER LIABILITY, WHETHER 
# //   IN AN ACTION OF CONTRACT, TORT, OR OTHERWISE, ARISING FROM, OUT OF, OR IN CONNECTION WITH THE SOFTWARE OR 
# //   THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# //-------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.28)
project(singleton)

set_property(GLOBAL PROPERTY CXX_STANDARD 20)
add_executable(singleton main.cpp logging_resource.cpp the_one_true_foo.cpp)

target_include_directories(singleton PUBLIC .)
target_compile_options(singleton PUBLIC -std=c++23 -Wall)
 
