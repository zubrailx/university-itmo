from typing import Sequence

from fastapi import APIRouter, HTTPException, status

from medlenno.cafes.recipes_db import Recipe
from medlenno.common.models import PydanticModel, AllOptionalMeta
from medlenno.ingredients.ingredients_rst import IngredientOut

controller = APIRouter(prefix="/recipes", tags=["recipes"])


class RecipeBase(PydanticModel):
    name: str
    description: str | None
    calories: int | None


class RecipeIn(RecipeBase):
    ingredients: list[int]


class RecipeEdit(RecipeIn, metaclass=AllOptionalMeta):
    pass


class RecipeOut(RecipeBase):
    id: int
    ingredients: list[IngredientOut]


@controller.get("/", response_model=list[RecipeOut])
def get_all_recipes() -> Sequence[Recipe]:
    return Recipe.find_all()


@controller.post("/", response_model=RecipeOut)
def create_recipe(data: RecipeIn) -> Recipe:
    data = data.dict()
    ingredients = data.pop("ingredients")
    recipe = Recipe.create(**data)
    recipe.update_ingredients(ingredients)
    return recipe


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

    data = data.dict()
    ingredients = data.pop("ingredients", None)
    recipe.update(**data)
    if ingredients is not None:
        recipe.update_ingredients(ingredients)
    return recipe
