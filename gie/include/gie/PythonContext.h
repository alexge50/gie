//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_PYTHONCONTEXT_H
#define GIE_LIBRARY_PYTHONCONTEXT_H

#include <boost/python.hpp>
#include <unordered_map>
#include <string>

class PythonContext
{
public:
    PythonContext();

    boost::python::object module(const std::string &);
    boost::python::object getFunction(const std::string &) const;

private:
    boost::python::object main;
    boost::python::object global;

    std::unordered_map<std::string, boost::python::object> importedModules;
};

#endif //GIE_LIBRARY_PYTHONCONTEXT_H
