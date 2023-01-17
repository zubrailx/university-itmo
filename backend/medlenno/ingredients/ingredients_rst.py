from typing import Sequence

from fastapi import APIRouter, HTTPException, status

from medlenno.common.models import PydanticModel, AllOptionalMeta
from medlenno.ingredients.ingredients_db import Ingredient
from medlenno.ingredients.units_db import Unit
from medlenno.ingredients.units_rst import UnitOut

controller = APIRouter(prefix="/ingredients", tags=["ingredients"])


class IngredientBase(PydanticModel):
    name: str
    description: str | None


class IngredientIn(IngredientBase):
    unit: str


class IngredientEdit(IngredientIn, metaclass=AllOptionalMeta):
    pass


class IngredientOut(IngredientBase):
    id: int
    unit: UnitOut


@controller.get("/", response_model=list[IngredientOut])
def get_all_ingredients(search: str | None = None) -> Sequence[Ingredient]:
    if search is None:
        return Ingredient.find_all()
    return Ingredient.search(search)


@controller.post("/", response_model=IngredientOut)
def create_ingredient(data: IngredientIn) -> Ingredient:
    ingredient = Ingredient.find_by_name(data.name)

    data = data.dict()
    data["unit"] = Unit.find_or_create(data["unit"])

    if ingredient is None:
        ingredient = Ingredient.create(**data)
    else:
        ingredient.update(**data)
    return ingredient


@controller.get("/{ingredient_id}/", response_model=IngredientOut)
def get_ingredient(ingredient_id: int) -> Ingredient:
    ingredient = Ingredient.find_by_id(ingredient_id)
    if ingredient is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    return ingredient


@controller.put("/{ingredient_id}/", response_model=IngredientOut)
def edit_ingredient(data: IngredientEdit, ingredient_id: int) -> Ingredient:
    ingredient = Ingredient.find_by_id(ingredient_id)
    if ingredient is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)

    data = data.dict()
    data["unit"] = Unit.find_or_create(data["unit"])
    ingredient.update(**data)

    return ingredient
