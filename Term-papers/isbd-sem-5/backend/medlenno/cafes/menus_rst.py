from typing import Sequence

from fastapi import APIRouter, HTTPException, status, Depends
from pydantic import Field

from medlenno.cafes.menus_db import Menu, Menu2Recipe
from medlenno.common.config import manager
from medlenno.common.models import PydanticModel, AllOptionalMeta, SuccessModel
from medlenno.cafes.recipes_rst import RecipeOut
from medlenno.users.users_db import User

controller = APIRouter(prefix="/menus", tags=["menus"])


class MenuIn(PydanticModel):
    name: str
    description: str | None


class MenuEdit(MenuIn, metaclass=AllOptionalMeta):
    pass


class MenuOut(MenuIn):
    id: int
    recipes: list[RecipeOut]


@controller.get("/", response_model=list[MenuOut])
def get_all_menus(user: User = Depends(manager)) -> Sequence[Menu]:
    if user.cafe is None:
        raise HTTPException(status.HTTP_403_FORBIDDEN)
    return Menu.find_by_cafe(user.cafe.id)


@controller.post("/", response_model=MenuOut)
def create_menu(data: MenuIn, user: User = Depends(manager)) -> Menu:
    if user.cafe is None:
        raise HTTPException(status.HTTP_403_FORBIDDEN)
    menu = Menu.create(**data.dict(), recipe_count=0)
    menu.attach_cafe(user.cafe.id)
    return menu


@controller.get("/{menu_id}/", response_model=MenuOut)
def get_menu(menu_id: int) -> Menu:
    menu = Menu.find_by_id(menu_id)
    if menu is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    return menu


@controller.put("/{menu_id}/", response_model=MenuOut)
def edit_menu(data: MenuEdit, menu_id: int) -> Menu:
    menu = Menu.find_by_id(menu_id)
    if menu is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    menu.update(**data.dict())
    return menu


@controller.delete("/{menu_id}/")
def delete_menu(menu_id: int) -> SuccessModel:
    menu = Menu.find_by_id(menu_id)
    if menu is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    menu.delete()
    return SuccessModel()


class MenuItemEdit(PydanticModel):
    menu_id: int = Field(alias="main")
    recipe_id: int = Field(alias="item")
    price: int | None


@controller.get("/{menu_id}/items/")
def get_ingredients_from_recipe(menu_id: int) -> list[dict]:
    return [
        {"item": entry.recipe_id, "amount": entry.price}
        for entry in Menu2Recipe.find_by_menu(menu_id)
    ]


@controller.post("/items/")
def add_ingredient_to_recipe(data: MenuItemEdit) -> SuccessModel:
    item = Menu2Recipe.find_by_ids(data.recipe_id, data.menu_id)
    if item is None:
        Menu2Recipe.create(**data.dict())
    elif data.price is not None:
        item.price = data.price
    return SuccessModel()


@controller.delete("/items/")
def remove_ingredient_from_recipe(data: MenuItemEdit) -> SuccessModel:
    item = Menu2Recipe.find_by_ids(data.recipe_id, data.menu_id)
    if item is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    item.delete()
    return SuccessModel()
