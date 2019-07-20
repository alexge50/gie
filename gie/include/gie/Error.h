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

    explicit ExecutionInterfaceError(errors error, NodeId id, std::optional<std::string> detail = std::nullopt):
        m_error{error},
        m_id{id},
        m_detail{std::move(detail)}
    {}

    std::string what()
    {
        static const char* names[] = {"PythonInternalError", "InvalidArguments"};
        std::string buffer = names[static_cast<int>(error())];
        buffer +=  "(";
        buffer += std::to_string(id().get());
        buffer += ")";

        if(m_detail)
        {
            buffer += ": ";
            buffer += m_detail.value();
        }

        return buffer;
    }

    NodeId id() const { return m_id; }
    errors error() const { return m_error; }
    const std::optional<std::string>& detail() const { return m_detail; }

private:
    errors m_error;
    NodeId m_id;
    std::optional<std::string> m_detail;
};

#endif //GIE_ERROR_H
