import React from "react";

import Link from "../../atoms/Link";
import Clock from "../../atoms/Clock";

import "./index.scss";

const Header = () => {
    return (
        <header className="header">
            <HeaderItem className="header__item_left">
                <HeaderClock />
            </HeaderItem>
            <HeaderItem>
                <span>Лабораторная работа №4. Вариант 30613*.</span>
            </HeaderItem>
            <HeaderItem className="header__item_right">
                <Link href="https://github.com/zubrailx">Github</Link>
            </HeaderItem>
        </header>
    );
}

const HeaderItem = ({ className, children }) => {
    return (
        <div className={"header-item header__item " + (className == undefined ? "" : className)}>
            {children}
        </div>
    );
}


let clockTickInterval = 1000;

const HeaderClock = () => {
    return (
        <div className="header-clock">
            <span key="1" className="header-clock__label">Текущее время: </span>
            <Clock key="2" className="header-clock__clock" interval={clockTickInterval} />
        </div>
    );
}


export default Header;