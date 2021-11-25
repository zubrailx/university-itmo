package se.s312563.lab4.entity;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;
import se.s312563.lab4.security.UserRole;

import javax.persistence.*;
import java.util.Objects;

@Getter
@Setter
@NoArgsConstructor
@Entity
@Table(name = "roles")
public class RoleEntity {
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private long id;

    @Enumerated(EnumType.STRING)
    @Column(name = "name", unique = true, nullable = false)
    private UserRole roleName;


    public RoleEntity(UserRole roleName) {
        this.roleName = roleName;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RoleEntity that = (RoleEntity) o;
        return Objects.equals(roleName, that.roleName);
    }

    @Override
    public int hashCode() {
        return Objects.hash(roleName);
    }
}
