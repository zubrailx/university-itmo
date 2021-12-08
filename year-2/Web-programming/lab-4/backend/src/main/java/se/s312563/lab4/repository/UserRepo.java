package se.s312563.lab4.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import se.s312563.lab4.entity.UserEntity;

@Repository
public interface UserRepo extends JpaRepository<UserEntity, Long> {

    UserEntity findByUsername(String username);
}
