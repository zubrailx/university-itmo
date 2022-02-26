import React from "react";
import { Redirect, Route } from "react-router";

import IndexPage from "../components/pages/IndexPage";
import MainPage from "../components/pages/MainPage";
import RegisterPage from "../components/pages/RegisterPage";

import userContext from "../model/context";


export const routes = [
    {
        path: "/index",
        component: IndexPage,
        isPrivate: false
    },
    {
        path: "/main",
        component: MainPage,
        isPrivate: true
    },
    {
        path: "/register",
        component: RegisterPage,
        isPrivate: false
    },
    {
        path: "/",
        component: IndexPage,
        isPrivate: false
    }
];

export const AppRoute = ({ component: Component, path, isPrivate, ...rest }) => {
    const isLogged = userContext.isLogged;
    return (
        <Route path={path} {...rest}>
            {(props) => {
                return (isPrivate && !isLogged) ? (
                    <Redirect to="/index" />
                ) : (
                    <Component {...props} />
                )
            }}
        </Route>
    );
};
