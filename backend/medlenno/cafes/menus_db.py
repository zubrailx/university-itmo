from __future__ import annotations

from typing import Self, Sequence

from sqlalchemy import select, ForeignKey, String, and_, delete
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

    @property
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

    def attach_cafe(self, cafe_id: int) -> None:
        Cafe2Menu.create(cafe_id=cafe_id, menu_id=self.id)


class Cafe2Menu(Base):
    __tablename__ = "cafe2menu"

    cafe_id: Mapped[int] = mapped_column(
        ForeignKey("cafes.id", ondelete="CASCADE"),
        primary_key=True,
    )
    cafe: Mapped[Cafe] = relationship()

    menu_id: Mapped[int] = mapped_column(
        ForeignKey("menus.id", ondelete="CASCADE"),
        primary_key=True,
    )
    menu: Mapped[Menu] = relationship()

    @classmethod
    def find_by_ids(cls, cafe_id: int, menu_id: int) -> Self | None:
        return cls.find_first_by_kwargs(
            cafe_id=cafe_id,
            menu_id=menu_id,
        )


class Menu2Recipe(Base):
    __tablename__ = "menu2recipe"

    menu_id: Mapped[int] = mapped_column(
        ForeignKey("menus.id", ondelete="CASCADE"),
        primary_key=True,
    )
    menu: Mapped[Menu] = relationship()

    recipe_id: Mapped[int] = mapped_column(
        ForeignKey("recipes.id", ondelete="CASCADE"),
        primary_key=True,
    )
    recipe: Mapped[Recipe] = relationship()

    price: Mapped[int]

    @classmethod
    def find_by_ids(cls, menu_id: int, recipe_id: int) -> Self | None:
        return cls.find_first_by_kwargs(
            menu_id=menu_id,
            recipe_id=recipe_id,
        )
