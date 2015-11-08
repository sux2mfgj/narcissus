#pragma once

#include <stdexcept>
#include <iostream>

namespace narcissus {
    namespace cpu {
        
        class invalid_operation : public std::logic_error
        {

            public:
                invalid_operation() : std::logic_error("invalid operation") {}
                invalid_operation(const char* what_arg): std::logic_error(what_arg){}
        };
    } // namespace cpu
} // namespace narcissus

