import React from "react";

import "./index.scss";

class Clock extends React.Component {
    constructor(props) {
        super(props);
        this.state = { date: new Date(), interval: props.interval };
    };

    componentDidMount() {
        this.timerID = setInterval(() => this.tick(), this.state.interval);
    };

    componentWillUnmount() {
        clearInterval(this.timerID);
    };

    tick() {
        this.setState({ date: new Date() });
    };

    render() {
        return (<span>{this.state.date.toLocaleTimeString()}</span>);
    };
};

export default Clock;