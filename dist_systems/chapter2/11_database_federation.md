# Database Federation
![](https://raw.githubusercontent.com/karanpratapsingh/portfolio/master/public/static/courses/system-design/chapter-II/database-federation/database-federation.png)
Divides the database by function architecturally, but to the user it should still feel like one logical database to end users. So your service would query data from a couple of data source in order to do things. So the federated database is "virtual" in this sense.

---
### Key Characters For A Federated Database 
- **Transparency:** The user shouldn't realize that it's a federated database, they shouldn't know how it's stored. If you're getting their order history, they shouldn't need to go on different pages or click different buttons to get the orders, order items, etc. They should just open the page, the service compiles the data from different services, and they see the data.
- **heterogeneity:** It should be able to interact with different types of databases SQL, Graph, document, etc. in a unified way.
- **Extensibility:** Should be easy to add new data sources.
- **Autonomy:** Your federated database shouldn't affect or modify the actual data sources or cause them to change.

---
### Considerations With Database Federation
Here's why database federation is good. You can access heterogeneous data in a unified way. Separation of concerns. However there's going to be additional complexity as this adds more servers, joins from two different databases is going to be complex, and this affects your latency and scalability.