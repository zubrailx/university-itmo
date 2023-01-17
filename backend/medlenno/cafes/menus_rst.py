from typing import Sequence

from fastapi import APIRouter, HTTPException, status, Depends

from medlenno.cafes.menus_db import Menu
from medlenno.common.config import manager
from medlenno.common.models import PydanticModel, AllOptionalMeta, SuccessModel
from medlenno.cafes.recipes_rst import RecipeOut
from medlenno.users.users_db import User

controller = APIRouter(prefix="/menus", tags=["menus"])


class MenuBase(PydanticModel):
    name: str
    description: str | None


class MenuIn(MenuBase):
    recipes: list[int]


class MenuEdit(MenuIn, metaclass=AllOptionalMeta):
    pass


class MenuOut(MenuBase):
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
    data = data.dict()
    recipes = data.pop("recipes")
    data["recipe_count"] = len(recipes)
    menu = Menu.create(**data)
    menu.update_recipes(recipes)
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

    data = data.dict()
    recipes = data.pop("recipes", None)
    if recipes is not None:
        data["recipe_count"] = len(recipes)
        menu.update_recipes(recipes)
    menu.update(**data)
    return menu


@controller.delete("/{menu_id}/")
def delete_menu(menu_id: int) -> SuccessModel:
    menu = Menu.find_by_id(menu_id)
    if menu is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    menu.delete()
    return SuccessModel()
