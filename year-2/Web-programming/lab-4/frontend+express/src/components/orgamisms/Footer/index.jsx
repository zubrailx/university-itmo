import React from "react";

import "./index.scss";

const Footer = () => {
    return (
        <footer className="footer">
            <FooterItem>
                <span>@Web-programming 2021</span>
            </FooterItem>
        </footer>
    );
}

const FooterItem = ({ className, children }) => {
    return (
        <div className={"footer-item footer__item " + (className == undefined ? "" : className)}>
            {children}
        </div>
    );
}


export default Footer;
