from typing import Self

from fastapi import APIRouter, Depends
from fastapi.security import OAuth2PasswordRequestForm
from fastapi_login.exceptions import InvalidCredentialsException
from pydantic import BaseModel

from medlenno.common.config import manager
from medlenno.users.users_db import User

controller = APIRouter()


class UserAuth(BaseModel):
    id: int
    access_token: str

    @classmethod
    def from_user(cls, user: User) -> Self:
        access_token = manager.create_access_token(data={"sub": user.id})
        return UserAuth(id=user.id, access_token=access_token)


@controller.post("/sign-up/", tags=["users"])
def signup(data: OAuth2PasswordRequestForm = Depends()) -> UserAuth:
    if User.find_by_username(data.username) is not None:
        raise InvalidCredentialsException()

    user = User.create(username=data.username, password=data.password)
    return UserAuth.from_user(user)


@controller.post("/sign-in/", tags=["users"])
def signin(data: OAuth2PasswordRequestForm = Depends()) -> UserAuth:
    user = User.find_by_username(data.username)
    if user is None or not user.verify_password(data.password):
        raise InvalidCredentialsException()
    return UserAuth.from_user(user)
