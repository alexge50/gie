import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import logo from './logo.svg';
import './App.css';

import {NodeEditor, NodeEditorProps} from './NodeEditor/NodeEditor';
import GoldenLayout, {ContentItem} from "golden-layout";
import jQuery from 'jquery';
import 'golden-layout/src/css/goldenlayout-base.css';
import 'golden-layout/src/css/goldenlayout-dark-theme.css';

//import {HelloWorld} from '../build/Release/gie_nodejs.node';

import 'gie';

(window as any).React = React;
(window as any).ReactDOM = ReactDOM;

class App extends Component {
    layout: GoldenLayout;
    root: any;

    componentWillMount() {
        this.layout = new GoldenLayout({
            content: [{
                type: 'row',
                content:[{
                    type: 'column',
                    content:[{
                        type:'react-component',
                        component: 'NodeEditor',
                        id: 'NodeEditor',
                        props: {}
                    }]
                }]
            }]
        });

        this.layout.registerComponent( 'NodeEditor', NodeEditor);

        this.layout.on('initialised', x => {
            let nodeTypes = {
                'Image Add': {'arguments': [{name: 'a', type: 'Image'}, {name: 'b', type: 'Image'}], 'result': 'Number'},
                'Number Add': {'arguments': [{name: '1', type: 'Number'}, {name: '2', type: 'Number'}], 'result': 'Number'},
            };

            let node = this.layout.root.getItemsById('NodeEditor')[0];

            node.parent.replaceChild(
                node,
                {
                type:'react-component',
                component: 'NodeEditor',
                id: 'NodeEditor',
                props: {
                    nodeTypes: nodeTypes,
                    nodeAddedCallback: this.onNodeAdded.bind(this),
                    nodeRemovedCallback: this.onNodeRemoved.bind(this),
                    nodeChangedCallback: this.onNodeChanged.bind(this)
                }
            });
        });

        this.layout.init();
    }

    onNodeAdded(node) {
        console.log('node added');
        console.log(node);
    }

    onNodeRemoved(node) {
        console.log('node removed');
        console.log(node);
    }

    onNodeChanged(node) {
        console.log('node changed');
        console.log(node);
    }

    render() {
        return (
          <div className='App' />
        );
    }
}

export default App;