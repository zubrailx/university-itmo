package se.s312563.lab4.security.userDetails;

import lombok.RequiredArgsConstructor;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import se.s312563.lab4.entity.UserEntity;
import se.s312563.lab4.repository.UserRepo;

@Service
@RequiredArgsConstructor
public class CustomUserDetailsService implements UserDetailsService {

    private final UserRepo userRepo;

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        final UserEntity userEntity = userRepo.findByUsername(username);

        if (userEntity == null) {
            throw new UsernameNotFoundException("User " + username + " not found.");
        }

        return CustomUserDetails.build(userEntity);
    }
}
