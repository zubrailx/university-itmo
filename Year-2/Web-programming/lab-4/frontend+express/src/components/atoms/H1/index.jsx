import React from "react";
import loadable from "@loadable/component";

import "./index.scss";

const H1Ring = loadable(() => import("@jetbrains/ring-ui/components/heading/heading"), {
    ssr: false,
    resolveComponent: (components) => components.H1
});


const H1 = ({ className, children }) => {
    return (
        <H1Ring className={className}>
            {children}
        </H1Ring>
    );
}

export default H1;