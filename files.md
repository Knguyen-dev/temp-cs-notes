
# File Formats, MIME Types, and Detection Methods

### Review on mime types and encoding
- **MIME types:** A MIME type is the format used to describe the nature and format of a file or data. In HTTP, the `Content-Type` header uses a MIME type to tell the client what kind of data is being sent. Files can also have MIME types, though they aren't stored directly. We have file extensions that are mapped to MIME types and OS/browsers use this to guess the MIME type of a file when it's being opened or served. 

- **Encoding and file contents:** The contents of files can be encoded in different ways. It could be UTF-8, UTF-16, ISO-8895, etc. If the file has special characters or non-ASCII data, using the wrong encoding could corrupt the files or lead to reading/parsing errors. So if our system expects UTF-8 and someone uploads a UTF-16 files, we could have some unexpected behavior. 

This is important for stuff such as:
- Ensuring security (e.g. preventing malicious file uploads)
- Interoperability between systems, as we can have correct rendering (e.g. showing an image or parsing text) regardless of the encoding strategy used to encode the file contents.


### How to do file format detection

#### Approach 1: Use file extensions
At the surface level you may check the file extension, however file extensions aren't actually enforced, basically a suggestion. If you open up `something.pdf` and it's not a pdf, you will probably have issues seeing the contents. It could also be spoofed.

#### Approach 2: Use MIME Type (Media Type)
You should realize that this MIME type would be set as http headers and are commonly guessed from file extensions. Meaning this is still spoofable. As well as thsi not all file formats are registered e.g. `.dat` and `.pqi` will go under `application/octet-stream` (a generic stream of bytes). However the more common file formats such as `.png` will get the `image/png` MIME Type.

#### Approach 3: File Signature
A unique pattern of bytes at the start of a file that identifies the format. This is harde rt ospoof and is more reliable for format detection, but not all file formats have these bytes at 
the start of the file.

#### Conclusion
I don't think anyone will suggest looking at the magic numbers or bytes at the start of a file to detect its format. In most cases you should probably just check the file extension and the MIME Type. Unless you're letting your user send in executables or software scripts that can be run and hurt your system, you should just stick to approach 1 and 2. 

Also think about how the files are going to be used in your application. If you have an application where users are sending in `.csv` files where they can visualize data, I don't think there are going to be major security or fatal issues if someone tries to send a `.txt` or a `.py` file. 

Just make sure you aren't executing the file, you are sanitizing the input name to prevent path traversal, placing limits on file size, etc. You should be good.




#### Byte Order Mark
At the beginning of a webpage that uses Unicode character encoding you may find some bytes that represent the Unicode  the code point U+FEFF. This is called the Byte-Order-Mark (BOM), and it's usualy




