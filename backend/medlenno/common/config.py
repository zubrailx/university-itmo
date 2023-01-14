from os import getenv
from pathlib import Path

from dotenv import load_dotenv
from sqlalchemy import create_engine, MetaData
from sqlalchemy.orm import sessionmaker as Sessionmaker, DeclarativeBase

from medlenno.common.sqla import MappingBase

current_directory = Path.cwd()
if current_directory.name == "medlenno":  # pragma: no cover
    current_directory = current_directory.parent

load_dotenv(current_directory / ".env")

# indexes, unique & check constraints, foreign & primary key namings
convention = {
    "ix": "ix_%(column_0_label)s",  # noqa: WPS323
    "uq": "uq_%(table_name)s_%(column_0_name)s",  # noqa: WPS323
    "ck": "ck_%(table_name)s_%(constraint_name)s",  # noqa: WPS323
    "fk": "fk_%(table_name)s_%(column_0_name)s_%(referred_table_name)s",  # noqa: WPS323
    "pk": "pk_%(table_name)s",  # noqa: WPS323
}

db_url: str = getenv("DB_LINK")
if db_url is None:
    raise EnvironmentError("No database URL (DB_LINK) specified")

engine = create_engine(db_url, pool_recycle=280, echo=True)
db_meta = MetaData(naming_convention=convention)
sessionmaker = Sessionmaker(bind=engine)


class Base(DeclarativeBase, MappingBase):
    metadata = db_meta
