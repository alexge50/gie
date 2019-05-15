import {
    DiagramEngine,
    DiagramModel,
    DefaultNodeModel,
    LinkModel,
    DiagramWidget,
    DefaultLinkModel, PortModel
} from "storm-react-diagrams2";
import * as React from "react";

import KeyHandler, {KEYPRESS} from 'react-key-handler';

import {NodeFactory} from "./Node/NodeFactory";
import {Node} from "./Node/Node";

import './NodeEditor.css'
import {PortFactory} from "./Node/PortFactory";
import {NodePortModel} from "./Node/PortModel";
import FloatingMenu from "../FloatingMenu/FloatingMenu";

require("storm-react-diagrams2/src/sass/main.scss");


export class NodeEditorState {
    showMenu: boolean;
}

export class NodeEditorProps {
    nodeTypes: any;

    nodeAddedCallback: any;
    nodeRemovedCallback: any;
    nodeChangedCallback: any;
}

export class NodeData {
    id: string;
    input: [string, string];
}

function convertToNodeData(node) {
    let f = p => {
        let l = Object.values((p as PortModel).getLinks())[0];

        if(l == undefined)
            return undefined;

        if(l.targetPort != p)
            return l.targetPort.getNode().id;
        return l.sourcePort.getNode().id;
    };

    return {
        id: node.id,
        input: node.getInPorts().map(port => ([port.label, f(port)]))
    };
}

export class NodeEditor extends React.Component<NodeEditorProps, NodeEditorState>{
    engine: DiagramEngine;
    model: DiagramModel;
    nodeTypes: any;
    badLink = new Set<string>();

    constructor(props) {
        super(props, 'NodeEditor');

        this.engine = new DiagramEngine();
        this.model = new DiagramModel();

        this.engine.installDefaultFactories();
        this.engine.setDiagramModel(this.model);
        this.engine.registerNodeFactory(new NodeFactory());
        this.engine.registerPortFactory(new PortFactory('Node', config => new NodePortModel('', '', 'input')));

        this.state = {
            showMenu : false,
        };

        if(props.nodeTypes != null)
            this.nodeTypes = props.nodeTypes;
        else this.nodeTypes = [];
    }

    componentDidMount() {
        let model = this.model;

        let notifyNodeChanged = link => {
            let f = port => {
                if((port as NodePortModel).position == 'input')
                    this.props.nodeChangedCallback(convertToNodeData(port.parent));
            };

            f(link.sourcePort);
            f(link.targetPort);
        };

        this.model.addListener({
           linksUpdated: event => {
               let link = event.link;

               let f = event => {
                   try {
                       if((link.sourcePort as NodePortModel).valueType != (link.targetPort as NodePortModel).valueType &&
                           (link.sourcePort as NodePortModel).position != (link.targetPort as NodePortModel).position) {
                           model.removeLink(link);
                           this.badLink.add(link.id);
                       } else {
                           notifyNodeChanged(link);
                       }
                   }
                   catch(e) { }
               };

               event.link.addListener({
                   sourcePortChanged: f,
                   targetPortChanged: f
               });

               if(!event.isCreated) {
                   if(!this.badLink.has(link.id))
                       notifyNodeChanged(link);
                   else this.badLink.delete(link.id);
               }
           }
        });

        this.model.addListener({
           nodesUpdated: event => {
               if(event.isCreated)
                   this.props.nodeAddedCallback(convertToNodeData(event.node));
               else this.props.nodeRemovedCallback(convertToNodeData(event.node));
           }
        });
    }

    onKeyDown(event) {
        if(event.keyCode === 32)
        {
            let state: NodeEditorState = this.state;
            state.showMenu = !state.showMenu;

            this.setState(state);
        }
    };

    onMouseDown(event) {
        setTimeout(() => {
            let state: NodeEditorState = this.state;
            state.showMenu = false;
            this.setState(state)
        }, 100);
    }

    createNode(name) {
        this.model.addNode(new Node(this.nodeTypes[name]['arguments'], this.nodeTypes[name]['result']))
    }

    render() {
        return (
            <div
                className={'node-editor'}
                onKeyDown={this.onKeyDown.bind(this)}
                onMouseDown={this.onMouseDown.bind(this)}
                tabIndex={0 }
            >
                <DiagramWidget className="node-editor" diagramEngine={this.engine} />

                    <FloatingMenu
                        triggered={this.state.showMenu}
                        options={Object.keys(this.nodeTypes)}
                        callback={this.createNode.bind(this)}
                    />

            </div>
        );
    }
}

export default NodeEditor;
