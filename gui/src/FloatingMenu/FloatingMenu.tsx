import * as React from "react";
import './FloatingMenu.css';

export class FloatingMenuProps {
    x: Number;
    y: Number;
}

export class FloatingMenu extends React.Component<FloatingMenuProps, any>{


    constructor(props) {
        super(props, 'FloatingMenu');
    }

    componentDidMount() {

    }

    render() {
        const style = {
          top: this.props.y.toString() + 'px',
          left: this.props.x.toString() + 'px'
        };
        return (
            <div
                className={'floating-menu'}
                style={style}
            >
                <p>Option 1</p>
            </div>
        );
    }
}

export default FloatingMenu;