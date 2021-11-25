package se.s312563.lab4.entity;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import javax.persistence.*;
import java.util.Objects;

@Getter
@Setter
@NoArgsConstructor
@Entity
@Table(name = "refresh_tokens")
public class RefreshTokenEntity {

    @Id
    @Column(nullable = false, unique = true)
    private String refreshToken;

    @OneToOne
    @JoinColumn(name = "user_id", referencedColumnName = "id", unique = true)
    private UserEntity userEntity;

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RefreshTokenEntity that = (RefreshTokenEntity) o;
        return Objects.equals(refreshToken, that.refreshToken);
    }

    @Override
    public int hashCode() {
        return Objects.hash(refreshToken);
    }
}
