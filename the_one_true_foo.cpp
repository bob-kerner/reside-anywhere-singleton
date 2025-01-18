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

#include "the_one_true_foo.hpp"
#include <iostream>
#include <format>

void TheOneTrueFoo::boop() { 
    std::cout<<std::format("Booping a foo [i={}, d={}].\n",i,d);
}

TheOneTrueFoo::~TheOneTrueFoo() { 
    std::cout<<"Foo destructor.\n"; 
}

TheOneTrueFoo::TheOneTrueFoo(int i, double d) : d(d), i(i) { 
    std::cout<<"Foo constructor.\n"; 
}
 
