import { PortModel, AbstractPortFactory } from "storm-react-diagrams2";

export class PortFactory extends AbstractPortFactory {
    cb: (initialConfig?: any) => PortModel;

    constructor(type: string, cb: (initialConfig?: any) => PortModel) {
        super(type);
        this.cb = cb;
    }

    getNewInstance(initialConfig?: any): PortModel {
        return this.cb(initialConfig);
    }
}