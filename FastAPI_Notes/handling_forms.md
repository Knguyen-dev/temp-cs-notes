# Handling Form Data

### Sections
- Form Data
- Form Models 
- Request files 
- request forms and files


### Form Data
Use `Form` when we want to receive form fields instead of JSON. When talking about form fields, we mean the individual inputs that are submitted through an HTML form. When submitting a form, the data is sent using either:
- `application/x-www-form-urlencoded` (default for most forms)
```
username=alex&password=12345
```
- `multipart/form-data` (use when sending form fields and files):
```
--boundary
Content-Disposition: form-data; name="username"

alex
--boundary
Content-Disposition: form-data; name="avatar"; filename="photo.png"
Content-Type: image/png

(binary image data)
--boundary--
```

So essentially:
- Form fields: The old school way browsers send form data, which is great for form and file uploads
- JSON: Modern format for structured data; great for APIs, but not for files.


### Form Models
You could also use Pydantic to create a "FormData" class that contains all of the fields that you expect to receive. So FastAPI will extract that form data for each field and give us a class instance containing all the data.

### Request Files
You can define files to be uploaded from the client using `File` class.
```Python
from fastapi import FastAPI, File, UploadFile

app = FastAPI()

# We're expecting a file, however FastAPI will read the file and we'll receive the file as bytes
# Good for smaller files.
@app.post("/files/")
async def create_file(file: bytes = File()):
    return {"file_size": len(file)}

# Here are some several advantages:
# - Don't have to use File() as a default parameter
# - It works well for larger files. So if under max limit, it's stored in memory, else it will be stored in disk.
# - Able to receive metadata from uploaded files
# Here are some attributes:
# - filename, content_type, and file, which is the actual Python file object. You can also write and read data to the file, traverse the file, etc.
# All of those methods are async methods, so we'll need to await them.
@app.post("/uploadfile/")
async def create_upload_file(file: UploadFile):
    return {"filename": file.filename}
```

- **Files will be uploaded as Form Data**. So you're not going to able to send that stuff as jSON.
- You can make a file option, using type annotations and setting a default value of None:
```Python

# File is either an array of bytes or Nothing, and it seems we default to nothing
@app.post("/files/")
async def create_file(file: Annotated[bytes | None, File()] = None):
    if not file:
        return {"message": "No file sent"}
    else:
        return {"file_size": len(file)}


# File is either an upload file or none, and we default ot none
@app.post("/uploadfile/")
async def create_upload_file(file: UploadFile | None = None):
    if not file:
        return {"message": "No upload file sent"}
    else:
        return {"filename": file.filename}
```

- **use form data:** FastAPI will read it from form data instead of JSON.  If you use `File` FastAPI will expect multipart/form-data instead of the regular `application/x-www-form-urlencoded`.
- You can also upload multiple files at the same time, though they'll be associated with the same form field and must be sent using form data, so native.

TLDR: You use File, bytes, and UploadFile to declare files to be uploaded in teh request, and this is sent as form data, not as JSON.

### Request Form and Files: True Multipart
You can define files and formfields, allowing you to get both types of data. So the data will be sent to your endpoint as form dat  and you'll receive the files and form fields. You may let some be bytes and other be UploadFile




However the only rule is that you need to make it more concise than the docs. So of course you'll lose information, but you'll focus on critical information.

Read up on the documentation for handling forms in fastAPI. It shouldn't take more than an hour or two
https://fastapi.tiangolo.com/tutorial/request-forms/
