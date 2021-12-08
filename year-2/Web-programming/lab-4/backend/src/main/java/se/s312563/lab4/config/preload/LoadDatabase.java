package se.s312563.lab4.config.preload;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.boot.CommandLineRunner;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import se.s312563.lab4.entity.RoleEntity;
import se.s312563.lab4.security.UserRole;
import se.s312563.lab4.service.PointService;
import se.s312563.lab4.service.RoleService;
import se.s312563.lab4.service.UserService;

import java.util.Arrays;

@Configuration
public class LoadDatabase {

    private static final Logger log = LoggerFactory.getLogger(LoadDatabase.class);

    @Bean
    CommandLineRunner runPoint(PointService pointService) {
        return args -> {
//            log.info("Preloading " + pointService.savePoint(new PointDto(12, 32, 13)));
//            log.info("Preloading " + pointService.savePoint(new PointDto(12, 22, 2)));
//            log.info("Preloading " + pointService.savePoint(new PointDto(12, 62, 16)));
//            log.info("Preloading " + pointService.savePoint(new PointDto(12, 72, 23)));
        };
    }

    @Bean
    CommandLineRunner runUser(UserService userService) {
        return args -> {
//            log.info("Preloading " + userService.saveUser(new UserDto("daughter", "kdfja")));
//            log.info("Preloading " + userService.saveUser(new UserDto("brotehr", "kga3131")));
        };
    }

    // saving roles
    @Bean
    CommandLineRunner runRoles(RoleService roleService) {
        return args -> {
            Arrays.stream(UserRole.values()).forEach(role ->
                    log.info("Preloading " + roleService.saveRole(new RoleEntity(role))));
        };
    }
}
