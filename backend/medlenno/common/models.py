from pydantic import BaseModel


class PydanticModel(BaseModel):
    def dict(self, *args, **kwargs) -> dict:
        kwargs["exclude_none"] = True
        return super().dict(*args, **kwargs)

    class Config:
        orm_mode = True
