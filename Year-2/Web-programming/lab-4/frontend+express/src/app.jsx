import React from 'react';
import { Switch } from 'react-router';

import { routes, AppRoute } from './security/routes';


const App = () => {
    return (
        <React.StrictMode>
            <Switch>
                {routes.map((route) => {
                    return <AppRoute
                        key={route.path}
                        path={route.path}
                        component={route.component}
                        isPrivate={route.isPrivate}
                    />
                })}
            </Switch>
        </React.StrictMode>
    );
};

export default App;
