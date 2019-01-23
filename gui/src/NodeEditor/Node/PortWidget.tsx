import { NodeModel } from 'storm-react-diagrams2';
import { BaseWidget, IBaseWidgetProps } from 'storm-react-diagrams2';
import React, { Component } from 'react';

export interface PortProps extends IBaseWidgetProps {
    name: string;
    node: NodeModel;
    label: string;
    valueType: string;
    position: 'input' | 'output';
}

export interface PortState {
    selected: boolean;
}

export class NodePortWidget extends BaseWidget<PortProps, PortState> {
    constructor(props: PortProps) {
        super("Node", props);
        this.state = {
            selected: false
        };
    }

    getClassName() {
        return "port " + super.getClassName() + (this.state.selected ? this.bem("--selected") : "");
    }

    render() {
        if(this.props.position == 'input')
        {
            return (
                <div className='port-line'>
                    <div
                        {...this.getProps()}
                        onMouseEnter={() => {
                            this.setState({ selected: true });
                        }}
                        onMouseLeave={() => {
                            this.setState({ selected: false });
                        }}
                        data-name={this.props.name}
                        data-nodeid={this.props.node.getID()}
                    />
                    <div className='port-tag'>
                        {this.props.label}
                    </div>
                </div>
            );
        } else {
            return (
                <div className='port-line'>
                    <div className='port-tag'>
                        {this.props.label}
                    </div>
                    <div
                        {...this.getProps()}
                        onMouseEnter={() => {
                            this.setState({ selected: true });
                        }}
                        onMouseLeave={() => {
                            this.setState({ selected: false });
                        }}
                        data-name={this.props.name}
                        data-nodeid={this.props.node.getID()}
                    />
                </div>
            );
        }

    }
}