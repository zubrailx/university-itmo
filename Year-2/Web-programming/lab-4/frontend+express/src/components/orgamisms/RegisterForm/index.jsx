import React, { useState } from "react";
import { useHistory } from "react-router-dom";

import H1 from "../../atoms/H1";
import Input from "../../molecules/Input";
import Button from "../../atoms/Button";
import Message from "../../atoms/Message";


import "./index.scss";

import { userRegister } from "../../../service/apiRequests";
import { encryptUsingSHA256 } from "../../../security/encrypt";

const msg = {
    none: undefined,
    login_invalid: "Login length is too small or long or it has bad symbols",
    password_small: "Password length is too small",
    repeated_bad: "Repeated password isn't equals to Password"
}

const RegisterForm = () => {
    const [login, setLogin] = useState("");
    const [password, setPassword] = useState("");
    const [repPassword, setRepPassword] = useState("");

    // set error messages labels
    const [errorLogin, setErrorLogin] = useState(msg.none);
    const [errorPassword, setErrorPassword] = useState(msg.none);
    const [errorRepPassword, setErrorRepPassword] = useState(msg.none);

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
    const HandleRepPasswordChange = (e) => {
        setRepPassword(e.target.value);
    }

    const validateValuesAndSetLabels = () => {
        let loginCorrect = false;
        let passwordCorrect = false;
        let repPasswordCorrect = false;

        if (login !== "" && login.match(/^[A-Za-z0-9_]{4,15}$/)) {
            loginCorrect = true;
        }
        if (password.length >= 4) {
            passwordCorrect = true;
        }
        if (repPassword === password) {
            repPasswordCorrect = true;
        }
        // input error message setter
        setErrorLogin(loginCorrect ? msg.none : msg.login_invalid);
        setErrorPassword(passwordCorrect ? msg.none : msg.password_small);
        setErrorRepPassword(repPasswordCorrect ? msg.none : msg.repeated_bad);

        return loginCorrect && passwordCorrect && repPasswordCorrect;
    }

    const handleRegisterSubmit = (e) => {
        e.preventDefault();
        if (validateValuesAndSetLabels()) {
            userRegister(login, encryptUsingSHA256(password))
                .then(res => {
                    // remove error message
                    setMessageVisible(false);
                    setTimeout(() => {
                        history.push("/index");
                    }, 200);

                }).catch(error => {
                    if (error.response) {
                        setMessage(error.response.data.message);
                        setMessageVisible(true);
                    }
                });
        }
    }

    return (
        <form className="form-register">
            <div className="form-register__wrapper">
                <H1 className="form-register__header">Register Form</H1>
                <Input type="text" onChange={handleLoginChange} error={errorLogin} label="Login" className="form-register__input" />
                <Input type="password" onChange={handlePasswordChange} error={errorPassword} label="Password" className="form-register__input" />
                <Input type="password" onChange={HandleRepPasswordChange} error={errorRepPassword} label="Repeat password" className="form-register__input" />
                <div className="register-buttons form-register__buttons">
                    <Button className="register-buttons__button" onClick={handleRegisterSubmit}>Sign up</Button>
                </div>
                <Message value={message} type="error" style={{ display: messageVisible ? "inline-block" : "none" }} />
            </div>
        </form>
    );
}


export default RegisterForm;