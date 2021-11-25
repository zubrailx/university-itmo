package se.s312563.lab4.payload.request;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Collection;

@Getter
@Setter
@NoArgsConstructor
public class PointRemoveRequest {
    Collection<Long> pointIds;
}
