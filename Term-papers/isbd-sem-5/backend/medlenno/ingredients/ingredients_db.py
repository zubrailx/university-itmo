from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String
from sqlalchemy.orm import Mapped, mapped_column, relationship

from medlenno.common.config import Base
from medlenno.common.sqla import db
from medlenno.ingredients.units_db import Unit


class Ingredient(Base):
    __tablename__ = "ingredients"

    id: Mapped[int] = mapped_column(primary_key=True)
    name: Mapped[str] = mapped_column(String(255), unique=True, index=True)
    description: Mapped[str | None]

    unit_id: Mapped[int] = mapped_column(ForeignKey("units.id"))
    unit: Mapped[Unit] = relationship(Unit, lazy="joined")

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
    def search(cls, search: str) -> Sequence[Self]:
        return db.get_all(select(cls).filter(cls.name.ilike(f"%{search}%")))
