import {
    DiagramEngine,
    DiagramModel,
    DefaultNodeModel,
    LinkModel,
    DiagramWidget,
    DefaultLinkModel
} from "storm-react-diagrams2";
import * as React from "react";

require("storm-react-diagrams2/src/sass/main.scss");

import {NodeFactory} from "./Node/NodeFactory";
import {Node} from "./Node/Node";

export class NodeEditor extends React.Component<any, any>{
    engine: DiagramEngine;
    model: DiagramModel;

    constructor() {
        super('NodeEditor', {});

        this.engine = new DiagramEngine();
        this.model = new DiagramModel();

        this.engine.installDefaultFactories();
        this.engine.setDiagramModel(this.model);
        this.engine.registerNodeFactory(new NodeFactory());
    }

    componentDidMount() {
        let node1 = new DefaultNodeModel("Node 1", "rgb(0,192,255)");
        let port1 = node1.addOutPort("Out");
        node1.setPosition(100, 100);

        let node2 = new DefaultNodeModel("Node 2", "rgb(192,255,0)");
        let port2 = node2.addInPort("In");
        node2.setPosition(400, 100);

        let node3 = new Node([{name: '1', type: 'Number'}, {name: '2', type: 'Number'}], 'Number');

        let link1 = port1.link(port2);
        (link1 as DefaultLinkModel).addLabel("Hello World!");

        this.model.addAll(node1, node2, node3, link1);
    }

    render() {
        return <DiagramWidget className="srd-demo-canvas" diagramEngine={this.engine} />;
    }
}

export default NodeEditor;
