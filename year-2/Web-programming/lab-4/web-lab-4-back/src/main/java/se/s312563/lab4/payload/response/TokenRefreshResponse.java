package se.s312563.lab4.payload.response;

import lombok.Data;

@Data
public class TokenRefreshResponse {

    private String accessToken;
    private String refreshToken;
    private String tokenType;

    public TokenRefreshResponse(String accessToken, String refreshToken, String tokenType) {
        this.accessToken = accessToken;
        this.refreshToken = refreshToken;
        this.tokenType = tokenType;
    }
}
