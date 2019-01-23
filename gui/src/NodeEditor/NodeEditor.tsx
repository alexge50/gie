import {
    DiagramEngine,
    DiagramModel,
    DefaultNodeModel,
    LinkModel,
    DiagramWidget,
    DefaultLinkModel
} from "storm-react-diagrams2";
import * as React from "react";

import KeyHandler, {KEYPRESS} from 'react-key-handler';

require("storm-react-diagrams2/src/sass/main.scss");

import {NodeFactory} from "./Node/NodeFactory";
import {Node} from "./Node/Node";

import './NodeEditor.css'
import {PortFactory} from "./Node/PortFactory";
import {NodePortModel} from "./Node/PortModel";
import FloatingMenu from "../FloatingMenu/FloatingMenu";

export class NodeEditorState {
    showMenu: boolean;
    menuX: number;
    menuY: number;

    mouseX: number;
    mouseY: number;
}

export class NodeEditorProps {
}

export class NodeEditor extends React.Component<any, NodeEditorState>{
    engine: DiagramEngine;
    model: DiagramModel;
    nodeTypes: any;

    constructor() {
        super({}, 'NodeEditor');

        this.engine = new DiagramEngine();
        this.model = new DiagramModel();

        this.engine.installDefaultFactories();
        this.engine.setDiagramModel(this.model);
        this.engine.registerNodeFactory(new NodeFactory());
        this.engine.registerPortFactory(new PortFactory('Node', config => new NodePortModel('', '', 'input')))

        this.state = {
            showMenu : false,
            menuX: 0,
            menuY: 0,
            mouseX: 0,
            mouseY: 0};

        this.nodeTypes = {
            'Image Add': {'arguments': [{name: 'a', type: 'Image'}, {name: 'b', type: 'Image'}], 'result': 'Number'},
            'Number Add': {'arguments': [{name: '1', type: 'Number'}, {name: '2', type: 'Number'}], 'result': 'Number'},
        };
    }

    componentDidMount() {

        let node1 = new Node([{name: '1', type: 'Number'}, {name: '2', type: 'Number'}], 'Number');
        let node2 = new Node([{name: 'a', type: 'Image'}, {name: 'b', type: 'Image'}], 'Number');

        this.model.addAll(node1, node2);

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

    onKeyDown(event) {
        if(event.keyCode == 32 && ! this.state.showMenu)
        {
            let state: NodeEditorState = this.state;
            state.showMenu = true;
            state.menuX = state.mouseX;
            state.menuY = state.mouseY;

            this.setState(state);
        }
    };

    onMouseMove(event) {
        let state: NodeEditorState = this.state;
        state.mouseX = event.clientX;
        state.mouseY = event.clientY;

        this.setState(state);
    }

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
                onMouseMove={this.onMouseMove.bind(this)}
                onKeyDown={this.onKeyDown.bind(this)}
                onMouseDown={this.onMouseDown.bind(this)}
                tabIndex={0}
            >
                <DiagramWidget className="node-editor" diagramEngine={this.engine} />

                {this.state.showMenu && (
                    <FloatingMenu
                        x={this.state.menuX}
                        y={this.state.menuY}
                        options={Object.keys(this.nodeTypes)}
                        callback={this.createNode.bind(this)}
                    />
                )}

            </div>
        );
    }
}

export default NodeEditor;
