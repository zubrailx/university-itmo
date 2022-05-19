package se.s312563.lab3.entity;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.util.Objects;

@Entity
@Table(name = "points")
@Getter
@Setter
@NoArgsConstructor
public class Point {
    @Id
    @GeneratedValue
    private Long id;

    @Column(name = "coordinate_x")
    private double x;
    @Column(name = "coordinate_y")
    private double y;
    @Column(name = "radius")
    private double r;
    @Column(name = "hit")
    private Boolean hit;
    @Column(name = "local_time")
    private LocalDateTime localDateTime;
    @Column(name = "execution_time")
    private Double scriptTimeSeconds;
    @Transient
    private Integer offset;

    public static Builder newBuilder() {
        return new Point().new Builder();
    }

    private boolean calculate(double x, double y, double r) {
        if (x <= 0 && y <= 0) {
            return (x >= -r && y >= -r);
        }
        if (x <= 0 && y >= 0) {
            return (Math.pow(x, 2) + Math.pow(y, 2) <= Math.pow(r, 2));
        }
        if (x >= 0 && y >= 0) {
            return ((r - x) / 2 >= y);
        }
        return false;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Point point = (Point) o;
        return Double.compare(point.x, x) == 0 && Double.compare(point.y, y) == 0 && Double.compare(point.r, r) == 0
                && Objects.equals(hit, point.hit) && Objects.equals(localDateTime, point.localDateTime)
                && Objects.equals(scriptTimeSeconds, point.scriptTimeSeconds);
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y, r, hit, localDateTime, scriptTimeSeconds);
    }

    public class Builder {
        private Builder() {
            // private constructor
        }

        public Builder setX(double x) {
            Point.this.x = x;
            return this;
        }

        public Builder setY(double y) {
            Point.this.y = y;
            return this;
        }

        public Builder setR(double r) {
            Point.this.r = r;
            return this;
        }

        public Builder setId(long id) {
            Point.this.id = id;
            return this;
        }

        public Builder setHit(Boolean hit) {
            Point.this.hit = hit;
            return this;
        }

        public Builder setLocalDateTime(LocalDateTime ldt) {
            Point.this.localDateTime = ldt;
            return this;
        }

        public Builder setScriptTime(Double scriptTime) {
            Point.this.scriptTimeSeconds = scriptTime;
            return this;
        }

        public Builder setOffset(Integer offset) {
            Point.this.offset = offset;
            return this;
        }

        public Point build() {
            long startTime = System.nanoTime();

            if (localDateTime == null) {
                offset = offset == null ? 0 : offset;
                localDateTime = ZonedDateTime.now(ZoneOffset.UTC).plusMinutes(offset).toLocalDateTime();
            }
            if (hit == null) {
                Point.this.hit = Point.this.calculate(Point.this.x, Point.this.y, Point.this.r);
            }
            if (scriptTimeSeconds == null) {
                scriptTimeSeconds = ((double) System.nanoTime() - startTime) / Math.pow(10, 9);
            }
            return Point.this;
        }
    }
}
