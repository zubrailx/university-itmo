from typing import Sequence

from fastapi import APIRouter

from medlenno.common.models import PydanticModel
from medlenno.ingredients.units_db import Unit

controller = APIRouter(prefix="/units", tags=["units"])


class UnitOut(PydanticModel):
    id: int
    name: str


@controller.get("/", response_model=list[UnitOut])
def get_all_units() -> Sequence[Unit]:
    return Unit.find_all()


@controller.post("/", response_model=UnitOut)
def create_unit(name: str):
    unit = Unit.find_by_name(name)
    if unit is None:
        unit = Unit.create(name=name)
    return unit
