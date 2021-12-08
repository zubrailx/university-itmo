package se.nikit.lab2.servlet;

import com.google.gson.Gson;
import se.nikit.lab2.ResponseData;
import se.nikit.lab2.model.Point;
import se.nikit.lab2.model.PointBean;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpServletResponseWrapper;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;

public class AreaCheckServlet extends HttpServlet {

    public void init() {
        System.out.printf("[%s] - loaded.%n", getClass());
    }

    public void doGet(HttpServletRequest req, HttpServletResponse resp) throws IOException, ServletException {
        double x = Double.parseDouble(req.getParameter("value_X").trim());
        double y = Double.parseDouble(req.getParameter("value_Y").trim());
        double r = Double.parseDouble(req.getParameter("value_R").trim());
        int offset = Integer.parseInt(req.getParameter("timezone_offset_minutes").trim());

        PointBean pb = (PointBean) req.getSession().getAttribute("pointbean");
        Point p = new Point(x, y, r, offset);
        pb.addFirst(p);

        HttpServletResponseWrapper responseWrapper = new HttpServletResponseWrapper(resp) {
            private final StringWriter sw = new StringWriter();

            @Override
            public PrintWriter getWriter() throws IOException {
                return new PrintWriter(sw);
            }

            @Override
            public String toString() {
                return sw.toString();
            }
        };

        try (PrintWriter out = resp.getWriter()) {
            resp.setContentType("application/json");
            req.getRequestDispatcher("jsp-components/table.jsp").include(req, responseWrapper);

            out.print(new Gson().toJson(ResponseData.newBuilder()
                    .setHit(p.isHit())
                    .setHtmlTable(responseWrapper.toString())
                    .setNewPageLink("index.jsp")
                    .build()
            ));
        }
    }

    public void destroy() {
        System.out.println("AreaCheckServlet destroyed.");
    }
}