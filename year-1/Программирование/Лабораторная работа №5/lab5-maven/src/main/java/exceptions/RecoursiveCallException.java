package exceptions;

/**
 * Uсключение, выбрасываемое когда script совершает рекурсивный вызов или вызов по циклу.
 */
public class RecoursiveCallException extends RuntimeException {

    public RecoursiveCallException() {

    }
}
