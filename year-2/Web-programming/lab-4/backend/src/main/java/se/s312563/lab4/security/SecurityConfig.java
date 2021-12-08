package se.s312563.lab4.security;

import lombok.RequiredArgsConstructor;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.config.annotation.authentication.builders.AuthenticationManagerBuilder;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.config.annotation.web.configuration.EnableWebSecurity;
import org.springframework.security.config.annotation.web.configuration.WebSecurityConfigurerAdapter;
import org.springframework.security.config.http.SessionCreationPolicy;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.cors.CorsConfiguration;
import se.s312563.lab4.security.filter.JwtFilterConfigurer;
import se.s312563.lab4.security.userDetails.CustomUserDetailsService;

@Configuration
@EnableWebSecurity
@RequiredArgsConstructor
public class SecurityConfig extends WebSecurityConfigurerAdapter {

    private final JwtProvider jwtProvider;
    private final CustomUserDetailsService customUserDetailsService;

    @Override
    protected void configure(HttpSecurity http) throws Exception {
        // enable CORS support
        http.cors().configurationSource(request -> new CorsConfiguration().applyPermitDefaultValues());
        // httpBasic - let you enable/disable http basic authentication
        http.httpBasic().disable();
        // disable cross site request forgery
        http.csrf().disable();
        // stateless sessions
        http.sessionManagement().sessionCreationPolicy(SessionCreationPolicy.STATELESS);

        http.authorizeRequests()
                // users
                .antMatchers("/api/v1/user/auth").permitAll()
                .antMatchers("/api/v1/user/register").permitAll()
                .antMatchers("/api/v1/user/refresh").permitAll()
                .antMatchers("/api/v1/user/all").hasAuthority(UserRole.ROLE_ADMIN.getAuthority())
                // points
                .antMatchers("/api/v1/point/save/*").hasAuthority(UserRole.ROLE_ADMIN.getAuthority())
                .antMatchers("/api/v1/point/remove/*").hasAuthority(UserRole.ROLE_ADMIN.getAuthority())
                .antMatchers("/api/v1/point/update/*").hasAnyAuthority(
                        UserRole.ROLE_ADMIN.getAuthority(), UserRole.ROLE_DEVELOPER.getAuthority())
                // disable everything else
                .anyRequest().authenticated();

        http.apply(new JwtFilterConfigurer(jwtProvider));
    }

    // remove encoding of password, cuz they are encoded on client-base server
    @Bean
    public PasswordEncoder passwordEncoder() {
        return new PasswordEncoder() {
            @Override
            public String encode(CharSequence charSequence) {
                return charSequence.toString();
            }

            @Override
            public boolean matches(CharSequence charSequence, String s) {
                return charSequence.toString().equals(s);
            }
        };
    }

    @Override
    @Bean
    public AuthenticationManager authenticationManagerBean() throws Exception {
        return super.authenticationManagerBean();
    }

    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {
        auth.userDetailsService(customUserDetailsService).passwordEncoder(passwordEncoder());
    }
}