package se.s312563.lab4.payload.response;

import lombok.Getter;
import lombok.Setter;

import java.io.Serializable;

@Getter
@Setter
public class MessageResponse implements Serializable {
    private String message;

    public MessageResponse(String message) {
        this.message = message;
    }
}
