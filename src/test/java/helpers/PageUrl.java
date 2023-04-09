package helpers;

public enum PageUrl {
  MAIN("https://www.xing.com/"),
  MAIN_EN("https://www.xing.com/en"),
  LOGIN("https://login.xing.com/"),
  LANGUAGE("https://www.xing.com/language"),
  HOME_PAGE("https://www.xing.com/discover/updates"),
  MESSAGES_PAGE("https://www.xing.com/chats"),
  YOU_PAGE("https://www.xing.com/you"),
  YOUR_POSTS("https://www.xing.com/discover/your-posts"),
  JOBS_PAGE("https://www.xing.com/jobs"),
  NEWS_PAGE("https://www.xing.com/news"),
  PREMIUM_PAGE("https://www.xing.com/premium"),
  PURCHASE_PREMIUM_PAGE("https://www.xing.com/premium"),
  NETWORK_PAGE("https://www.xing.com/network"),
  COMPANIES_PAGE("https://www.xing.com/companies"),
  SEARCH_PAGE("https://www.xing.com/search/members")
  ;

  private final String url;

  PageUrl(String url) {
    this.url = url;
  }

  public String getUrl() {
    return this.url;
  }
}
