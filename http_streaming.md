

# HTTP streaming

### What is it?
HTTP streaming is the idea of transmitting multimedia content (video, audio) over the internet using HTTP. It's going to divide the data into small chunks, which are delivered sequentially in a continuous data stream to the client.


### Streaming vs Downloading
- Traditional file downloading: Entire file must be downloaded before playback can start.
- Streaming: Playback is immediate since we send small sequential chunks over HTTP

### HTTP Streaming vs Traditional Streaming
- Traditional Streaming: Direct continuous connections between client and server. Though it requires specific network configs and servers. This limits scalability and compatibility.
- HTTP Streaming: Uses standard web servers. More scalable, but less suited for large scale content distribution.

### HTTP Streaming vs Long Polling
- HTTP Streaming: Servers pushes data to the client over a single long-lived connection. But it's real time data delivery e.g. the file download happens immediately. 
- Long Polling: You could be waiting a while to get that response from the server. So this is better for use-cases where you're not going to get the data immediately.


## Credits: 
- [HTTP Streaming (Long Guide) - PubNub](https://www.pubnub.com/guides/http-streaming/)
-