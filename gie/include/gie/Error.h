//
// Created by alex on 7/14/19.
//

#ifndef GIE_ERROR_H
#define GIE_ERROR_H

class NodeInterfaceError
{
public:
    enum class errors
    {
        TypeCheckingFailed = 0,
        IncorrectNodeId,
        IncorrectSymbolName
    };

    explicit NodeInterfaceError(errors error): m_error{error} {}

    const char* what()
    {
        static const char* names[] = {"TypeCheckingFailed", "IncorrectNodeId", "IncorrectSymbolName"};

        return names[static_cast<int>(m_error)];
    }

private:
    errors m_error;
};

class ExecutionInterfaceError
{
public:
    enum class errors
    {
        PythonInternalError = 0,
        InvalidArguments
    };

    explicit ExecutionInterfaceError(errors error): m_error{error} {}

    const char* what()
    {
        static const char* names[] = {"PythonInternalError", "InvalidArguments"};

        return names[static_cast<int>(m_error)];
    }

private:
    errors m_error;

};

#endif //GIE_ERROR_H
