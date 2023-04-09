package pages;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.PageFactory;

import components.HomeLogoComponent;
import helpers.PageUrl;

public class JobsPage extends XingPage {

  private final HomeLogoComponent homeLogoComponent;

  public JobsPage(WebDriver driver, boolean wasRedir) {
    super(driver);
    if (!wasRedir)
      driverGet(PageUrl.JOBS_PAGE);
    PageFactory.initElements(driver, this);

    homeLogoComponent = new HomeLogoComponent(driver);
  }

  public HomeLogoComponent getHomeLogoComponent() {
    return homeLogoComponent;
  }
}
