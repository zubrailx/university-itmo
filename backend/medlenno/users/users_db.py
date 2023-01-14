from typing import Self

from passlib.hash import pbkdf2_sha256
from sqlalchemy import String
from sqlalchemy.orm import Mapped, mapped_column

from medlenno.common.config import Base


class User(Base):
    __tablename__ = "users"

    id: Mapped[int] = mapped_column(primary_key=True)
    username: Mapped[str] = mapped_column(String(100), unique=True, index=True)
    password: Mapped[str] = mapped_column(String(100))

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
    def create(cls, username: str, password: str) -> Self:
        return super().create(username=username, password=cls.generate_hash(password))

    def verify_password(self, password: str) -> bool:
        return pbkdf2_sha256.verify(password, self.password)
