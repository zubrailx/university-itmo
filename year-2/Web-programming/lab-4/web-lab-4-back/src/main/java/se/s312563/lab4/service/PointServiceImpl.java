package se.s312563.lab4.service;

import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import se.s312563.lab4.converter.PointEntityDtoConverter;
import se.s312563.lab4.entity.PointEntity;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.payload.PointDto;
import se.s312563.lab4.repository.PointRepo;
import se.s312563.lab4.repository.UserRepo;

import java.util.Collection;
import java.util.Optional;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
@Slf4j
public class PointServiceImpl implements PointService {

    private final PointRepo pointRepo;
    private final UserRepo userRepo;

    @Override
    public Optional<PointDto> savePoint(PointDto point) {
        //validation
        Optional<UserEntity> userOptional = userRepo.findById(point.getUserId());
        if (!userOptional.isPresent()) {
            return Optional.empty();
        }
        PointEntity pointEntity = PointEntityDtoConverter.dtoToEntity(point, userOptional.get());
        return Optional.of(PointEntityDtoConverter.entityToDto(pointRepo.save(pointEntity)));
    }

    @Override
    public Optional<PointDto> getPointByPointId(Long pointId) {
        Optional<PointEntity> pointEntityOptional = pointRepo.findById(pointId);
        return pointEntityOptional.map(PointEntityDtoConverter::entityToDto);
    }

    @Override
    public Collection<PointDto> getPointsByUserId(Long userId) {
        return pointRepo.findAllByUserEntityId(userId).stream()
                .map(PointEntityDtoConverter::entityToDto).collect(Collectors.toList());
    }

    @Override
    public Optional<PointDto> removePointByPointId(Long pointId) {
        Optional<PointEntity> pointEntityOptional = pointRepo.findById(pointId);
        if (pointEntityOptional.isPresent()) {
            pointRepo.delete(pointEntityOptional.get());
            return Optional.of(PointEntityDtoConverter.entityToDto(pointEntityOptional.get()));
        } else {
            return Optional.empty();
        }
    }

    @Override
    public Collection<PointDto> removePointByPointIdMany(Collection<Long> pointIds) {

        Collection<PointEntity> pointEntityCollection = pointRepo.findAllById(pointIds);
        pointRepo.deleteAllById(pointIds);
        return pointEntityCollection.stream().map(PointEntityDtoConverter::entityToDto).collect(Collectors.toList());
    }

    @Override
    public Collection<PointDto> removePointsByUserId(Long userId) {
        Collection<PointEntity> pointDtoCollection = pointRepo.deleteAllByUserEntityId(userId);
        return pointDtoCollection.stream().map(PointEntityDtoConverter::entityToDto).collect(Collectors.toList());
    }

}
