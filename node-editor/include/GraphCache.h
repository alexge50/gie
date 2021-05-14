#ifndef NODE_EDITOR_GRAPHCACHE_H
#define NODE_EDITOR_GRAPHCACHE_H

#include <glm/vec3.hpp>

#include <optional>
#include <vector>
#include <string_view>

#include <detail/BoundingBox.h>
#include <Camera.h>


struct NodeCache
{
    glm::vec3 position;
    glm::vec3 size;
};

struct NodeHeaderCache
{
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;
    std::string_view name;
};

struct NodeOutlineCache
{
    glm::vec3 position;
    glm::vec3 size;
    bool selected;
};

struct PortCache
{
    glm::vec3 position;
    glm::vec3 color;
};

struct PortOutlineCache
{
    glm::vec3 position;
    bool selected;
};

struct ConnectionCache
{
    glm::vec3 source_position;
    glm::vec3 destination_position;
};

struct SelectBoxCache
{
    BoundingBox box;
};

struct GraphCache
{
    std::vector<NodeCache> nodes;
    std::vector<NodeOutlineCache> node_outlines;
    std::vector<NodeHeaderCache> node_headers;
    std::vector<PortCache> ports;
    std::vector<PortOutlineCache> port_outline;
    std::vector<ConnectionCache> connections;
    std::optional<SelectBoxCache> select_box;

    Camera camera{};
};

struct NodeEditor;
GraphCache compute_graph_cache(const NodeEditor &node_editor);

#endif //NODE_EDITOR_GRAPHCACHE_H
