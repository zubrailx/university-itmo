import React from "react";

import "./index.scss";

import Header from "../../orgamisms/Header";
import Footer from "../../orgamisms/Footer";
import NavigationPanel from "../../orgamisms/NavigationPanel";
import LoginForm from "../../orgamisms/LoginForm"

import "../../templates/typography.scss";
import "../../templates/resets.scss";
import "../../templates/page_layout.scss";

const IndexPage = () => {
    return (
        <div>
            <Header />
            <NavigationPanel />
            <div className="content">
                <LoginForm />
            </div>
            <Footer />
        </div>
    );
}

export default IndexPage;