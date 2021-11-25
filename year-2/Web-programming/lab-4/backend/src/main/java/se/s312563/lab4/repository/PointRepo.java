package se.s312563.lab4.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import org.springframework.transaction.annotation.Transactional;
import se.s312563.lab4.entity.PointEntity;

import java.util.Collection;

@Repository
public interface PointRepo extends JpaRepository<PointEntity, Long> {
    Collection<PointEntity> findAllByUserEntityId(Long userId);

    @Transactional
    Collection<PointEntity> deleteAllByUserEntityId(Long userId);
}
