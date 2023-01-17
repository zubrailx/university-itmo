from __future__ import annotations

from typing import Self

from passlib.hash import pbkdf2_sha256
from sqlalchemy import String, ForeignKey
from sqlalchemy.orm import Mapped, mapped_column, relationship

from medlenno.common.config import Base


class User(Base):
    __tablename__ = "suppliers"

    id: Mapped[int] = mapped_column(primary_key=True)
    username: Mapped[str] = mapped_column(String(100), unique=True, index=True)
    password: Mapped[str] = mapped_column(String(100))

    name: Mapped[str] = mapped_column(String(255))
    description: Mapped[str | None]
    address: Mapped[str | None]

    cafe: Mapped[Cafe | None] = relationship(back_populates="user", lazy="joined")

    @staticmethod
    def generate_hash(password: str) -> str:
        return pbkdf2_sha256.hash(password)

    @classmethod
    def find_by_id(cls, user_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=user_id)

    @classmethod
    def find_by_username(cls, username: str) -> Self | None:
        return cls.find_first_by_kwargs(username=username)

    @classmethod
    def create(cls, *, password: str, as_cafe: bool, **kwargs) -> Self:
        password = cls.generate_hash(password)
        result: Self = super().create(password=password, **kwargs)
        if as_cafe:
            Cafe.create(id=result.id, menu_count=0)
        return result

    def verify_password(self, password: str) -> bool:
        return pbkdf2_sha256.verify(password, self.password)


class Cafe(Base):
    __tablename__ = "cafes"

    id: Mapped[int] = mapped_column(ForeignKey("suppliers.id"), primary_key=True)
    user: Mapped[User] = relationship(back_populates="cafe", lazy="joined")

    menu_count: Mapped[int] = mapped_column(default=0)
