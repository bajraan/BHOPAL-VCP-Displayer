# pragma once

//
// header only
//

#include <iostream>


class PreTest_compiler {

    void print_version()
    {
        #if defined(__clang__)
          std::cout << "Clang version: "
                    << __clang_major__ << "."
                    << __clang_minor__ << "."
                    << __clang_patchlevel__ << std::endl;

        #elif defined(__GNUC__)
          std::cout << "GCC version: "
                    << __GNUC__ << "."
                    << __GNUC_MINOR__ << "."
                    << __GNUC_PATCHLEVEL__ << std::endl;

        #elif defined(_MSC_VER)
            std::cout << "MSVC version: ";
            std::cout << _MSC_VER;
            if (_MSC_FULL_VER > 100000)
            std::cout << " (full: " << _MSC_FULL_VER << ")";
            std::cout << std::endl;

        #elif defined(__INTEL_COMPILER)
            std::cout << "Intel C++ Compiler version: " << __INTEL_COMPILER << std::endl;

        #else
            std::cout << "Unknown or unsupported compiler" << std::endl;

        #endif
    }

public:
    PreTest_compiler(void) { print_version(); }
};