//
// Created by alex on 5/15/19.
//

#include <string>
#include <iostream>

#include "nbind/nbind.h"

struct Greeter {
    static void sayHello(
            std::string name
    ) {
        std::cout
                << "Hello, "
                << name << "!\n";
    }
};

NBIND_CLASS(Greeter) {
        method(sayHello);
}