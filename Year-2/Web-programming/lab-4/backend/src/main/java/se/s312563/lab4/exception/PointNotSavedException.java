package se.s312563.lab4.exception;

public class PointNotSavedException extends RuntimeException {

    private static final long serialVersionUID = 1L;

    private final String message;

    public PointNotSavedException(String message) {
        this.message = message;
    }

    @Override
    public String getMessage() {
        return message;
    }
}
