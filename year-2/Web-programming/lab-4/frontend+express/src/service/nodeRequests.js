import axios from "axios";


export function saveUserContext(context) {
    const data = {
        userId: context.userId,
        accessToken: context.accessToken,
        refreshToken: context.refreshToken,
        username: context.username,
        isLogged: context.isLogged
    }
    const headers = {
        "Content-Type": "application/json"
    };

    return axios.post(
        "/node/session/save",
        data,
        { headers: headers }
    );
}

export function getUserContext() {
    return axios.get("/node/session/get")
}

export function clearUserContext() {
    return axios.post("/node/session/clear");
}