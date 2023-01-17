from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String
from sqlalchemy.orm import Mapped, mapped_column, relationship

from medlenno.common.config import Base
from medlenno.common.sqla import db
from medlenno.ingredients.ingredients_db import Ingredient
from medlenno.users.users_db import User


class Price(Base):
    __tablename__ = "prices"

    id: Mapped[int] = mapped_column(primary_key=True)

    supplier_id: Mapped[int] = mapped_column(ForeignKey("suppliers.id"))
    supplier: Mapped[User] = relationship(User)

    ingredient_id: Mapped[int] = mapped_column(ForeignKey("ingredients.id"))
    ingredient: Mapped[User] = relationship(Ingredient)

    nominal_amount: Mapped[int]
    nominal_cost: Mapped[int]

    min_count: Mapped[int]
    max_count: Mapped[int]

    @classmethod
    def find_by_id(cls, price_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=price_id)

    @classmethod
    def find_by_ids(cls, price_id: int, supplier_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=price_id, supplier_id=supplier_id)

    @classmethod
    def find_by_supplier(cls, supplier_id: int) -> Sequence[Self]:
        return cls.find_all_by_kwargs(supplier_id=supplier_id)

    @classmethod
    def find_by_ingredient(cls, ingredient_id: int) -> Sequence[Self]:
        return cls.find_all_by_kwargs(ingredient_id=ingredient_id)
