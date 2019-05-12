import * as React from "react";
import ReactDOM from 'react-dom';
import './FloatingMenu.css';

export class FloatingMenuProps {
    triggered: boolean;
    options: string[];
    callback: any;
}

export class FloatingMenuState {
    parent: any;

    positionX: Number;
    positionY: Number;
}

export class FloatingMenu extends React.Component<FloatingMenuProps, FloatingMenuState>{

    state = {
        parent: null,
        positionX: 0,
        positionY: 0
    };

    constructor(props) {
        super(props, 'FloatingMenu');

        this.state = {
            parent: null,
            positionX: 0,
            positionY: 0
        };

        console.log(this.state);
    }

    componentDidMount() {
        document.addEventListener('mousemove', this.onMouseMove.bind(this), false);

        this.setState({
            parent: ReactDOM.findDOMNode(this).parentNode,
            positionX: 0,
            positionY: 0
        });

        console.log(this.state);
    }

    componentWillUnmount() {
        document.removeEventListener('mousemove', this.onMouseMove, false);
    }

    onMouseMove(event) {
        let state = this.state;
        if(this.props == null || !this.props.triggered)
        {
            let rect = state.parent.getBoundingClientRect();
            state.positionX = event.clientX - rect.x;
            state.positionY = event.clientY - rect.y ;

            this.setState(state);
         }
    }

    render() {
        const style = {
          top: this.state.positionY.toString() + 'px',
          left: this.state.positionX.toString() + 'px'
        };

        const props = this.props;
        const options = this.props.options.map(item => <p onClick={() => props.callback(item)}>{item}</p>);

        if(props.triggered)
        {
            return (
                <div
                    className={'floating-menu'}
                    style={style}
                >
                    {options}
                </div>
            );
        }
        return <div></div>;
    }
}

export default FloatingMenu;