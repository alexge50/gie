//
// Created by alex on 7/14/19.
//

#ifndef GIE_EXPECTED_H
#define GIE_EXPECTED_H

#include <variant>
#include <utility>
#include <exception>

class bad_expected_access: std::exception
{
public:
    const char* what() const noexcept override
    {
        return "bad_expected_access";
    }
};

template <typename ErrorType>
class Unexpected
{
public:
    ErrorType error;

    ErrorType* operator->() { return error; }
    const ErrorType* operator->() const { return error; }
};

template <typename T>
auto makeUnexpected(T&& t)
{
    return Unexpected<T>{std::forward<T>(t)};
}

template <typename ExpectedType, typename ErrorType>
class Expected
{
public:
    explicit Expected(): m_value{ExpectedType{}} {}
    explicit Expected(ExpectedType value): m_value{std::move(value)} {}
    explicit Expected(Unexpected<ErrorType> error): m_value{{std::move(error)}} {}

    void swap(Expected<ExpectedType, ErrorType>& other) noexcept
    {
        std::swap(m_value, other.m_value);
    }

    const ExpectedType* operator->() const
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    ExpectedType* operator->()
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    ExpectedType& operator*()
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    const ExpectedType& operator*() const
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    explicit operator bool() const noexcept { return std::holds_alternative<ExpectedType>(m_value); }

    bool hasValue() const { return std::holds_alternative<ExpectedType>(m_value); }

    ExpectedType& value()
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    const ExpectedType& value() const
    {
        if(*this)
            return &std::get<ExpectedType>(m_value);
        else throwUnhandled();
    }

    ErrorType& error()
    {
        if(!*this)
            return &std::get<ErrorType>(m_value);
        else throwUnhandled();
    }

    const ErrorType& error() const
    {
        if(!*this)
            return &std::get<ErrorType>(m_value);
        else throwUnhandled();
    }

    ExpectedType valueOr(ExpectedType&& value)
    {
        if(*this)
            return std::get<ErrorType>(m_value);
        else return std::move(value);
    }

    ExpectedType valueOr(ExpectedType&& value) const
    {
        if(*this)
            return std::get<ErrorType>(m_value);
        else return std::move(value);
    }

private:
    [[noreturn]]
    void throwUnhandled()
    {
        throw bad_expected_access{};
    }

private:
    std::variant<ExpectedType, Unexpected<ErrorType>> m_value;
};

#endif //GIE_EXPECTED_H
