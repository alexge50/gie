import * as React from "react";
import './FloatingMenu.css';

export class FloatingMenuProps {
    x: Number;
    y: Number;

    options: string[];
    callback: any;
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

        const props = this.props;
        const options = this.props.options.map(item => <p onClick={() => props.callback(item)}>{item}</p>);

        return (
            <div
                className={'floating-menu'}
                style={style}
            >
                {options}
            </div>
        );
    }
}

export default FloatingMenu;