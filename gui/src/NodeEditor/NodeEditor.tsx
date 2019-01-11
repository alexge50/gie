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

import './NodeEditor.css'
import {PortFactory} from "./Node/PortFactory";
import {NodePortModel} from "./Node/PortModel";

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
        this.engine.registerPortFactory(new PortFactory('Node', config => new NodePortModel('', '', 'input')))
    }

    componentDidMount() {
        let node1 = new DefaultNodeModel("Node 1", "rgb(0,192,255)");
        let port1 = node1.addOutPort("Out");
        node1.setPosition(100, 100);

        let node2 = new DefaultNodeModel("Node 2", "rgb(192,255,0)");
        let port2 = node2.addInPort("In");
        node2.setPosition(400, 100);

        let node3 = new Node([{name: '1', type: 'Number'}, {name: '2', type: 'Number'}], 'Number');
        let node4 = new Node([{name: 'a', type: 'Image'}, {name: 'b', type: 'Image'}], 'Number');

        let link1 = port1.link(port2);
        (link1 as DefaultLinkModel).addLabel("Hello World!");

        this.model.addAll(node1, node2, node3, node4, link1);

        let model = this.model;
        this.model.addListener({
           linksUpdated: event => {
               let link = event.link;
               event.link.addListener({
                  sourcePortChanged: event => {
                      try {
                          console.log(event);
                          if((link.sourcePort as NodePortModel).valueType != (link.targetPort as NodePortModel).valueType &&
                              (link.sourcePort as NodePortModel).position != (link.targetPort as NodePortModel).position)
                              model.removeLink(link);
                      }
                      catch(e) { }
                  },
                  targetPortChanged: event => {
                      try {
                          console.log(event);
                          if((link.sourcePort as NodePortModel).valueType != (link.targetPort as NodePortModel).valueType &&
                              (link.sourcePort as NodePortModel).position != (link.targetPort as NodePortModel).position)
                              model.removeLink(link);
                      }
                      catch(e) { }
                  }
               });
           }
        });
    }

    render() {
        return <DiagramWidget className="node-editor" diagramEngine={this.engine} />;
    }
}

export default NodeEditor;
