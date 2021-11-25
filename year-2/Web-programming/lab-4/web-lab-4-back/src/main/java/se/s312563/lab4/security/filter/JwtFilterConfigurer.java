package se.s312563.lab4.security.filter;

import lombok.RequiredArgsConstructor;
import org.springframework.security.config.annotation.SecurityConfigurerAdapter;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.web.DefaultSecurityFilterChain;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;
import se.s312563.lab4.security.JwtProvider;

@RequiredArgsConstructor
public class JwtFilterConfigurer extends SecurityConfigurerAdapter<DefaultSecurityFilterChain, HttpSecurity> {

    private final JwtProvider jwtProvider;

    @Override
    public void configure(HttpSecurity http) {
        JwtFilter customFilter = new JwtFilter(jwtProvider);
        http.addFilterBefore(customFilter, UsernamePasswordAuthenticationFilter.class);
    }


}
