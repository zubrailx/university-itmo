import React from 'react';
import { Link } from 'react-router-dom';
import { useHistory, useLocation } from "react-router-dom";

import "./index.scss";
import userContext from '../../../model/context';
import { clearUserContext } from '../../../service/nodeRequests';

const NavigationPanel = ({ children }) => {
    let history = useHistory();
    const location = useLocation();

    const logOut = (e) => {
        e.preventDefault();
        clearUserContext()
            .then(res => {
                userContext.setAccessToken(null);
                userContext.setRefreshToken(null);
                userContext.setIsLogged(false);

                setTimeout(() => {
                    history.push("/index");
                }, 200);
            })
    }
    return (
        <nav className="navigation">
            <Link className="navigation__item" to="/index">Index page</Link>
            {children}
            <a onClick={logOut} className="navigation__item navigation__item_right"
                style={{
                    display: (location.pathname === "/index" || location.pathname === "/"
                        || location.pathname === "/register") ? "none" : "inline-block"
                }}>Log out</a>
        </nav>
    );
}

export default NavigationPanel;
