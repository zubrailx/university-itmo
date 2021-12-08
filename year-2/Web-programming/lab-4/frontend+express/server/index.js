import fs from 'fs';
import path from 'path';
import express from 'express';
import session from 'express-session';
import React from 'react';
import { renderToString } from 'react-dom/server';
import { ChunkExtractor } from '@loadable/server';
import { StaticRouter } from 'react-router';

import App from '../src/app'

import 'ignore-styles';

const PORT = 9000;

const statsFile = path.resolve(__dirname, 'loadable-stats.json');
const extractor = new ChunkExtractor({ statsFile });

const app = express();

// access request as json
app.use(express.json())

app.use(session({
    secret: 'keyboard cat',
    resave: false,
    saveUninitialized: true,
    cookie: {
        maxAge: 24 * 60 * 60 * 1000
    },
}));

// store tokens and username
app.get("/node/session/get", (req, res) => {
    console.log(req.session.data);
    res.contentType("application/json");
    if (req.session.data != undefined) {
        res.send(req.session.data);
    } else {
        res.send({ message: "no data stored in session" });
    }
});

app.post("/node/session/save", (req, res) => {
    req.session.data = {
        userId: req.body.userId,
        accessToken: req.body.accessToken,
        refreshToken: req.body.refreshToken,
        username: req.body.username,
        isLogged: req.body.isLogged
    }
    console.log("post");
    console.log(req.body);
    res.send();
})

app.post("/node/session/clear", (req, res) => {
    req.session.data = {
        userId: null,
        accessToken: null,
        refreshToken: null,
        username: null,
        isLogged: false
    }
    console.log("clear");
    res.send();
});

app.get(/\.(js|css|map|ico)$/, express.static(path.resolve(__dirname, '../assets/')));

app.use('*', (req, res) => {
    let indexHTML = fs.readFileSync(path.resolve(__dirname, '../assets/index.html'), {
        encoding: 'utf8',
    });

    const jsx = extractor.collectChunks(
        <StaticRouter location={req.originalUrl}>
            <App />
        </StaticRouter>
    );
    const html = renderToString(jsx);
    console.log(`Request url is: ${req.originalUrl}`);
    res.contentType('text/html');
    res.status(200);

    return res.send(indexHTML.replace("<div id='root'></div>", `<div id='root'>${html}</div>`));
});

// server
app.listen(PORT, () => {
    console.log(`Express server started at <http://localhost:${PORT}>`);
});
