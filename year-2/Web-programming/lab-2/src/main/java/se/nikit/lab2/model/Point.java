package se.nikit.lab2.model;

import java.io.Serializable;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.time.format.DateTimeFormatter;

public class Point implements Serializable {
    private static final long serialVersionUID = 1L;

    private final double x;
    private final double y;
    private final double r;
    private final boolean hit;
    private final String localTimeFormatted;
    private final double executionTimeSeconds;

    public Point(double x, double y, double r, int offset) {
        this.x = x;
        this.y = y;
        this.r = r;

        long startTime = System.nanoTime();

        hit = calculate(x, y, r);
        localTimeFormatted = ZonedDateTime.now(ZoneOffset.UTC).plusMinutes(offset)
                .format(DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss"));

        executionTimeSeconds = ((double) System.nanoTime() - startTime) / Math.pow(10, 9);
    }

    public double getX() {
        return x;
    }

    public double getY() {
        return y;
    }

    public double getR() {
        return r;
    }

    public boolean isHit() {
        return hit;
    }

    public String getLocalTimeFormatted() {
        return localTimeFormatted;
    }

    public double getExecutionTimeSeconds() {
        return executionTimeSeconds;
    }

    private boolean calculate(double x, double y, double r) {
        if (x <= 0 && y <= 0){
            return (Math.pow(x, 2) + Math.pow(y, 2) <= Math.pow(r/2, 2));
        }
        if (x <= 0 && y >= 0){
            return (r / 2 + x / 2 >= y);
        }
        if ( x >= 0 && y <= 0){
            return (x <= r && y >= -r);
        }
        return false;
    }
}
