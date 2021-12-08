package se.s312563.lab4.exception;

public class RefreshTokenNotFound extends RuntimeException {

    private static final long serialVersionUID = 1L;

    private final String token;
    private final String message;

    public RefreshTokenNotFound(String token, String message) {
        this.token = token;
        this.message = message;
    }

    public String getToken() {
        return token;
    }

    @Override
    public String getMessage() {
        return message + "\ntoken: " + getToken();
    }
}
