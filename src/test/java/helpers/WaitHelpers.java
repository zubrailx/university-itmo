package helpers;

import java.time.Duration;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.ExpectedCondition;
import org.openqa.selenium.support.ui.WebDriverWait;

/**
 * WaitHelpers
 */
public class WaitHelpers {

  public static Boolean waitStringEqual(WebDriver driver, Duration timeout, String lsv) {
    return (new WebDriverWait(driver, timeout))
        .until((ExpectedCondition<Boolean>) (u) -> lsv.equals(driver.getCurrentUrl()));
  }
}
