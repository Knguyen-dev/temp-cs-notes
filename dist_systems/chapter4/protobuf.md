# Protobuf (Protocol Buffers)

A language-agnostic data serialization mechanism. Developers define the structure in a `.proto` file, which is the used to **generate source code** that can read and write from different data streams. 

### Quick History Lesson
Protobuf was originally developed by Google engineers who needed an efficient way to serialize structured data across various internal services. They decided release it as an open source project, and it gained traction.

Google then released gRPC (Google Procedure Call), which is a schema driven framework that facilitates service-to-service communication in distributed environments. Protobuf was the preferred data format for working with gRPC APIs, and so they both grew in popularity together.

### How it works
Protobuf uses a binary data format, which is more compact and faster to read/write than text-based formats. It also provides an interface definition language (IDL) that makes it easy to define the structure of the data to be serialized. A Protobuf file is saved using the `.proto` file is written in Protobuf's IDL format, and contains all the information about the structure of your data:
```proto
syntax = "proto3"
message Customer {
  required int32 id = 1;
  required string name = 2;
  required string email = 3;
  optional string address = 4;
}
```
In this example, the Customer message contains four fields: `id`, `name`, `email`, and `address`. You see the data types for each, and whether they're required to exist on the Customer message.  The cool part is that you can compile this file into several programming languages using "Protoc", which is Protobuf's compiler. This compiler generates source code in the target language you want, which will include classes and methods for reading/writing the message type defined in the `.proto` file.

Then the workflow is that you create instances of those generated classes. You fill them with your data and then they're serialized into binary. When you ready the data, the binary is de-serialized back into instances of your classes generated by your protofile. The binary data format is platform independent, allowing applications running on different programming languages nad platforms to communicate with each other.

### Advantages and best practices
I mean it's strongly typed and requires developers to write a clear schema that the data must follow, which other formats don't really do. It is efficient as it's serializing data into binary which makes storage more compact and reduces bandwidth. This could be really good for mobile networks or IoT devices. 

Write clear schemas, use clear names, try not to use the `Any` type. Also try to optimize for data size and efficiency, so choosing something like int64 or uint32 based on the expected range of values. Also you should maintain backward and forward compatibility. This means avoiding the removal or modification of existing fields, and versioning your Protobuf files to track changes over time.



## Credits
- [Introduction to Protobuf](https://blog.postman.com/what-is-protobuf/)