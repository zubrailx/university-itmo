import React from "react";
import loadable from "@loadable/component";

import "./index.scss";

const LinkRing = loadable(() => import("@jetbrains/ring-ui/components/link/link"), {
    ssr: false
});

const Link = ({ className, href, children }) => {
    return (
        <LinkRing className={className} href={href}>
            {children}
        </LinkRing>
    );
}

export default Link;