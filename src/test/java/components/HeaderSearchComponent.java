package components;

import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.support.FindBy;
import org.openqa.selenium.support.PageFactory;
import org.openqa.selenium.support.ui.ExpectedConditions;
import org.openqa.selenium.support.ui.WebDriverWait;

import pages.SearchPage;

public class HeaderSearchComponent extends Component {
  
  @FindBy(xpath = "//input[contains(@class, 'SearchInput')]")
  private WebElement searchBar;

  public HeaderSearchComponent(WebDriver driver) {
    super(driver);
    PageFactory.initElements(driver, this);
  }

  public SearchPage writeEnterSearch(String data) {
    var element = new WebDriverWait(driver, getWaitTimeout())
        .until(ExpectedConditions.elementToBeClickable(searchBar));

    element.click();
    element.sendKeys(data, Keys.ENTER);
    return new SearchPage(driver, true);
  }
}
