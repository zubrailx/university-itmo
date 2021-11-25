package se.s312563.lab4.service;

import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import se.s312563.lab4.entity.RefreshTokenEntity;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.repository.RefreshTokenRepo;
import se.s312563.lab4.repository.UserRepo;

import java.util.Optional;
import java.util.UUID;

@Slf4j
@Service
@RequiredArgsConstructor
public class RefreshTokenServiceImpl implements RefreshTokenService {

    private final UserRepo userRepo;
    private final RefreshTokenRepo refreshTokenRepo;

    @Override
    public Optional<String> createRefreshToken(Long userId) {
        RefreshTokenEntity refreshToken = new RefreshTokenEntity();
        Optional<UserEntity> userOptional = userRepo.findById(userId);

        if (userOptional.isPresent()) {
            refreshToken.setUserEntity(userOptional.get());
        } else {
            return Optional.empty();
        }

        refreshToken.setRefreshToken(UUID.randomUUID().toString());

        refreshToken = refreshTokenRepo.save(refreshToken);
        return Optional.of(refreshToken.getRefreshToken());
    }

    @Override
    public int deleteByUserId(Long userId) {
        Optional<UserEntity> userOptional = userRepo.findById(userId);
        // how many records were deleted
        return userOptional.map(refreshTokenRepo::deleteByUserEntity).orElse(0);
    }

    @Override
    public Optional<String> updateRefreshToken(Long userId) {
        this.deleteByUserId(userId);
        return this.createRefreshToken(userId);
    }

    @Override
    public Optional<RefreshTokenEntity> findByRefreshTokenName(String refreshToken) {
        RefreshTokenEntity tokenEntity = refreshTokenRepo.findByRefreshToken(refreshToken);
        return tokenEntity == null ? Optional.empty() : Optional.of(tokenEntity);
    }
}
