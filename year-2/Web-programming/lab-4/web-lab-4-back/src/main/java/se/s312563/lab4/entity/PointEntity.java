package se.s312563.lab4.entity;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.time.LocalDateTime;
import java.time.ZoneOffset;
import java.time.ZonedDateTime;
import java.util.Objects;

@Getter
@Setter
@NoArgsConstructor
@Entity
@Table(name = "points")
public class PointEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long id;
    @Column(name = "coordinate_x")
    private double coordinateX;
    @Column(name = "coordinate_y")
    private double coordinateY;
    @Column(name = "radius")
    private double radius;
    @Column(name = "hit")
    private Boolean hit;
    @Column(name = "script_time")
    // in milliseconds
    private Double stm;
    @Column(name = "local_time")
    private LocalDateTime ldt;

    @ManyToOne
    @JoinColumn(name = "user_id", referencedColumnName = "id")
    private UserEntity userEntity;

    public PointEntity(double coordinateX, double coordinateY, double radius, UserEntity userEntity) {
        this.coordinateX = coordinateX;
        this.coordinateY = coordinateY;
        this.radius = radius;
        if (hit == null) {
            this.calculate();
        }
        this.userEntity = userEntity;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PointEntity that = (PointEntity) o;
        return Double.compare(that.coordinateX, coordinateX) == 0 &&
                Double.compare(that.coordinateY, coordinateY) == 0 &&
                Double.compare(that.radius, radius) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(coordinateX, coordinateY, radius);
    }

    void calculate() {
        int offset = 180;
        // already initialized coordinateX, coordinateY, radius
        long startTime = System.nanoTime();
        hit = calculateHit(coordinateX, coordinateY, radius);
        ldt = ZonedDateTime.now(ZoneOffset.UTC).plusMinutes(offset).toLocalDateTime();
        stm = (System.nanoTime() - startTime) / Math.pow(10, 6);
    }

    boolean calculateHit(double x, double y, double r) {
        if (x <= 0 && y >= 0) {
            return (Math.pow(x, 2) + Math.pow(y, 2) <= Math.pow(r, 2));
        }
        if (x <= 0 && y <= 0) {
            return ((-r - x) / 2 <= y);
        }
        if (x >= 0 && y <= 0) {
            return (x <= r / 2 && y >= -r);
        }
        return false;
    }
}
