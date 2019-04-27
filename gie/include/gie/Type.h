//
// Created by alex on 4/26/19.
//

#ifndef GIE_LIBRARY_TYPE_H
#define GIE_LIBRARY_TYPE_H

#include <string>
#include <functional>

class Type
{
    static std::hash<std::string> Hasher;
public:
    Type(): m_hash{0} {}
    explicit Type(std::string name):
        m_hash{Hasher(name)},
        m_name{std::move(name)}{}

    bool operator==(const Type& other)
    {
        if(m_hash != other.m_hash)
            return false;
        return m_name == other.m_name;
    }

    bool operator!=(const Type& other)
    {
        return !(*this == other);
    }

private:
    std::size_t m_hash;
    std::string m_name;
};

#endif //GIE_LIBRARY_TYPE_H
