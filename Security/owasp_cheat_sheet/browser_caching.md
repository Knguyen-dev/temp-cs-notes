# Browser Caching 

### How browser caching works?
When you visit a website, your browser may cache some of the data it receives, such as images, scripts or even API responses. This makes future visits faster because the browser doesn't have to re-download everything.
- How does it decide what to cache? Well servers use headers like `Cache-Control` and `ETag` to tell the browser how long a response can be stored and reused. 
- Whilst caching will give us faster load times and reduced server load, remember that it comes at the sacrifice of fresh data. You must think about how time sensitive your data is before you have caching in your application.

However to control this, you'd have to modify the response from your server to have those headers. 

We can manually implement caching on the client side by using `localStorage` or `sessionStorage`. 