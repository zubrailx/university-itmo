package se.nikit.lab2.servlet;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;

public class ControllerServlet extends HttpServlet {

    @Override
    public void init(){
        System.out.printf("[%s] - loaded.%n", getClass());
    }

    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

        final String source = req.getParameter("source");
        switch (source){
            case "form-clear":
                req.getRequestDispatcher("/clear").forward(req, resp);
                break;
            case "form-submit":
            case "svg":
                req.getRequestDispatcher("/check").forward(req, resp);
                break;
            default:
                req.getRequestDispatcher("/index.jsp").forward(req, resp);
        }
    }
}
