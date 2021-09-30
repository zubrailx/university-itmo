package se.nikit.lab2.servlet;

import se.nikit.lab2.model.PointBean;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

public class ClearServlet extends HttpServlet {
    @Override
    public void init() throws ServletException {
        System.out.printf("[%s] - loaded.%n", getClass());
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        PointBean pb = (PointBean) req.getSession().getAttribute("pointbean");
        pb.clearList();
        try(PrintWriter out = resp.getWriter()){
            out.print("section cleared");
        }
    }

    @Override
    public void destroy() {

    }
}
