import { instanceAPI } from "./config";

export function userRegister(username, password, roles) {
    const data = {
        "username": username,
        "password": password,
        "roles": roles
    }

    return instanceAPI.post("/user/register", data);
}

export function userAuth(username, password) {
    const data = {
        "username": username,
        "password": password
    };

    return instanceAPI.post("/user/auth", data);
}

export function userRefreshTokens(refreshToken) {
    const data = {
        "refreshToken": refreshToken
    };

    return instanceAPI.post("/user/refresh", data);
}

export function pointGetByPointId(pointId) {
    const params = {
        "pointId": pointId
    }

    return instanceAPI.get("/point/get", {
        params: params
    }
    );
}

export function pointGetByUserId(userId) {

    return instanceAPI.get(`/point/get/${userId}`);
}

export function pointSave(point) {
    const data = {
        "coordinateX": point.coordinateX,
        "coordinateY": point.coordinateY,
        "radius": point.radius
    };

    return instanceAPI.post("/point/save", data);
}

export function pointRemove(pointId) {
    const data = pointId;

    return instanceAPI.post("/point/remove", data);
}

// DIFFERENT FOR USER AND ADMIN
export function pointRemoveMany(pointIds) {
    const data = {
        "pointIds": pointIds
    };

    return instanceAPI.post("/point/remove_many", data);
}

// ADMIN 
export function userGetAllUsers() {

    return instanceAPI.get("/user/all");
}

export function pointSaveByUserId(userId, point) {
    const data = {
        "coordinateX": point.x,
        "coordinateY": point.y,
        "radius": point.r
    };

    return instanceAPI.post(`/point/save/${userId}`, data);
}

export function pointRemoveByUserId(userId) {

    return instanceAPI.post(`/point/remove/${userId}`);
}

