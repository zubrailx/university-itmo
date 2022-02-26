import React from "react";

import "./index.scss";

import Header from "../../orgamisms/Header";
import Footer from "../../orgamisms/Footer";
import NavigationPanel from "../../orgamisms/NavigationPanel";
import RegisterForm from "../../orgamisms/RegisterForm";

import "../../templates/typography.scss";
import "../../templates/resets.scss";
import "../../templates/page_layout.scss";

const RegisterPage = () => {
    return (
        <div>
            <Header />
            <NavigationPanel />
            <div className="content">
                <RegisterForm />
            </div>
            <Footer />
        </div>
    );
}

export default RegisterPage;