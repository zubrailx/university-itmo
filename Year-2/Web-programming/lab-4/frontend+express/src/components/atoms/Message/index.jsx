import React from "react";

import "./index.scss";


const Message = ({ type, value, ...rest }) => {
    return (
        <div className={"message message__" + type} {...rest}>
            <span> {value}</span>
        </div >
    )
}

export default Message;