package se.s312563.lab4.controller;

import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.BadCredentialsException;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.*;
import se.s312563.lab4.entity.RefreshTokenEntity;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.exception.InvalidRequestException;
import se.s312563.lab4.exception.RefreshTokenNotFound;
import se.s312563.lab4.exception.UserNotFoundException;
import se.s312563.lab4.payload.UserDto;
import se.s312563.lab4.payload.request.LoginRequest;
import se.s312563.lab4.payload.request.RegisterRequest;
import se.s312563.lab4.payload.request.TokenRefreshRequest;
import se.s312563.lab4.payload.response.JwtResponse;
import se.s312563.lab4.payload.response.MessageResponse;
import se.s312563.lab4.payload.response.TokenRefreshResponse;
import se.s312563.lab4.security.JwtProvider;
import se.s312563.lab4.security.UserRole;
import se.s312563.lab4.security.userDetails.CustomUserDetails;
import se.s312563.lab4.service.RefreshTokenService;
import se.s312563.lab4.service.UserService;

import java.util.*;
import java.util.stream.Collectors;

@RestController
@RequiredArgsConstructor
@RequestMapping("/api/v1/user")
public class UserController {

    private final AuthenticationManager authenticationManager;
    private final UserService userService;
    private final RefreshTokenService refreshTokenService;
    private final JwtProvider jwtProvider;

    private final String tokenType = "Bearer";


    @PostMapping("/auth")
    public ResponseEntity<?> authenticate(@RequestBody LoginRequest req) {
        try {
            if (req.getUsername() == null || req.getUsername().equals("")) {
                throw new InvalidRequestException("Invalid request data: username is not set.");
            }
            if (req.getPassword() == null || req.getPassword().equals("")) {
                throw new InvalidRequestException("Invalid request data: password is not set.");
            }

            Authentication authentication = authenticationManager
                    .authenticate(new UsernamePasswordAuthenticationToken(req.getUsername(), req.getPassword()));
            SecurityContextHolder.getContext().setAuthentication(authentication);

            CustomUserDetails userDetails = (CustomUserDetails) authentication.getPrincipal();
            List<String> roles = userDetails.getAuthorities().stream().map(GrantedAuthority::getAuthority).collect(Collectors.toList());

            String accessToken = jwtProvider.createToken(userDetails);
            // delete if existed
            Optional<String> refreshTokenOptional = refreshTokenService.updateRefreshToken(userDetails.getId());
            if (!refreshTokenOptional.isPresent()) {
                throw new UserNotFoundException("User is not found in database");
            }

            return ResponseEntity.status(HttpStatus.CREATED).body(new JwtResponse(accessToken, tokenType,
                    refreshTokenOptional.get(), userDetails.getId(), userDetails.getUsername(), roles));

        } catch (InvalidRequestException iRE) {
            return ResponseEntity.badRequest().body(new MessageResponse(iRE.getMessage()));
        } catch (BadCredentialsException bCE) {
            return ResponseEntity.badRequest().body(new MessageResponse("Invalid login or password"));
        }
    }

    @PostMapping("/register")
    public ResponseEntity<?> register(@RequestBody RegisterRequest req) {
        try {
            if (req.getUsername() == null || req.getUsername().equals("")) {
                throw new InvalidRequestException("Invalid request data: username is not set");
            }
            if (req.getPassword() == null || req.getPassword().equals("")) {
                throw new InvalidRequestException("Invalid request data: password is not set");
            }
            if (userService.userExistByName(req.getUsername())) {
                throw new InvalidRequestException(String.format("User with username '%s' already exists", req.getUsername()));
            }

            UserDto userDto = new UserDto(req.getUsername(), req.getPassword());
            Set<UserRole> roleSet = new HashSet<>();

            List<String> strRoles = req.getRoles();
            if (strRoles == null || !strRoles.contains(UserRole.ROLE_USER.getAuthority())) {
                roleSet.add(UserRole.ROLE_USER);
            } else {
                strRoles.forEach(reqRole -> {
                    if (Arrays.stream(UserRole.values()).anyMatch(userRole -> userRole.getAuthority().equals(reqRole))) {
                        roleSet.add(UserRole.valueOf(reqRole));
                    }
                });
            }
            userDto.setRoleSet(roleSet);
            return ResponseEntity.status(HttpStatus.CREATED).body(userService.saveUser(userDto));

        } catch (InvalidRequestException iRE) {
            return ResponseEntity.badRequest().body(new MessageResponse(iRE.getMessage()));
        }
    }

    @PostMapping("/refresh")
    public ResponseEntity<?> refreshToken(@RequestBody TokenRefreshRequest req) {
        try {
            // refresh token
            Optional<RefreshTokenEntity> optionalRTE = refreshTokenService.findByRefreshTokenName(req.getRefreshToken());
            if (!optionalRTE.isPresent()) {
                throw new RefreshTokenNotFound(req.getRefreshToken(), "Token is not found");
            }
            UserEntity userEntity = optionalRTE.get().getUserEntity();
            Optional<String> optionalRToken = refreshTokenService.updateRefreshToken(userEntity.getId());
            if (!optionalRToken.isPresent()) {
                throw new UserNotFoundException("User is not present in database");
            }
            // access token
            CustomUserDetails customUserDetails = CustomUserDetails.build(userEntity);
            String accessToken = jwtProvider.createToken(customUserDetails);

            return ResponseEntity.status(HttpStatus.CREATED).body(new TokenRefreshResponse(accessToken, optionalRToken.get(), tokenType));

        } catch (Exception ex) {
            return ResponseEntity.badRequest().body(new MessageResponse(ex.getMessage()));
        }
    }

    @GetMapping("/all")
    public ResponseEntity<?> getAllUsers() {
        return ResponseEntity.ok().body(userService.getUsers());
    }
}
