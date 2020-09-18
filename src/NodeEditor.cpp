#include <NodeEditor.h>
#include <detail/Compute.h>

void compute(NodeEditor& node_editor)
{
    for(auto& [id, _]: node_editor.graph.node_types)
    {
        compute_type(node_editor, id);
    }

    for(auto& [id, _]: node_editor.graph.nodes)
    {
        compute_node(node_editor, id);
    }

    compute_focus_stack(node_editor);
}