package se.nikit.lab2.filter;

import javax.servlet.*;
import javax.xml.bind.ValidationException;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.Locale;

public class AreaCheckFilter implements Filter {

    private final double[] rValues = {1, 2, 3, 4, 5};
    private final double[] xValues = {-2, -1.5, -1, -0.5, 0, 0.5, 1, 1.5, 2};

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        Filter.super.init(filterConfig);
        System.out.printf("[%s] - loaded.%n", getClass());
    }

    @Override
    public void doFilter(ServletRequest req, ServletResponse resp, FilterChain chain) throws IOException, ServletException {

        try (PrintWriter out = resp.getWriter()) {
            try {
                final String source = req.getParameter("source");

                if (req.getParameter("value_R") == null) {
                    throw new NullPointerException("radius is not set");
                }

                double x = Double.parseDouble(req.getParameter("value_X").trim());
                double y = Double.parseDouble(req.getParameter("value_Y").trim());
                double r = Double.parseDouble(req.getParameter("value_R").trim());
                Integer.parseInt(req.getParameter("timezone_offset_minutes").trim());

                switch (source.toLowerCase(Locale.ROOT)) {
                    case "form-submit":
                        if (!validateForm(x, y, r)) throw new ValidationException("data is invalid");
                        break;
                    case "svg":
                        if (!validateSvg(r)) throw new ValidationException("data is invalid");
                        break;
                    default:
                        break;
                }

            } catch (NumberFormatException | NullPointerException | ValidationException ex) {
                out.write(ex.getMessage());
            } catch (Exception ex) {
                out.write("Error");
            }
            chain.doFilter(req, resp);
        }
    }

    @Override
    public void destroy() {
        Filter.super.destroy();
    }

    private boolean validateForm(double x, double y, double r) {
        boolean flagX = Arrays.binarySearch(xValues, x) > -1;
        boolean flagY = y > -5 && y < 5;
        boolean flagR = Arrays.binarySearch(rValues, r) > -1;
        return flagX && flagY && flagR;
    }

    private boolean validateSvg(double r) {
        return Arrays.binarySearch(rValues, r) > -1;
    }
}
