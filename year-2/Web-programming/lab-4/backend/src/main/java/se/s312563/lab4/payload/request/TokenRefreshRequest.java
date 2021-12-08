package se.s312563.lab4.payload.request;

import lombok.Data;

import java.io.Serializable;

@Data
public class TokenRefreshRequest implements Serializable {

    private String refreshToken;

}
