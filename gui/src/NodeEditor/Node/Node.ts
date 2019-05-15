import {NodeModel} from "storm-react-diagrams2"
import {NodePortModel} from "./PortModel";

export class Node extends NodeModel {
    constructor(nodeArguments, result) {
        super("Node");

        let node = this;
        nodeArguments.forEach(function (argument) {
            node.addPort(new NodePortModel(argument.name, argument.type, 'input'));
        });

        node.addPort(new NodePortModel('Result', result, 'output'))
    }

    getInPorts() {
        return Object.values(this.ports).filter(p => (p as NodePortModel).position == 'input');
    }

    getOutPorts() {
        return Object.values(this.ports).filter(p => (p as NodePortModel).position == 'output');
    }
}