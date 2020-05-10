//
// Created by alex on 7/14/19.
//

#ifndef GIE_NODEID_H
#define GIE_NODEID_H

#include <StrongAlias.h>
#include <functional>

using NodeId = StrongAlias<std::size_t, struct NodeIdTag>;

inline bool operator==(const NodeId& lhs, const NodeId& rhs) { return lhs.get() == rhs.get(); }

namespace std
{
    template<> struct hash<NodeId>
    {
        using argument_type = NodeId;
        using result_type = std::size_t;

        result_type operator()(const NodeId& id) const noexcept
        {
            return (std::hash<std::size_t>{})(id.get());
        }
    };
}

using ResourceId = StrongAlias<std::size_t, struct ResourceIdTag>;

inline bool operator==(const ResourceId& lhs, const ResourceId& rhs) { return lhs.get() == rhs.get(); }

namespace std
{
    template<> struct hash<ResourceId>
    {
        using argument_type = NodeId;
        using result_type = std::size_t;

        result_type operator()(const ResourceId& id) const noexcept
        {
            return (std::hash<std::size_t>{})(id.get());
        }
    };
}


#endif //GIE_NODEID_H
