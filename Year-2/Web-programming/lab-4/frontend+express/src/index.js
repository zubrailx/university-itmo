import React from 'react';
import { hydrate } from 'react-dom'
import { BrowserRouter } from 'react-router-dom';
import { loadableReady } from '@loadable/component'
import { getUserContext } from './service/nodeRequests';

import userContext from "./model/context";
import App from "./app"

// load userContext from nodeJS session

getUserContext()
    .then(res => {
        userContext.setUserId(res.data.userId);
        userContext.setUsername(res.data.username);
        userContext.setAccessToken(res.data.accessToken);
        userContext.setRefreshToken(res.data.refreshToken);
        userContext.setIsLogged(res.data.isLogged);

        loadableReady(() => {
            const root = document.getElementById('root');
            hydrate(
                <BrowserRouter>
                    <App />
                </BrowserRouter>, root)
        });
    });
