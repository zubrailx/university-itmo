from typing import Sequence

from fastapi import APIRouter, Depends, HTTPException, status

from medlenno.common.config import manager
from medlenno.common.models import PydanticModel, AllOptionalMeta
from medlenno.ingredients.ingredients_rst import IngredientOut
from medlenno.suppliers.prices_db import Price
from medlenno.users.users_db import User

controller = APIRouter(prefix="/prices", tags=["prices"])


class PriceBase(PydanticModel):
    nominal_amount: int
    nominal_cost: int
    min_count: int
    max_count: int


class PriceIn(PriceBase):
    ingredient_id: int


class PriceEdit(PriceIn, metaclass=AllOptionalMeta):
    pass


class PriceOwnerOut(PriceBase):
    id: int
    ingredient: IngredientOut


@controller.get("/", response_model=list[PriceOwnerOut])
def get_my_prices(user: User = Depends(manager)) -> Sequence[Price]:
    return Price.find_by_supplier(user.id)


@controller.post("/", response_model=PriceOwnerOut)
def create_price(data: PriceIn, user: User = Depends(manager)) -> Price:
    return Price.create(**data.dict(), supplier_id=user.id)


@controller.get("/{price_id}/", response_model=PriceOwnerOut)
def get_price(price_id: int, user: User = Depends(manager)):
    return Price.find_by_ids(price_id, user.id)


@controller.put("/{price_id}/", response_model=PriceOwnerOut)
def edit_price(price_id: int, data: PriceEdit, user: User = Depends(manager)):
    price = Price.find_by_ids(price_id, user.id)
    if price is None:
        raise HTTPException(status.HTTP_404_NOT_FOUND)
    price.update(**data.dict())
    return price
