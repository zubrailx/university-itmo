from contextvars import ContextVar
from typing import Optional, Sequence, TypeVar, Self

from sqlalchemy import Row, Select, select
from sqlalchemy.orm import Session, sessionmaker as Sessionmaker
from starlette.middleware.base import BaseHTTPMiddleware, RequestResponseEndpoint
from starlette.requests import Request
from starlette.responses import Response
from starlette.types import ASGIApp

session: ContextVar[Optional[Session]] = ContextVar("session", default=None)


class DBSessionMiddleware(BaseHTTPMiddleware):
    def __init__(self, app: ASGIApp, sessionmaker: Sessionmaker):
        super().__init__(app)
        self.sessionmaker = sessionmaker
        self.token = None

    def __enter__(self):
        self.token = session.set(self.sessionmaker())
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is not None:
            session.get().rollback()
        else:
            session.get().commit()

        session.get().close()
        # session.reset(self.token)

    async def dispatch(
        self, request: Request, call_next: RequestResponseEndpoint
    ) -> Response:
        with self:
            response = await call_next(request)
        return response


t = TypeVar("t")


class DBController:
    @property
    def session(self) -> Session:
        """Return an instance of Session local to the current context"""
        result = session.get()
        if result is None:
            raise ValueError("Session not initialized")
        return result

    def get_first(self, stmt: Select[t]) -> t | None:
        return self.session.scalars(stmt).first()

    def get_first_row(self, stmt: Select) -> Row | None:
        return self.session.execute(stmt).first()

    def get_all(self, stmt: Select[t]) -> Sequence[t]:
        return self.session.scalars(stmt).all()

    def get_all_rows(self, stmt: Select) -> Sequence[Row]:
        return self.session.execute(stmt).all()

    def get_paginated(self, stmt: Select[t], offset: int, limit: int) -> Sequence[t]:
        return self.get_all(stmt.offset(offset).limit(limit))

    def get_paginated_rows(
        self, stmt: Select, offset: int, limit: int
    ) -> Sequence[Row]:
        return self.get_all_rows(stmt.offset(offset).limit(limit))


db: DBController = DBController()


class MappingBase:
    @classmethod
    def create(cls, **kwargs) -> Self:
        entry = cls(**kwargs)  # noqa
        db.session.add(entry)
        db.session.flush()
        return entry

    @classmethod
    def select_by_kwargs(cls, *order_by, **kwargs) -> Select:
        if len(order_by) == 0:
            return select(cls).filter_by(**kwargs)
        return select(cls).filter_by(**kwargs).order_by(*order_by)

    @classmethod
    def find_first_by_kwargs(cls, *order_by, **kwargs) -> Self | None:
        return db.get_first(cls.select_by_kwargs(*order_by, **kwargs))

    @classmethod
    def find_all_by_kwargs(cls, *order_by, **kwargs) -> Sequence[Self]:
        return db.get_all(cls.select_by_kwargs(*order_by, **kwargs))

    @classmethod
    def find_paginated_by_kwargs(
        cls, offset: int, limit: int, *order_by, **kwargs
    ) -> Sequence[Self]:
        return db.get_paginated(
            cls.select_by_kwargs(*order_by, **kwargs), offset, limit
        )

    def update(self, **kwargs) -> None:
        for key, value in kwargs.items():
            setattr(self, key, value)

    def delete(self) -> None:
        db.session.delete(self)
        db.session.flush()
