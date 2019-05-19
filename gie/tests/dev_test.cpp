//
// Created by alex on 2/2/19.
//

#include <gie/Node.h>
#include <gie/Program.h>
#include <gie/NodeUtil.h>

#include <boost/python.hpp>

#include <cassert>
#include <vector>

#include <iostream>

int main()
{

    Program program;

    program.context().module("builtins", false);

    auto sys = program.context().module("sys", false);
    auto os = program.context().module("os", false);

    sys.attr("path").attr("insert")(1, os.attr("getcwd")());

    program.import("test_modules.basic");

    for(const auto& s: program.context().importedSymbols())
    {
        std::cout << s << std::endl;
        auto x = fetchMetadata(program.context(), s);
    }
}
