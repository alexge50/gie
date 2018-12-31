//
// Created by alex on 12/29/18.
//

#ifndef GIE_LIBRARY_PYTHONCONTEXT_H
#define GIE_LIBRARY_PYTHONCONTEXT_H

#include <boost/python.hpp>

class PythonContext
{
public:
    PythonContext();

    void importModule(const std::string &);
    boost::python::object getFunction(const std::string &) const;

private:
    boost::python::object main;
    boost::python::object global;

    std::vector<boost::python::object> importedModules;
};

#endif //GIE_LIBRARY_PYTHONCONTEXT_H
