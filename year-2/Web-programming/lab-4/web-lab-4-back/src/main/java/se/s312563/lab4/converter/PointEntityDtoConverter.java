package se.s312563.lab4.converter;

import se.s312563.lab4.entity.PointEntity;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.payload.PointDto;

public class PointEntityDtoConverter {

    public static PointEntity dtoToEntity(PointDto pointDto, UserEntity persistent) {
        return new PointEntity(pointDto.getCoordinateX(), pointDto.getCoordinateY(),
                pointDto.getRadius(), persistent);
    }

    public static PointDto entityToDto(PointEntity pointEntity) {
        return PointDto.newBuilder()
                .setUserId(pointEntity.getUserEntity().getId())
                .setCoordinateX(pointEntity.getCoordinateX())
                .setCoordinateY(pointEntity.getCoordinateY())
                .setLocalTime(pointEntity.getLdt())
                .setHit(pointEntity.getHit())
                .setScriptTimeMillis(pointEntity.getStm())
                .setRadius(pointEntity.getRadius())
                .setPointId(pointEntity.getId()).build();
    }
}
