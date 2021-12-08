package se.s312563.lab4.service;

import se.s312563.lab4.entity.RoleEntity;

public interface RoleService {
    RoleEntity saveRole(RoleEntity roleEntity);

    void removeRole(RoleEntity roleEntity);
}
