//
// Created by alex on 7/14/19.
//

#ifndef GIE_MAYBEERROR_H
#define GIE_MAYBEERROR_H

#include <optional>
#include <utility>
#include <exception>

class maybe_error_bad_access: std::exception
{
public:
    const char* what() const noexcept override
    {
        return "maybe_error_bad_access";
    }
};

template<typename ErrorType>
class [[nodiscard]] MaybeError
{
public:
    MaybeError(): m_error{std::nullopt} {}
    MaybeError(ErrorType error): m_error{std::move(error)} {}

    operator bool() const { return m_error.has_value(); }
    bool errorSet() const { return m_error.has_value(); }

    [[nodiscard]] ErrorType& error()
    {
        if(!errorSet())
            return m_error.value();
        else throw maybe_error_bad_access{};
    }


    [[nodiscard]] const ErrorType& error() const
    {
        if(!errorSet())
            return m_error.value();
        else throw maybe_error_bad_access{};
    }

    template <typename Handler>
    void handle(Handler&& handler)
    {
        if(errorSet())
            handler(m_error);
    }

    void discard() { }

private:
    std::optional<ErrorType> m_error;
};


#endif //GIE_MAYBEERROR_H
