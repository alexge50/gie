//
// Created by alex on 2/2/19.
//

#include <gie/Program.h>

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

    program.context().module("test_modules.basic", "test_modules/basic");

    for(const auto& symbol: program.context().importedSymbols())
    {
        std::cout << symbol.name.qualifiedName << std::endl;
    }

    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.addNode("test_modules.basic.to_int", {});
    program.removeNode(NodeId{7}).discard();
    program.addNode("test_modules.basic.to_int", {});
}
