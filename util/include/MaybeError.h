//
// Created by alex on 7/14/19.
//

#ifndef GIE_MAYBEERROR_H
#define GIE_MAYBEERROR_H

#include <optional>
#include <utility>
#include <exception>

class unhandled_error: std::exception
{
public:
    const char* what() const noexcept override
    {
        return "unhandled_error";
    }
};

template<typename ErrorType>
class MaybeError
{
public:
    MaybeError(): m_error{std::nullopt} {}
    MaybeError(ErrorType error): m_error{std::move(error)} {}

    ~MaybeError() { throw unhandled_error{}; }

    operator bool() const { return m_error.has_value(); }
    bool errorSet() const { return m_error.has_value(); }

    ErrorType& error() { return m_error.value(); }
    const ErrorType& error() const { return m_error.value(); }

private:
    std::optional<ErrorType> m_error;
};


#endif //GIE_MAYBEERROR_H
