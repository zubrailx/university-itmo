package se.s312563.lab4.security.filter;

import org.springframework.security.core.Authentication;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.filter.OncePerRequestFilter;
import se.s312563.lab4.exception.ProviderException;
import se.s312563.lab4.security.JwtProvider;

import javax.servlet.FilterChain;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Arrays;

public class JwtFilter extends OncePerRequestFilter {

    private final JwtProvider jwtProvider;
    private final String[] paths = {"/api/v1/user/auth", "/api/v1/user/register", "/api/v1/user/refresh"};

    public JwtFilter(JwtProvider jwtProvider) {
        this.jwtProvider = jwtProvider;
    }

    @Override
    protected void doFilterInternal(HttpServletRequest req, HttpServletResponse res, FilterChain filterChain) throws ServletException, IOException {
        String token = jwtProvider.resolveToken(req);
        try {
            if (token != null && jwtProvider.validateToken(token)) {
                Authentication auth = jwtProvider.getAuthentication(token);
                SecurityContextHolder.getContext().setAuthentication(auth);
            }
        } catch (ProviderException ex) {
            //this is very important, since it guarantees the user is not authenticated at all
            SecurityContextHolder.clearContext();
            // send response
            res.resetBuffer();
            res.setStatus(ex.getHttpStatus().value());
            res.setHeader("Content-Type", "text/plain");
            PrintWriter out = res.getWriter();
            out.write(ex.getMessage());
            return;
        }
        filterChain.doFilter(req, res);
    }

    @Override
    protected boolean shouldNotFilter(HttpServletRequest request) {
        return Arrays.asList(paths).contains(request.getServletPath());
    }
}
