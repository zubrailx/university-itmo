from datetime import timedelta
from typing import Self

from fastapi import APIRouter, Depends
from fastapi.security import OAuth2PasswordRequestForm
from fastapi_login.exceptions import InvalidCredentialsException

from medlenno.common.config import manager
from medlenno.common.models import PydanticModel
from medlenno.users.users_db import User

controller = APIRouter()


class SignInModel(PydanticModel):
    username: str
    password: str


class UserBase(PydanticModel):
    name: str
    description: str | None = None
    address: str | None = None


class UserIn(UserBase, SignInModel):
    as_cafe: bool = False


class UserAuth(PydanticModel):
    id: int
    access_token: str
    as_cafe: bool

    @classmethod
    def from_user(cls, user: User) -> Self:
        access_token = manager.create_access_token(
            data={"sub": user.id},
            expires=timedelta(hours=12),
        )
        return UserAuth(
            id=user.id,
            access_token=access_token,
            as_cafe=user.cafe is not None,
        )


@controller.post("/sign-up/", tags=["users"])
def signup(data: UserIn) -> UserAuth:
    if User.find_by_username(data.username) is not None:
        raise InvalidCredentialsException

    user = User.create(**data.dict())
    return UserAuth.from_user(user)


@controller.post("/sign-in-form/", tags=["users"], deprecated=True)
def signin(data: OAuth2PasswordRequestForm = Depends()) -> UserAuth:
    user = User.find_by_username(data.username)
    if user is None or not user.verify_password(data.password):
        raise InvalidCredentialsException
    return UserAuth.from_user(user)


@controller.post("/sign-in/", tags=["users"])
def signin(data: SignInModel) -> UserAuth:
    user = User.find_by_username(data.username)
    if user is None or not user.verify_password(data.password):
        raise InvalidCredentialsException
    return UserAuth.from_user(user)
