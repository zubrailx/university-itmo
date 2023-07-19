import pymongo
import faust

import os
import json
import logging
import traceback

from pymongo.collection import Collection
from ctypes import c_int64

def get_env_and_check(name: str, required: bool):
    env = os.environ.get(name)
    if required and env is None:
        print(f'{name} is not set.')
        exit(1)
    return env

APP_MONGO_SOCKET        = get_env_and_check('APP_MONGO_SOCKET', True)
APP_MONGO_REPLICA_SET   = get_env_and_check('APP_MONGO_REPLICA_SET', False)
APP_MONGO_DATABASE      = get_env_and_check('APP_MONGO_DATABASE', True)
APP_MONGO_COLLECTION    = get_env_and_check('APP_MONGO_COLLECTION', True)
APP_KAFKA_BROKER        = get_env_and_check('APP_KAFKA_BROKER', False)
APP_KAFKA_TOPIC         = get_env_and_check('APP_KAFKA_TOPIC', True)

assert(APP_MONGO_SOCKET is not None)
assert(APP_MONGO_DATABASE is not None)
assert(APP_MONGO_COLLECTION is not None)
assert(APP_KAFKA_BROKER is not None)
assert(APP_KAFKA_TOPIC is not None)


def get_connection_string():
    con_str = f"mongodb://{APP_MONGO_SOCKET}/"

    if APP_MONGO_REPLICA_SET:
        con_str += f"?replicaSet={APP_MONGO_REPLICA_SET}"

    return con_str

def get_database(con_str: str):
    client = pymongo.MongoClient(con_str)
    return client[APP_MONGO_DATABASE]

connection_string = get_connection_string()
database = get_database(connection_string)
collection = database[APP_MONGO_COLLECTION]


def get_payload(query: dict) -> dict:
    return json.loads(query["payload"])

def get_op_type(payload: dict) -> str:
    return payload["operationType"]

def get_full_document(payload: dict) -> dict:
    return payload["fullDocument"]


def update_document_schema(document: dict) -> None:
    # Number(Int32) -> String
    document["_id"] = str(document["_id"])
    # Number(Int32) -> Long(Int64)
    document["age"] = c_int64(document["age"]).value
    del document["sex"]

def get_updated_document(payload: dict) -> dict:
    document = get_full_document(payload)
    update_document_schema(document)
    return document

def get_primary_key(document: dict) -> dict:
    return { "_id": document["_id"] }


def execute_insert(collection: Collection, payload: dict):
    document = get_updated_document(payload)
    collection.insert_one(document)

def execute_update(collection: Collection, payload: dict):
    document = get_updated_document(payload)
    pk = get_primary_key(document)
    collection.replace_one(pk, document)

def execute_drop(collection: Collection, payload: dict):
    document = get_updated_document(payload)
    pk = get_primary_key(document)
    collection.delete_one(pk)

def execute(collection, payload: dict, operation: str):
    try:
        if (operation == "insert"):
            execute_insert(collection, payload)
        elif (operation == "update"):
            execute_update(collection, payload)
# NOTE: Isn't implemented by source driver
        # elif (operation == "drop"):
        #     execute_drop(collection, payload)
        else:
            logging.warn(f"Unsupported operation type: {operation}")

    except Exception:
        logging.error(traceback.format_exc())


app = faust.App('app', broker='kafka://' + APP_KAFKA_BROKER)
topic = app.topic(APP_KAFKA_TOPIC)

@app.agent(topic)
async def process_topic(stream):
    async for query in stream:
        payload = get_payload(query)
        operation = get_op_type(payload)
        execute(collection, payload, operation)
