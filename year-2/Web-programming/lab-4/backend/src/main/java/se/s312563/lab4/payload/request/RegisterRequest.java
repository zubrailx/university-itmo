package se.s312563.lab4.payload.request;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.io.Serializable;
import java.util.List;

@Getter
@Setter
@NoArgsConstructor
public class RegisterRequest implements Serializable {

    private String username;
    private String password;
    private List<String> roles;

}
