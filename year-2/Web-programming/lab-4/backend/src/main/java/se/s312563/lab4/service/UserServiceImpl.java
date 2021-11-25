package se.s312563.lab4.service;

import lombok.RequiredArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.springframework.stereotype.Service;
import se.s312563.lab4.converter.UserEntityDtoConverter;
import se.s312563.lab4.entity.RoleEntity;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.payload.UserDto;
import se.s312563.lab4.repository.RoleRepo;
import se.s312563.lab4.repository.UserRepo;

import java.util.Collection;
import java.util.Set;
import java.util.stream.Collectors;

@Service
@RequiredArgsConstructor
@Slf4j
public class UserServiceImpl implements UserService {

    private final UserRepo userRepo;
    private final RoleRepo roleRepo;

    @Override
    public UserDto saveUser(UserDto userDto) {
        UserEntity userEntity = UserEntityDtoConverter.dtoToEntity(userDto);
        // making roles persistent from transient
        Set<RoleEntity> rolePersistSet = userEntity.getRoleSet().stream()
                .map(rt -> roleRepo.findByRoleName(rt.getRoleName()))
                .collect(Collectors.toSet());
        userEntity.setRoleSet(rolePersistSet);
        UserEntity savedEntity = userRepo.save(userEntity);
        return UserEntityDtoConverter.entityToDto(savedEntity);
    }

    @Override
    public Collection<UserDto> getUsers() {
        return userRepo.findAll().stream().map(UserEntityDtoConverter::entityToDto).collect(Collectors.toList());
    }

    @Override
    public boolean userExistByName(String username) {
        return (userRepo.findByUsername(username) != null);
    }

    @Override
    public boolean userExistById(Long id) {
        return userRepo.findById(id).isPresent();
    }
}
