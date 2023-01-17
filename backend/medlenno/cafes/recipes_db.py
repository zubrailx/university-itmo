from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String, and_, delete
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
    def find_all(cls) -> Sequence[Self]:
        return cls.find_all_by_kwargs()

    @classmethod
    def find_by_id(cls, recipe_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=recipe_id)

    @property
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
    __tablename__ = "recipe2ingredient"

    recipe_id: Mapped[int] = mapped_column(
        ForeignKey("recipes.id", ondelete="CASCADE"),
        primary_key=True,
    )
    recipe: Mapped[Recipe] = relationship()

    ingredient_id: Mapped[int] = mapped_column(
        ForeignKey("ingredients.id", ondelete="CASCADE"),
        primary_key=True,
    )
    ingredient: Mapped[Ingredient] = relationship()

    amount: Mapped[int] = mapped_column(default=1)
    required: Mapped[bool] = mapped_column(default=False)

    @classmethod
    def find_by_ids(cls, recipe_id: int, ingredient_id: int) -> Self | None:
        return cls.find_first_by_kwargs(
            recipe_id=recipe_id,
            ingredient_id=ingredient_id,
        )

    @classmethod
    def find_by_recipe(cls, recipe_id: int) -> Sequence[Self]:
        return cls.find_all_by_kwargs(recipe_id=recipe_id)
