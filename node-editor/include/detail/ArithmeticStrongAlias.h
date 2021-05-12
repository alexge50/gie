#ifndef NODE_EDITOR_ARITHMETICSTRONGALIAS_H
#define NODE_EDITOR_ARITHMETICSTRONGALIAS_H

#include <utility>

template <typename T, typename Tag>
class ArithmeticStrongAlias
{
public:
    ArithmeticStrongAlias(const T& value): m_value{value} {};
    ArithmeticStrongAlias(T&& value): m_value{std::move(value)} {};
    explicit ArithmeticStrongAlias(): m_value{} {}

    T& get() { return m_value; }
    const T& get() const { return m_value; }

    explicit operator T() { return m_value; }
    explicit operator T() const { return m_value; }

    auto& operator=(const T& t)
    {
        m_value = t;

        return *this;
    }

    auto& operator=(T&& t)
    {
        m_value = std::move(t);

        return *this;
    }

    template <typename T2>
    auto operator+(const T2& rhs) const
    {
        return m_value + rhs;
    }

    template <typename T2>
    auto operator*(const T2& rhs) const
    {
        return m_value * rhs;
    }

    template <typename T2>
    auto operator-(const T2& rhs) const
    {
        return m_value - rhs;
    }

    template <typename T2>
    auto operator/(const T2& rhs) const
    {
        return m_value / rhs;
    }

    template <typename T2>
    auto& operator-=(const T2& rhs)
    {
        m_value -= rhs;
        return *this;
    }

    template <typename T2>
    auto& operator+=(const T2& rhs)
    {
        m_value += rhs;
        return *this;
    }

    template <typename T2>
    auto& operator/=(const T2& rhs)
    {
        m_value /= rhs;
        return *this;
    }

    template <typename T2>
    auto& operator*=(const T2& rhs)
    {
        m_value *= rhs;
        return *this;
    }

private:
    T m_value;
};

#endif //NODE_EDITOR_ARITHMETICSTRONGALIAS_H
