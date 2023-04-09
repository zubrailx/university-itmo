package helpers;

public enum PageUrl {
  MAIN("https://www.xing.com/"),
  MAIN_EN("https://www.xing.com/en"),
  LOGIN("https://login.xing.com/"),
  LANGUAGE("https://www.xing.com/language"),
  YOU_PAGE("https://www.xing.com/you"),
  YOUR_POSTS("https://www.xing.com/discover/your-posts"),
  HOME_PAGE("https://www.xing.com/discover/updates"),
  HOME_PAGE_POSTED("https://www.xing.com/discover/updates?sc_o=navigation_my_startpage&sc_o_PropActionOrigin=navigation_badge_no_badge");

  private final String url;

  PageUrl(String url) {
    this.url = url;
  }

  public String getUrl() {
    return this.url;
  }
}
