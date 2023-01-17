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

    def update_recipes(self, recipes: list[int]) -> None:
        db.session.execute(delete(Menu2Recipe).filter_by(recipe_id=self.id))
        for recipe in recipes:
            Menu2Recipe.create(menu_id=self.id, recipe_id=recipe)

    def attach_cafe(self, cafe_id: int) -> None:
        Cafe2Menu.create(cafe_id=cafe_id, menu_id=self.id)


class Cafe2Menu(Base):
    __tablename__ = "cafe2menu"

    id: Mapped[int] = mapped_column(primary_key=True)

    cafe_id: Mapped[int] = mapped_column(ForeignKey("cafes.id", ondelete="CASCADE"))
    cafe: Mapped[Cafe] = relationship()

    menu_id: Mapped[int] = mapped_column(ForeignKey("menus.id", ondelete="CASCADE"))
    menu: Mapped[Menu] = relationship()


class Menu2Recipe(Base):
    __tablename__ = "menu2recipe"

    id: Mapped[int] = mapped_column(primary_key=True)

    menu_id: Mapped[int] = mapped_column(ForeignKey("menus.id", ondelete="CASCADE"))
    menu: Mapped[Menu] = relationship()

    recipe_id: Mapped[int] = mapped_column(ForeignKey("recipes.id", ondelete="CASCADE"))
    recipe: Mapped[Recipe] = relationship()
