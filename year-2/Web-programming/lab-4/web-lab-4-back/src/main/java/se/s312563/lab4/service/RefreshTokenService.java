package se.s312563.lab4.service;

import se.s312563.lab4.entity.RefreshTokenEntity;

import java.util.Optional;

public interface RefreshTokenService {

    Optional<String> createRefreshToken(Long userId);

    Optional<String> updateRefreshToken(Long userId);

    int deleteByUserId(Long userId);

    Optional<RefreshTokenEntity> findByRefreshTokenName(String refreshToken);
}
