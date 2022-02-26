package se.s312563.lab4.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;
import se.s312563.lab4.entity.RoleEntity;
import se.s312563.lab4.security.UserRole;

@Repository
public interface RoleRepo extends JpaRepository<RoleEntity, Long> {

    RoleEntity findByRoleName(UserRole userRole);
}
