export function saveToken(token: string): void {
  window.localStorage.setItem("rss-token", token)
}

export function getToken(): string {
  return window.localStorage.getItem("rss-token") || ""
}
