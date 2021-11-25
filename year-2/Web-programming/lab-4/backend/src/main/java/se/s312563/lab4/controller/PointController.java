package se.s312563.lab4.controller;

import lombok.RequiredArgsConstructor;
import org.springframework.http.ResponseEntity;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.*;
import se.s312563.lab4.exception.AnonymousUserException;
import se.s312563.lab4.exception.InvalidRequestException;
import se.s312563.lab4.exception.PointNotFoundException;
import se.s312563.lab4.exception.PointNotSavedException;
import se.s312563.lab4.payload.PointDto;
import se.s312563.lab4.payload.request.PointRemoveRequest;
import se.s312563.lab4.payload.request.PointSaveRequest;
import se.s312563.lab4.payload.response.MessageResponse;
import se.s312563.lab4.security.UserRole;
import se.s312563.lab4.security.userDetails.CustomUserDetails;
import se.s312563.lab4.service.PointService;
import se.s312563.lab4.service.UserService;

import java.util.Collection;
import java.util.Optional;
import java.util.stream.Collectors;

@RestController
@RequiredArgsConstructor
@RequestMapping("api/v1/point")
public class PointController {

    private final PointService pointService;
    private final UserService userService;

    @PostMapping("/save")
    public ResponseEntity<?> savePoint(@RequestBody PointSaveRequest req) {
        try {
            checkSaveRequest(req);
            Long userId = ((CustomUserDetails) SecurityContextHolder.getContext().getAuthentication().getPrincipal()).getId();
            return save(userId, req);

        } catch (AnonymousUserException | PointNotSavedException | InvalidRequestException ex) {
            return ResponseEntity.badRequest().body(new MessageResponse(ex.getMessage()));
        }
    }

    // admin
    @PostMapping("/save/{userId}")
    public ResponseEntity<?> savePointByUserId(@PathVariable(value = "userId") Long userId, @RequestBody PointSaveRequest req) {
        try {
            checkSaveRequest(req);
            return save(userId, req);

        } catch (InvalidRequestException | AnonymousUserException ex) {
            ResponseEntity.badRequest().body(new MessageResponse(ex.getMessage()));
        }
        return ResponseEntity.ok().body(userId);
    }

    private void checkSaveRequest(PointSaveRequest req) throws InvalidRequestException, AnonymousUserException {
        if (req.getCoordinateX() == null || req.getCoordinateY() == null) {
            throw new InvalidRequestException("Request data is invalid.");
        }
        if (req.getRadius() == null || req.getRadius().isInfinite() || req.getRadius().isNaN()) {
            throw new InvalidRequestException("Request data is invalid");
        }
    }

    private ResponseEntity<?> save(Long userId, PointSaveRequest req) throws PointNotSavedException {
        PointDto pointDto = PointDto.newBuilder()
                .setUserId(userId)
                .setCoordinateX(req.getCoordinateX())
                .setCoordinateY(req.getCoordinateY())
                .setRadius(req.getRadius()).build();
        Optional<PointDto> pointDtoOptional = pointService.savePoint(pointDto);
        if (!pointDtoOptional.isPresent()) {
            throw new PointNotSavedException("Point has not been saved.");
        }
        return ResponseEntity.ok().body(pointDtoOptional.get());
    }


    @GetMapping("/get")
    public ResponseEntity<?> getByPointId(@RequestParam Long pointId) {
        try {
            Optional<PointDto> pointDtoOptional = pointService.getPointByPointId(pointId);
            if (!pointDtoOptional.isPresent()) {
                throw new PointNotFoundException(String.format("Point with id = %d is not found in database.", pointId));
            } else {
                return ResponseEntity.ok().body(pointDtoOptional.get());
            }
        } catch (PointNotFoundException ex) {
            return ResponseEntity.badRequest().body(new MessageResponse(ex.getMessage()));
        }
    }

    @GetMapping("/get/{userId}")
    public ResponseEntity<?> getByUserId(@PathVariable(value = "userId") Long userId) {
        if (userService.userExistById(userId)) {
            Collection<PointDto> pointDtoCollection = pointService.getPointsByUserId(userId);
            return ResponseEntity.ok().body(pointDtoCollection);
        } else {
            return ResponseEntity.badRequest()
                    .body(new MessageResponse(String.format("User with id = %d is not found in database", userId)));
        }
    }


    @PostMapping("/remove")
    public ResponseEntity<?> removeByPointId(@RequestBody Long pointId) {
        Optional<PointDto> pointDtoOptional = pointService.removePointByPointId(pointId);
        if (pointDtoOptional.isPresent()) {
            return ResponseEntity.ok().body(pointDtoOptional.get());
        } else {
            return ResponseEntity.badRequest()
                    .body(new MessageResponse(String.format("Point with id = %d is not found in database.", pointId)));
        }
    }

    @PostMapping("/remove_many")
    public ResponseEntity<?> removeByPointIdMany(@RequestBody PointRemoveRequest req) {
        Collection<Long> pointIds = req.getPointIds();
        if (pointIds == null) {
            return ResponseEntity.badRequest()
                    .body(new MessageResponse("Point list is null"));
        } else {
            CustomUserDetails principal = ((CustomUserDetails) SecurityContextHolder.getContext().getAuthentication().getPrincipal());
            // if not ROLE_ADMIN
            if (!principal.getAuthorities().contains(new SimpleGrantedAuthority(UserRole.ROLE_ADMIN.getAuthority()))) {
                Long userId = principal.getId();
                pointIds = pointService.getPointsByUserId(userId).stream()
                        .map(PointDto::getPointId).filter(pointIds::contains).collect(Collectors.toList());
            }
            return ResponseEntity.ok().body(pointService.removePointByPointIdMany(pointIds));
        }
    }

    // admin
    @PostMapping("/remove/{userId}")
    public ResponseEntity<?> removeByUserId(@PathVariable(value = "userId") Long userId) {
        if (userService.userExistById(userId)) {
            Collection<PointDto> pointDtoCollection = pointService.removePointsByUserId(userId);
            return ResponseEntity.ok().body(pointDtoCollection);
        } else {
            return ResponseEntity.badRequest()
                    .body(new MessageResponse(String.format("User with id = %d is not found in database", userId)));
        }
    }


}
