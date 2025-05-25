# Content Delivery Networks (CDNs)

### What is a CDN?
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-I/content-delivery-network/cdn-map.png)
A globally distributed network of proxy servers that are intended to serve content from locations that are closer to the user. Typically these serve static files such as HTML/CSS/JS, photos, and videos. This vastly increases load times. For example, a person in Japan will now see the photos on your simple website from Tokyo, rather than from Boston, where you're hosting the site. As a result, they'll have noticeable shorter wait times when seeing the photo.

However some CDNs such as Amazon's CloudFront allow you to serve dynamic content. Cloudflare which is just a web service package, is a free alternative. Anyways serving content from CDNS is pretty good as users get content from data centers closer to them, and your CDNs handle some requests that servers don't have to. I mean the whole point of this is that you don't have servers all over the world, and this helps solve some of that.

---
### Pull CDNs
Cache is updated base on requests. Here's what happens:
1. When the client sends a request that require sstatic assets from the CDN and the CDN doesn't have it.
2. CDN fetches those assets from the origin server, updates itself with this new content and sends this content back to the user.

It requires less maintenance since the CDN will automatically pull from your origin servers when its own data expires. It works with heavy traffic sites.

---
### Push CDNs
Push CDNs receive new content whenever changes occur on the server. We're responsible for providing content, uploading it to the CDN, and rewriting URLs to point to the CDN. We can configure when that content expires and when it's updated though, and content is uploaded only change it's new or changed. 

Sites with small amounts of traffic or sites where the content isn't updated as much work well with push CDNs. Content is placedon the CDNs once instead of being re-pulled at regular intervals.

---
### Which to choose
Let's say you have a travel site that hosts video and podcasts (large download sizes), a push CDN is probably cheaper and more efficient. This is because you send the CDN the content, and it only needs to process it once. No need to re-download the videos to your CDNs, unless you manually push.

A pull CDN is good for high-traffic-small-download sites as you're going to keep the most popular content on the CDN servers. Subsequent updates/pulls for content aren't going to happen often and operation is going to be cheaper than a push CDN.

---
### Examples of CDNs
Things like Amazon CloudFront, Google Cloud CDN, and Cloudflare CDN.

# Credits:
1. [Push vs Pull CDNs](https://www.travelblogadvice.com/technical/the-differences-between-push-and-pull-cdns/)