import * as _ from "lodash";
import {LinkModel, PortModel, DefaultLinkModel, Toolkit} from 'storm-react-diagrams2';

export class NodePortModel extends PortModel {
    label: string;
    valueType: string;
    position: 'input' | 'output';

    constructor(label: string, valueType: string, position: 'input' | 'output') {
       super(Toolkit.UID(), 'Node');
       this.label = label;
       this.valueType = valueType;
       this.position = position
    }

    serialize() {
        return _.merge(super.serialize(), {
            portName: this.label,
           type: this.valueType
        });
    }

    deSerialize(data, engine) {
        super.deSerialize(data, engine);
        this.label = data.label;
        this.position = data.position;
    }

    createLinkModel(): LinkModel {
        return new DefaultLinkModel();
    }
}