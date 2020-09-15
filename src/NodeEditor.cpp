#include <NodeEditor.h>
#include <detail/ComputeNode.h>

void compute(NodeEditor& node_editor)
{
    for(auto& [id, _]: node_editor.graph.nodes)
    {
        compute_node(node_editor, id);
    }
}