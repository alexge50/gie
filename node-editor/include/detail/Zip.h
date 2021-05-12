#ifndef NODE_EDITOR_ZIP_H
#define NODE_EDITOR_ZIP_H

#include <tuple>
#include <functional>


template<typename... ContainerTs>
struct ConstZipObject
{

    struct Iterator
    {
        std::tuple<typename ContainerTs::const_iterator...> iterators;

        Iterator& operator++()
        {
            (std::get<typename ContainerTs::const_iterator>(iterators)++, ...);

            return *this;
        }

        bool operator!=(const Iterator& other) const
        {
            static auto compare = [](const auto& a, const auto& b) {
                return a != b;
            };

            return (compare(
                    std::get<typename ContainerTs::const_iterator>(iterators),
                    std::get<typename ContainerTs::const_iterator>(other.iterators)
                   ) && ...);
        }

        auto operator*()
        {
            return std::make_tuple(std::reference_wrapper(*std::get<typename ContainerTs::const_iterator>(iterators))...);
        }
    };

    std::tuple<const ContainerTs*...> containers;

    ConstZipObject(const ContainerTs&... containers_):
        containers{std::make_tuple(&containers_ ...)}
    {}

    Iterator begin()
    {
        return Iterator{{std::get<const ContainerTs*>(containers)->cbegin() ...}};
    }

    Iterator end()
    {
        return Iterator{{std::get<const ContainerTs*>(containers)->cend() ...}};
    }
};

#endif //NODE_EDITOR_ZIP_H
