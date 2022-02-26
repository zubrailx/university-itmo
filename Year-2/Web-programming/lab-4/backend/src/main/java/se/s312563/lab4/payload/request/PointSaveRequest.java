package se.s312563.lab4.payload.request;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@Getter
@Setter
@NoArgsConstructor
public class PointSaveRequest {

    private Double coordinateX;
    private Double coordinateY;
    private Double radius;

}
