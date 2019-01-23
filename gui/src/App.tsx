import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';

import {NodeEditor} from './NodeEditor/NodeEditor';

class App extends Component {
    componentDidMount() {
    }

    render() {
      return (
        <div className="App">
            <NodeEditor/>
        </div>
    );
  }
}

export default App;