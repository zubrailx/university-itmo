import React from "react";

import "./index.scss";

import { pointGetByUserId, userRefreshTokens, userRegister } from "../../../service/apiRequests";
import userContext from "../../../model/context";
import { loadStore } from "../../../state/state";

import Header from "../../orgamisms/Header";
import Footer from "../../orgamisms/Footer";
import NavigationPanel from "../../orgamisms/NavigationPanel";
import PointForm from "../../orgamisms/PointForm";
import Table from "../../orgamisms/Table";
import Graph from "../../orgamisms/Graph";

import "../../templates/typography.scss";
import "../../templates/resets.scss";
import "../../templates/page_layout.scss";
import Point from "../../../model/point";

const MainPage = () => {
    pointGetByUserId(userContext.userId)
        .then(res => {
            loadStore(res.data.map(e => new Point(e.pointId, e.coordinateX, e.coordinateY, e.radius, e.hit, e.ldt, e.stm)));
        })
        .catch(err => alert(err));

    return (
        <div>
            <Header />
            <NavigationPanel />
            <div className="content main">
                <div className="main-interactive">
                    <Graph />
                    <PointForm />
                    <Table />
                </div>
            </div>
            <Footer />
        </div>
    )
}

export default MainPage;
