package ru.nkulakov.gui.locale;

import java.util.HashMap;
import java.util.Locale;
import java.util.ResourceBundle;

public class Localization {

    private static final HashMap<LocaleEnum, Locale> locales = new HashMap<>();
    private static ResourceBundle rb = getBundle(LocaleEnum.BASE);

    static {
        setLocales();
    }

    private static void setLocales() {
        locales.put(LocaleEnum.ru_RU, new Locale("ru", "RU"));
        locales.put(LocaleEnum.sl_SI, new Locale("sl", "SI"));
        locales.put(LocaleEnum.el_GR, new Locale("el", "GR"));
        locales.put(LocaleEnum.es_PR, new Locale("es", "PR"));
    }

    public static ResourceBundle getBundle(LocaleEnum le) {
        return locales.containsKey(le) ? ResourceBundle.getBundle("locales.message", locales.get(le)) :
                ResourceBundle.getBundle("locales.message");
    }

    public static ResourceBundle getCurrentResourceBundle() {
        return rb;
    }

    public static void setCurrentResourceBundle(ResourceBundle resourceBundle) {
        rb = resourceBundle;
    }
}
