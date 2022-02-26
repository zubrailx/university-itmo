package se.s312563.lab4.service;

import se.s312563.lab4.payload.PointDto;

import java.util.Collection;
import java.util.Optional;

public interface PointService {
    Optional<PointDto> savePoint(PointDto point);

    Optional<PointDto> getPointByPointId(Long pointId);

    Collection<PointDto> getPointsByUserId(Long userId);

    Optional<PointDto> removePointByPointId(Long pointId);

    Collection<PointDto> removePointByPointIdMany(Collection<Long> pointId);

    Collection<PointDto> removePointsByUserId(Long userId);


//    boolean updatePointByPointId(Long pointId);
}
