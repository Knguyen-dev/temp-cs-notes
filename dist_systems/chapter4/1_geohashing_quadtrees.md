# Geohashing and quadtrees 

## What is geohashing?
Geohashing is a system for encoding geographic coordinates such as latitude and longitude into short alphanumeric strings. For example, Sanfrancisco has coordinates `37.7564, -122.4016` which can be represented in geohash as `9q8yy9mf`.

### How does Geohashing work?
A geohash is a hierarchical spatial index that uses Base-32 alphabet encoding. The first character identifies an initial location as one of 32 cells. Then inside this cell, we have 32 more cells, and so the next character would indicate the location being one out of 32 of these, and it keeps going. To represent a point, the world is recursively divided into smaller and smaller cells with each additional bit until a desired precision (for physical location) is attained. The amount of precision you want also determines the size of the cell.

**Note:** Indices are numbers in an array that determine the position. An index here is similar, it's a spatial index in the sense that it determines a physical location in the world. The reason we call it hierarchical is due to the recursive nature of the problem, and how you must read the characters in the indices in a specific order to get the correct position.

The more characters in a hash, the more precise we're trying to narrow the location down to. Locations that share prefixes will be closer, and the longer the prefix they share, the closer they are spatially. For example, geohashes `9q8yy9mf` and `9q8yy9vx` are spatially close as they share the prefix `9q8yy9`. Here's a table of geohash lengths and the size of the cells that they represent:
| Geohash Length | Cell Width | Cell Height |
| -------------- | ---------- | ----------- |
| 1              | 5,000 km   | 5,000 km    |
| 2              | 1,250 km   | 1,250 km    |
| 3              | 156 km     | 156 km      |
| 4              | 39.1 km    | 19.5 km     |
| 5              | 4.89 km    | 4.89 km     |
| 6              | 1.22 km    | 0.61 km     |
| 7              | 153 m      | 153 m       |
| 8              | 38.2 m     | 19.1 m      |
| 9              | 4.77 m     | 4.77 m      |
| 10             | 1.19 m     | 0.596 m     |
| 11             | 149 mm     | 149 mm      |
| 12             | 37.2 mm    | 18.6 mm     |

### Use Cases, Applications, and Motivations
Geohashing is one of the fundamental techniques used in modern location-based applications. geohashing is used in location-based games and services. Modern ride-sharing apps, food delivery services, and social networking platforms probably rely on geohashing to quickly find nearby drivers, restaurants, or friends.

Before geohashing, spatial indexing ("how we can represent physical locations digitally") relied primarily on complex tree-based structures. Geohashing lets spatial problems be seen as simple string operations instead of complex geometric calculations. Again geohashing is just one way to do spatial indexing, we'll talk about quadtrees next as a different method.

## What are quadtrees? 
A Quadtree is a tree data structure which is mainly used to divide 2D space by recursively splitting regions into four equal quadrants. Each node in the tree represents a spatial region, and leaf nodes contain the actual data points or indicate empty space.

### How do Quadtrees Work?
- Recursive subdivision: Like Geohash, a quadtree recursively divides a space, but instead of encoding the location data into a string, it creates a tree where each node has four children, each one representing a quadrant.
- Dynamic Structure: They're dynamically built based on the density of points in an area. When a region becomes "crowded" (contains too many points), the quadtree subdivides it further.
- Spatial Queries: Searching for points, regions, or nearest neighbors can be done efficiently by traversing the tree structure.

### Quadtree Example: Restaurant Discovery App
Remember the data we have refers to points of interests or events that we're trying to index spatially. Examples could be restaurants, stores, users, drivers, delivery personell, crime incidents, etc. However this data could be distributed unevenly. An urban area may have 1000+ restaurants per square kilometer, hundreds of uber drivers, etc. Whilst rural areas oculd have 1 restaurant per 100 square kilometers, a single Uber driver, and fewer points of interests. 

If you're building a restaurant discovery app. In Manhattan (dense urban area), a 6-character geohash covers about 1.22 by 0.61 kilometers could enclose 500+ restaurants. So when someone searches for "nearby restaurants" you'd get way too many results. Your database queries are slower since one geohash "bucket" is overloaded, the leaf node contains way too many points of interests. However in rural Kansas (sparse area), that same 6-character geohash could enclose maybe 1-2 restaurants within the same area. Not that many results, so we may need to search many neighboring geohash cells to more data points. And now we're doing multiple database queries and handling more complex logic. With geohashing, we could run into overpopulated cells, empty cells, and we lack adaptation.

