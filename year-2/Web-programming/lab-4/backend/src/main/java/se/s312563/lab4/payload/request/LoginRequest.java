package se.s312563.lab4.payload.request;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.io.Serializable;

@Getter
@Setter
@NoArgsConstructor
public class LoginRequest implements Serializable {

    private String username;
    private String password;

}
