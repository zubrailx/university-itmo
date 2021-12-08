package ru.nkulakov.commands.abstr;

/**
 * Режим, с которым должна быть запущена команда.
 */
public enum InvocationStatus {
    CLIENT,
    SERVER,
    CLIENT_RECEIVED
}
