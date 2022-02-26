const userContext = {
    userId: null,
    username: null,
    accessToken: null,
    refreshToken: null,
    isLogged: null,
    setUserId: (long) => userContext.userId = long,
    setUsername: (str) => userContext.username = str,
    setAccessToken: (str) => userContext.accessToken = str,
    setRefreshToken: (str) => userContext.refreshToken = str,
    setIsLogged: (bool) => userContext.isLogged = bool
}

export default userContext;
