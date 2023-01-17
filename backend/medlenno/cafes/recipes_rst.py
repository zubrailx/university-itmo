from typing import Sequence

from fastapi import APIRouter, HTTPException, status
from pydantic import Field

from medlenno.cafes.recipes_db import Recipe, Recipe2Ingredient
from medlenno.common.models import PydanticModel, AllOptionalMeta, SuccessModel
from medlenno.ingredients.ingredients_rst import IngredientOut

controller = APIRouter(prefix="/recipes", tags=["recipes"])


class RecipeIn(PydanticModel):
    name: str
    description: str | None
    calories: int | None


class RecipeEdit(RecipeIn, metaclass=AllOptionalMeta):
    pass


class RecipeOut(RecipeIn):
    id: int
    ingredients: list[IngredientOut]


@controller.get("/", response_model=list[RecipeOut])
def get_all_recipes() -> Sequence[Recipe]:
    return Recipe.find_all()


@controller.post("/", response_model=RecipeOut)
def create_recipe(data: RecipeIn) -> Recipe:
    return Recipe.create(**data.dict())


@controller.get("/{recipe_id}/", response_model=RecipeOut)
def get_recipe(recipe_id: int) -> Recipe:
    recipe = Recipe.find_by_id(recipe_id)
    if recipe is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    return recipe


@controller.put("/{recipe_id}/", response_model=RecipeOut)
def edit_recipe(data: RecipeEdit, recipe_id: int) -> Recipe:
    recipe = Recipe.find_by_id(recipe_id)
    if recipe is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    recipe.update(**data.dict())
    return recipe


class RecipeItemEdit(PydanticModel):
    recipe_id: int = Field(alias="main")
    ingredient_id: int = Field(alias="item")
    amount: int | None
    required: bool | None


@controller.get("/{recipe_id}/items/")
def get_ingredients_from_recipe(recipe_id: int) -> list[dict]:
    return [
        {
            "item": entry.ingredient_id,
            "amount": entry.amount,
            "required": entry.required,
        }
        for entry in Recipe2Ingredient.find_by_recipe(recipe_id)
    ]


@controller.post("/items/")
def add_ingredient_to_recipe(data: RecipeItemEdit) -> SuccessModel:
    item = Recipe2Ingredient.find_by_ids(data.recipe_id, data.ingredient_id)
    if item is None:
        Recipe2Ingredient.create(**data.dict())
    else:
        item.update(**data.dict())
    return SuccessModel()


@controller.delete("/items/")
def remove_ingredient_from_recipe(data: RecipeItemEdit) -> SuccessModel:
    item = Recipe2Ingredient.find_by_ids(data.recipe_id, data.ingredient_id)
    if item is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    item.delete()
    return SuccessModel()