Quadtrees would handle this application a lot better:
  - Subdivide dense areas into smaller cells until each cell has a manageable number of data points.
  - Keep sparse (rural) areas as large cells since there's no need to subdivide empty space.
  - Adapt automatically, so as data is removed or inserted, we'll adjust our tree.

## Types of Spatial Queries
There are different types of spatial queries:
- **Proximity Search:** Finds items that are close to a given location/point based on some distance metric (e.g. Euclidean distance). An example would be finding all restaurants within 2 kilometers of your current location.
- **Nearest Neighbor Search (KNN):** Find the k closest points to a given target point. An example could be finding the 5 nearest hospitals for a person. 
- **Range Search:** Finds all points within a specified range (bounding box or radius) from a query point or region. An example could be a delivery service where we find all customers in a rectangular area bounded by GPS coordinates.

## Comparing Geohash and and Quadtrees
Let''s do a final review of the key differences between Geohash and Quadtrees:
- **Data Structure:**
  - Geohash encodes geographic coordinates into a single alphanumeric string which compact and easy to store or transmit. A lightweight option for databases and distributed systems. 
  - Quadtree is a hierarchical tree structure that divides space recursively, where each node represents a region and each child is a subregion. It's more flexible and gives you more control, but you have more storage overhead.
- **Divisions and Precision Control:**
  - Geohash Divides the world into a fixed grid based on the length of the string. The longer the string, the more precise things are. Meaning you can represent even smaller locations, as you're able to narrow the scope more.
  - Quadtree adapts its divisions based on the data density.
- **Proximity Search:**
  - Geohash excels in proximity search as it just needs to compare prefixes. However, it could be inaccurate when you have points on the opposite sides of a boundary/wall, and so they have different geohashes since they're in different cells, despite actually being close.
  -  Quadtrees do proximity searches by traversing the tree, which is more ocmputationally expensive, but more accurate.
- **Dynamic Resolution:**
  - Geohash uses a fixed grid system, meaning it applies the same precision level to all cells, regardless of how much data is in each area. It lacks adaptability and can lead to inefficiencies, especially when working with highly uneven data distributions.
  - Quadtrees on the other hand offer dynamic resolution as they subdivide areas with higher data densities whilst leaving sparse areas with fewer divisions. This leads to more efficient querying and more even regions.
- **Handling Irregular Shapes:**
  - Geohash struggles to accurately represent irregular or non-rectangular such as coastlines or winding roads. The grid-based approach leads to imprecision, where areas outside the target region may still fall within the same Geohash.
  - Quadtree is better at handling irregularly shaped regions since it can adapt to the shape and density of the data. It can subdivide more to make sure to only enclose certain regions, allowing for more precise representations of complex geographic features like coastlines, or areas with irregular shapes.
- **Implementation and Indexing Complexity:**
  - Geohash is a lot easier to implemenet and its operations to encode, decode and proximity search are pretty efficient. You can easily index this in a database, as its just a string, use a string column.
  - Quadtrees are more complex and their operations are a little more computationally expensive, but you do get more flexibility and control when representing spatial data. This is harder to index in a database.

## When to use which? Alternatives?
Use Geohash when:
- Need a simple and compact way to store spatial data. 
- When proximity searches are common. Because remember geohashes use a hierarchical system where similar locations share the same prefix, simplifying proximity searches to comparing hash prefixes.
- Easier to work with distributed systems as the sring data cna be index in databases.

Use Quadtree when:
- When you need more precision and you want your data points to be evenly divided across your quadrants. Quadtrees have dynamic subdivisions, which allows us to offer more precision in dense areas.
- When you need to perform complex queries like range searches, nearest neighbor searches, etc.
- When you need to adjust to areas with more or fewer data points dynamically, you'll need to use quadtrees.

## Takeaway
The spatial indexing landscape evolves every day. Newer systems like Google's S2 geometry library based on spherical stuff or Uber's H3 hexagonal grid system could help out your usecase. In any case, the information here truly becomes useful when you start working on an application involving spatial indexing.

## Credits
- [Geohashing and Quadtrees - The Medium](https://medium.com/@agustin.ignacio.rossi/geohash-or-quadtree-ready-set-and-go-for-system-design-interviews-4fd81fb1049f)