from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import String
from sqlalchemy.orm import Mapped, mapped_column

from medlenno.common.config import Base


class Unit(Base):
    __tablename__ = "units"

    id: Mapped[int] = mapped_column(primary_key=True)
    name: Mapped[str] = mapped_column(String(255), unique=True, index=True)

    @classmethod
    def find_by_id(cls, unit_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=unit_id)

    @classmethod
    def find_by_name(cls, name: str) -> Self | None:
        return cls.find_first_by_kwargs(name=name)

    @classmethod
    def find_all(cls) -> Sequence[Self]:
        return cls.find_all_by_kwargs()

    @classmethod
    def find_or_create(cls, name: str) -> Self:
        result = cls.find_by_name(name)
        if result is None:
            result = cls.create(name=name)
        return result
