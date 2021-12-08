package ru.nkulakov.exceptions;

/**
 * Uсключение, выбрасываемое когда script совершает рекурсивный вызов или вызов по циклу.
 */
public class RecursiveCallException extends RuntimeException {

    private final String filename;

    public RecursiveCallException(String filename) {
        this.filename = filename;
    }

    public String getFilename() {
        return filename;
    }
}
