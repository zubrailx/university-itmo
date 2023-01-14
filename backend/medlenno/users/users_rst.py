from fastapi import APIRouter, Depends

from medlenno.common.config import manager
from medlenno.common.models import PydanticModel
from medlenno.users.auth_rst import UserBase
from medlenno.users.users_db import User

controller = APIRouter()


class CafeOut(PydanticModel):
    menu_count: int


class UserOut(UserBase):
    cafe: CafeOut | None


@controller.get("/users/me/data/", tags=["users"], response_model=UserOut)
def get_user_data(user: User = Depends(manager)):
    return user
