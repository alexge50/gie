import * as React from 'react';
import {NodePortModel} from "./PortModel";
import {Node} from "./Node";
import {NodePortWidget} from "./PortWidget";

export class NodeWidgetProps {
    node = null;
}

export class NodeWidgetState {}

export class NodeWidget extends React.Component<NodeWidgetProps, NodeWidgetState> {
    static defaultProps = new NodeWidgetProps();
    static nodeWidth = 120;

    constructor(props){
        super(props);

        this.state = {};
    }

    render() {
        let inputPorts = [];
        let outputPorts = [];
        let props = this.props;
        let i = 0;
        Object.values(this.props.node.getPorts()).forEach(function(port) {
            let position = (port as NodePortModel).position;
            if(position == 'output') {
                outputPorts.push(<NodePortWidget key={i++} label={(port as NodePortModel).label} name={(port as NodePortModel).name} valueType={(port as NodePortModel).valueType}  node={props.node} position={position}/>);
            } else {
                inputPorts.push(<NodePortWidget key={i++} label={(port as NodePortModel).label} name={(port as NodePortModel).name} valueType={(port as NodePortModel).valueType}  node={props.node} position={position}/>);
            }
        });

        return (
            <div
                className={'node'}
                style = {{
                    position: 'relative',
                    width: NodeWidget.nodeWidth,
                    height: 'auto',
                    color: 'yellow',
                }} >
                <div className='node-title'>
                </div>
                <div className='node-output-area'>
                    {outputPorts}
                </div>
                <div className='node-input-area'>
                    {inputPorts}
                </div>
            </div>
        );
    }
}