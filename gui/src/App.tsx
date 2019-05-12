import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import logo from './logo.svg';
import './App.css';

import {NodeEditor} from './NodeEditor/NodeEditor';
import GoldenLayout from "golden-layout";
import 'golden-layout/src/css/goldenlayout-base.css';
import 'golden-layout/src/css/goldenlayout-dark-theme.css';

(window as any).React = React;
(window as any).ReactDOM = ReactDOM;

class App extends Component {
    componentDidMount() {
        let myLayout = new GoldenLayout({
            content: [{
                type: 'row',
                content:[{
                    type: 'column',
                    content:[{
                        type:'react-component',
                        component: 'NodeEditor',
                        props: {}
                    }]
                }]
            }]
        });

        myLayout.registerComponent( 'NodeEditor', NodeEditor);

        myLayout.init();
    }

    /*
    <div className="App">
            <NodeEditor/>
        </div>
     */

    render() {
      return (
          <div className='goldenLayout' />
      );
  }
}

export default App;