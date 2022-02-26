import React, { useState } from "react";
import { useHistory } from "react-router-dom";

import H1 from "../../atoms/H1";
import Input from "../../molecules/Input";
import Button from "../../atoms/Button";
import Link from "../../atoms/Link";
import Message from "../../atoms/Message";

import "./index.scss";
import { userAuth } from "../../../service/apiRequests";
import userContext from "../../../model/context";

import { encryptUsingSHA256 } from "../../../security/encrypt";
import { saveUserContext } from "../../../service/nodeRequests";

const LoginForm = () => {
    const [login, setLogin] = useState("");
    const [password, setPassword] = useState("");

    const [message, setMessage] = useState("");
    const [messageVisible, setMessageVisible] = useState(false);

    let history = useHistory();

    // input value handlers
    const handleLoginChange = (e) => {
        setLogin(e.target.value);
    }

    const handlePasswordChange = (e) => {
        setPassword(e.target.value);
    }

    const handleLoginSubmit = (e) => {
        e.preventDefault();
        userAuth(login, encryptUsingSHA256(password))
            .then(res => {
                setMessageVisible(false);
                // creating the copy of context
                userContext.setUserId(res.data.userId);
                userContext.setUsername(res.data.username);
                userContext.setAccessToken(res.data.accessToken);
                userContext.setRefreshToken(res.data.refreshToken);
                userContext.setIsLogged(true);
                // save context to node js
                saveUserContext(userContext);

                setTimeout(() => {
                    history.push("/main");
                }, 200);

            }).catch(error => {
                if (error.response) {
                    setMessage(error.response.data.message);
                    setMessageVisible(true);
                }
            });
    }

    return (
        <form className="form-login">
            <div className="form-login__wrapper">
                <H1 className="form-login__header">Login Form</H1>
                <Message value={message} type="error" style={{ display: messageVisible ? "inline-block" : "none" }} />
                <Input label="Login" onChange={handleLoginChange} className="form-login__input" />
                <Input type="password" onChange={handlePasswordChange} label="Password" className="form-login__input" />
                <div className="login-buttons form-login__buttons">
                    <Button onClick={handleLoginSubmit} className="login-buttons__button" >Sign in</Button>
                    <Link className="login-buttons__create" href="/register">create account</Link>
                </div>
            </div>
        </form>
    );
}


export default LoginForm;