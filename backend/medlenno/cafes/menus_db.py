from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String, and_
from sqlalchemy.orm import Mapped, mapped_column, relationship

from medlenno.cafes.recipes_db import Recipe
from medlenno.common.config import Base
from medlenno.common.sqla import db
from medlenno.users.users_db import Cafe


class Menu(Base):
    __tablename__ = "menus"

    id: Mapped[int] = mapped_column(primary_key=True)
    name: Mapped[str] = mapped_column(String(255))
    description: Mapped[str | None]
    recipe_count: Mapped[int]

    @classmethod
    def find_by_id(cls, menu_id: int) -> Self | None:
        return cls.find_first_by_kwargs(id=menu_id)

    @classmethod
    def find_by_cafe(cls, cafe_id: int) -> Sequence[Self]:
        return db.get_all(
            select(cls).join(
                Cafe2Menu,
                and_(cls.id == Cafe2Menu.menu_id, Cafe2Menu.cafe_id == cafe_id),
            )
        )

    def recipes(self) -> Sequence[Recipe]:
        return db.get_all(
            select(Recipe).join(
                Menu2Recipe,
                and_(
                    Recipe.id == Menu2Recipe.recipe_id,
                    Menu2Recipe.menu_id == self.id,
                ),
            )
        )


class Cafe2Menu(Base):
    id: Mapped[int] = mapped_column(primary_key=True)

    cafe_id: Mapped[int] = mapped_column(ForeignKey("cafes.id"))
    cafe: Mapped[Cafe] = relationship()

    menu_id: Mapped[int] = mapped_column(ForeignKey("menus.id"))
    menu: Mapped[Menu] = relationship()


class Menu2Recipe(Base):
    id: Mapped[int] = mapped_column(primary_key=True)

    menu_id: Mapped[int] = mapped_column(ForeignKey("menus.id"))
    menu: Mapped[Menu] = relationship()

    recipe_id: Mapped[int] = mapped_column(ForeignKey("recipes.id"))
    recipe: Mapped[Recipe] = relationship()
