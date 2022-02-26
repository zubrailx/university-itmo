import React from "react";
import loadable from "@loadable/component";

import "./index.scss";

const InputRing = loadable(() => import("@jetbrains/ring-ui/components/input/input"), {
    ssr: false,
    resolveComponent: (components) => components.Input
});


const Input = ({ className, type, label, error, onChange, ...rest }) => {
    return (
        <InputRing type={type} label={label} className={className} error={error} onChange={onChange} {...rest} />
    );
}

export default Input;