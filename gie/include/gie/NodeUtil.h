//
// Created by alex on 4/27/19.
//

#ifndef GIE_LIBRARY_NODEUTIL_H
#define GIE_LIBRARY_NODEUTIL_H

#include <gie/Node.h>
#include <gie/NodeLogic.h>
#include <gie/NodeMetadata.h>
#include <gie/NodeDrawable.h>

#include <gie/PythonContext.h>

NodeMetadata fetchMetadata(PythonContext&, std::string qualifiedFunctionName);
Node makeNode(PythonContext&, std::string name, std::vector<ArgumentValue>);

#endif //GIE_LIBRARY_NODEUTIL_H
