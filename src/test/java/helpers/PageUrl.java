package helpers;

public enum PageUrl {
  MAIN("https://www.xing.com/"),
  LANGUAGE("https://www.xing.com/language");

  private final String url;

  PageUrl(String url) {
    this.url = url;
  }

  public String getUrl() {
    return this.url;
  }
}
