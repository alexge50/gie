import {AbstractNodeFactory, DiagramEngine, NodeModel} from 'storm-react-diagrams2';
import {NodeWidget} from "./NodeWidget";
import {Node} from "./Node";
import React from 'react'

export class NodeFactory extends AbstractNodeFactory {
    constructor() {
        super('Node');
    }

    generateReactWidget(diagramEngine: DiagramEngine, node: NodeModel): JSX.Element {
        return <NodeWidget node={node}/>;
    }

    getNewInstance(config) {
        return new Node([], '');
    }
}