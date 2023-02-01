from fastapi import APIRouter, Depends

from medlenno.common.config import manager
from medlenno.common.models import PydanticModel, AllOptionalMeta
from medlenno.users.auth_rst import UserBase
from medlenno.users.users_db import User

controller = APIRouter(prefix="/users/me", tags=["users"])


class CafeOut(PydanticModel):
    menu_count: int


class UserOut(UserBase):
    cafe: CafeOut | None


class UserEdit(UserBase, metaclass=AllOptionalMeta):
    pass


@controller.get("/data/", response_model=UserOut)
def get_user_data(user: User = Depends(manager)):
    return user


@controller.put("/data/", response_model=UserOut)
def edit_user_data(data: UserEdit, user: User = Depends(manager)):
    user.update(**data.dict())
    return user
