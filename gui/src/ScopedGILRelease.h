//
// Created by alex on 5/28/19.
//

#ifndef GUI_SCOPEDGILRELEASE_H
#define GUI_SCOPEDGILRELEASE_H

#include <boost/python.hpp>

class ScopedGILRelease
{
public:
    inline ScopedGILRelease() { m_state = PyEval_SaveThread(); }
    inline ~ScopedGILRelease() { PyEval_RestoreThread(m_state); }
private:
    PyThreadState *m_state;
};

#endif //GUI_SCOPEDGILRELEASE_H
