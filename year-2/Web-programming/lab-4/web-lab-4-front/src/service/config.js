import axios from "axios";
import userContext from "../model/context"
import { userRefreshTokens } from "./apiRequests";
import { saveUserContext } from "./nodeRequests";
// API
export const tokenType = "Bearer ";
export const apiHostPath = "http://localhost:8080";

export const instanceAPI = axios.create({
    baseURL: "http://localhost:8080/lab-4-0.0.1-SNAPSHOT/api/v1",
    headers: {
        "Content-Type": "application/json",
    },
});

instanceAPI.interceptors.request.use(
    (config) => {
        const token = userContext.accessToken;
        if (token) {
            config.headers["Authorization"] = (tokenType + token);
        }
        return config;
    },
    (error) => {
        return Promise.reject(error);
    }
);

instanceAPI.interceptors.response.use(
    (res) => {
        return res;
    },
    async (err) => {
        const originalConfig = err.config;

        if (err.response) {
            // Access Token was expired
            if (err.response.status === 401 && !originalConfig._retry) {
                originalConfig._retry = true;

                try {
                    const rs = await userRefreshTokens(userContext.refreshToken);
                    const { accessToken, refreshToken } = rs.data;
                    userContext.setAccessToken(accessToken);
                    userContext.setRefreshToken(refreshToken);
                    saveUserContext(userContext);
                    instanceAPI.defaults.headers.common["Authorization"] = (tokenType + accessToken);

                    return instanceAPI(originalConfig);
                } catch (_error) {
                    if (_error.response && _error.response.data) {
                        return Promise.reject(_error.response.data);
                    }

                    return Promise.reject(_error);
                }
            }

            if (err.response.status === 403 && err.response.data) {
                return Promise.reject(err.response.data);
            }
        }

        return Promise.reject(err);
    }
);
