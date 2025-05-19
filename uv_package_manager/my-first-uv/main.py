from fastapi import FastAPI, Response
import requests

app = FastAPI()


@app.get("/")
def main():
    # By default it's json, so you need to use response object

    
    return Response("Hello from my-first-uv!", media_type="text/plain")


if __name__ == "__main__":
    main()
