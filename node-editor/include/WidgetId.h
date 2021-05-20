#ifndef GIE_WIDGETID_H
#define GIE_WIDGETID_H

#include <Graph.h>

using WidgetId = std::variant<Port, NodeId, int>;

static bool operator==(const WidgetId& rhs, const WidgetId& lhs)
{
    if(rhs.index() == lhs.index())
    {
        if(rhs.index() == 0)
            return std::get<0>(rhs) == std::get<0>(lhs);
        else if(rhs.index() == 1)
            return std::get<1>(rhs) == std::get<1>(lhs);
        else if(rhs.index() == 2)
            return std::get<2>(rhs) == std::get<2>(lhs);

        return false;
    }
    return false;
}

struct WidgetIdHasher
{
    size_t operator()(const WidgetId& id) const
    {
        size_t r = 0;

        std::hash<int32_t> hasher;
        PortHasher port_hasher{};

        r ^= hasher(static_cast<int>(id.index())) + 0x9e3779b9 + (r << 6) + (r >> 2);

        if(id.index() == 0)
            r ^= hasher(port_hasher(std::get<0>(id))) + 0x9e3779b9 + (r << 6) + (r >> 2);
        else if(id.index() == 1)
            r ^= hasher(hasher(std::get<1>(id))) + 0x9e3779b9 + (r << 6) + (r >> 2);
        else if(id.index() == 2)
            r ^= hasher(hasher(std::get<2>(id))) + 0x9e3779b9 + (r << 6) + (r >> 2);

        return r;
    }
};

#endif //GIE_WIDGETID_H
