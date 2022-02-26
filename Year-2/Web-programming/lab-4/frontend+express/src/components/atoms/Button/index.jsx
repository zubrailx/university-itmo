import React from "react";
import loadable from "@loadable/component";

import "./index.scss";

const ButtonRing = loadable(() => import("@jetbrains/ring-ui/components/button/button"), {
    ssr: false
});

const Button = ({ children, className, onClick }) => {
    return (
        <ButtonRing className={className} onClick={onClick} children={children} />
    );
}

export default Button;