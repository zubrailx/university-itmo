from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String, and_
from sqlalchemy.orm import Mapped, mapped_column, relationship

from medlenno.common.config import Base
from medlenno.common.sqla import db
from medlenno.ingredients.ingredients_db import Ingredient


class Recipe(Base):
    __tablename__ = "recipes"

    id: Mapped[int] = mapped_column(primary_key=True)
    name: Mapped[str] = mapped_column(String(255))
    description: Mapped[str | None]
    calories: Mapped[int | None]

    @classmethod
    def find_by_id(cls, recipe_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=recipe_id)

    def ingredients(self) -> Sequence[Ingredient]:
        return db.get_all(
            select(Ingredient).join(
                Recipe2Ingredient,
                and_(
                    Ingredient.id == Recipe2Ingredient.ingredient_id,
                    Recipe2Ingredient.recipe_id == self.id,
                ),
            )
        )


class Recipe2Ingredient(Base):
    id: Mapped[int] = mapped_column(primary_key=True)

    recipe_id: Mapped[int] = mapped_column(ForeignKey("recipes.id"))
    recipe: Mapped[Recipe] = relationship()

    ingredient_id: Mapped[int] = mapped_column(ForeignKey("ingredients.id"))
    ingredient: Mapped[Ingredient] = relationship()
