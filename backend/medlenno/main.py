from fastapi import FastAPI
from fastapi.security import OAuth2PasswordBearer

from medlenno.common.config import sessionmaker, db_meta, engine, manager
from medlenno.common.sqla import DBSessionMiddleware
from medlenno.users.auth_rst import controller as auth_controller
from medlenno.users.users_db import User

app = FastAPI()
oauth2_scheme = OAuth2PasswordBearer(tokenUrl="sign-in")
app.add_middleware(DBSessionMiddleware, sessionmaker=sessionmaker)

app.include_router(auth_controller)


@manager.user_loader()
def load_user(user_id: str):
    try:
        return User.find_by_id(int(user_id))
    except ValueError:
        return None


# db_meta.drop_all(bind=engine)  # to drop everything from the database
db_meta.create_all(bind=engine)
