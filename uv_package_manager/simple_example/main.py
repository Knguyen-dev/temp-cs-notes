from enum import Enum

from fastapi import FastAPI


class ModelName(str, Enum):
    alexnet = "alexnet"
    resnet = "resnet"
    lenet = "lenet"
    
app = FastAPI()



"""
The name of the parameter is "file_path", whilst :path is the part that the parameter 
should match any path.
"""
@app.get("/files/{file_path:path}")
async def read_file(file_path: str):
    return {"file_path": file_path}


# Here we enforce the the path parameters to be a certain enum
@app.get("/models/{model_name}")
async def get_model(model_name: ModelName):
    if model_name is ModelName.alexnet:
        return {"model_name": model_name, "message": "Deep Learning FTW!"}

    if model_name.value == "lenet":
        return {"model_name": model_name, "message": "LeCNN all the images"}

    return {"model_name": model_name, "message": "Have some residuals"}

# Query params 
fake_items_db = [{"item_name": "Foo"}, {"item_name": "Bar"}, {"item_name": "Baz"}]
@app.get("/items/")
async def read_item(skip: int = 0, limit: int = 10):
    return fake_items_db[skip : skip + limit]

# item_id is converted into integer
# FastAPI handles sending error response when item_id can't be parsed into an intege
@app.get("/items/{item_id}")
async def read_item(item_id: int):
    return {"item_id": item_id}

@app.get("/hello")
def main():
    return {"message": "Hello from simple-example!"}
    

@app.get("/")
async def root():
    return {"message": "Hello World"}


if __name__ == "__main__":
    main()
