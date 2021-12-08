package se.s312563.lab4.service;

import se.s312563.lab4.payload.UserDto;

import java.util.Collection;

public interface UserService {
    UserDto saveUser(UserDto userDto);

    Collection<UserDto> getUsers();

    boolean userExistByName(String username);

    boolean userExistById(Long id);
}
