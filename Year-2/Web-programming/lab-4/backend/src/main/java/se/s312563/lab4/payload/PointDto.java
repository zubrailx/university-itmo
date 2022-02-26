package se.s312563.lab4.payload;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.time.LocalDateTime;
import java.util.Objects;

@Getter
@Setter
@NoArgsConstructor
public class PointDto {

    private Long userId;
    private double coordinateX;
    private double coordinateY;
    private double radius;
    private Boolean hit;
    private LocalDateTime ldt;
    // script time millis
    private Double stm;
    private Long pointId;

    public static Builder newBuilder() {
        return new PointDto().new Builder();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        PointDto that = (PointDto) o;
        return Double.compare(that.pointId, pointId) == 0 && Double.compare(that.coordinateX, coordinateX) == 0 && Double.compare(that.coordinateY, coordinateY) == 0 && Double.compare(that.radius, radius) == 0;
    }

    @Override
    public int hashCode() {
        return Objects.hash(pointId, coordinateX, coordinateY, radius);
    }

    //builder
    public class Builder {
        private Builder() {
        }

        public Builder setCoordinateX(double coordinateX) {
            PointDto.this.coordinateX = coordinateX;
            return this;
        }

        public Builder setCoordinateY(double coordinateY) {
            PointDto.this.coordinateY = coordinateY;
            return this;
        }

        public Builder setRadius(double radius) {
            PointDto.this.radius = radius;
            return this;
        }

        public Builder setHit(Boolean hit) {
            PointDto.this.hit = hit;
            return this;
        }

        public Builder setLocalTime(LocalDateTime ldt) {
            PointDto.this.ldt = ldt;
            return this;
        }

        public Builder setScriptTimeMillis(Double stm) {
            PointDto.this.stm = stm;
            return this;
        }

        public Builder setUserId(Long userId) {
            PointDto.this.userId = userId;
            return this;
        }

        public Builder setPointId(Long pointId) {
            PointDto.this.pointId = pointId;
            return this;
        }

        public PointDto build() {
            return PointDto.this;
        }
    }
}
