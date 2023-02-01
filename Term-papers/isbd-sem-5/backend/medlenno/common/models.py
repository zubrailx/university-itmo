from typing import Optional

from pydantic import BaseModel
from pydantic.main import ModelMetaclass


class PydanticModel(BaseModel):
    def dict(self, *args, **kwargs) -> dict:
        kwargs["exclude_none"] = True
        return super().dict(*args, **kwargs)

    class Config:
        orm_mode = True


class SuccessModel(PydanticModel):
    message: str = "success"


class AllOptionalMeta(ModelMetaclass):
    def __new__(mcs, name: str, bases: tuple[type], namespaces: dict[str, ...], **kwargs):
        annotations: dict = namespaces.get('__annotations__', {})
        for base in bases:
            for base_ in base.__mro__:
                if base_ is BaseModel:
                    break
                annotations.update(base_.__annotations__)
        for field in annotations:
            if not field.startswith('__'):
                annotations[field] = Optional[annotations[field]]
        namespaces['__annotations__'] = annotations
        return super().__new__(mcs, name, bases, namespaces, **kwargs)
