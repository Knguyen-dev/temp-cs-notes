from fastapi.testclient import TestClient 
from main import app 


client = TestClient(app)


def test_main():
  response = client.get("/")
  assert response.status_code == 200
  assert response.json() == "Hello from my-first-uv!"
  