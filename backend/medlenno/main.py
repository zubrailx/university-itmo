from fastapi import FastAPI

from medlenno.common.config import sessionmaker
from medlenno.common.sqla import DBSessionMiddleware

app = FastAPI()
app.add_middleware(DBSessionMiddleware, sessionmaker=sessionmaker)


@app.get("/")
async def root():
    return {"message": "Hello World"}
