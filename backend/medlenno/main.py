from fastapi import FastAPI
from fastapi.security import OAuth2PasswordBearer
from starlette.middleware.cors import CORSMiddleware

from medlenno.cafes import recipes_rst, menus_rst
from medlenno.common.config import sessionmaker, db_meta, engine, manager
from medlenno.common.sqla import DBSessionMiddleware
from medlenno.ingredients import units_rst, ingredients_rst
from medlenno.users import auth_rst, users_rst
from medlenno.users.users_db import User

app = FastAPI()

oauth2_scheme = OAuth2PasswordBearer(tokenUrl="/sign-in-form/")
app.add_middleware(DBSessionMiddleware, sessionmaker=sessionmaker)

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

app.include_router(auth_rst.controller)
app.include_router(users_rst.controller)

app.include_router(units_rst.controller)
app.include_router(ingredients_rst.controller)

app.include_router(recipes_rst.controller)
app.include_router(menus_rst.controller)


@manager.user_loader()
def load_user(user_id: str):
    try:
        return User.find_by_id(int(user_id))
    except ValueError:
        return None


# db_meta.drop_all(bind=engine)  # to drop everything from the database
db_meta.create_all(bind=engine)
