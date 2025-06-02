# Amortized Algorithms
Amortized time is a way to express the time complexity when an algorithm has a really bad time complexity only once in a while besides the time complexity that happens most of the time. Other definitions would be the average time taken per operation, if you do many operations. 

A good example would be array list (Dynamic Array), which is a data structure that contains an array and can be extended. When an array list hits its maximum capacity, it internally creates a new array with double the size of the old array, and copies all the current elements over to the new array.

As a result, there are two main time complexities:
- Adding to the end of the array: $O(1)$. I mean we're just accessing an existing empty slot and putting a value in it.
- Resizing the array: $O(n)$, all n values are copied to the new array. 

So like most of the time inserting is $O(1)$, but every once in a while we'd have to do an $O(n)$ resize alongside inserting. To express tehse two time complexities, we use **amortized time**. Think of it as letting us describe the worse case that happens once in a while everytime the internal array hits its capacity. 

You can say: Insertion takes $O(n)$ when the capacity has been reached, and the amortized time for each insertion is $O(1)$. 

To get even more technical, insertion takes $O(2X)$, when the capacity of $X$ has been reached, and the the amortized time for each insertion is $O(1)$


## Credits
- [Amortized Time Complexity of Algorithms - Medium](https://medium.com/@satorusasozaki/amortized-time-in-the-time-complexity-of-an-algorithm-6dd9a5d38045)
- [Amortized Time Complexity - Wikipedia](https://en.wikipedia.org/wiki/Amortized_analysis)
- [Amortized Algorithms - CS Cornell University](https://www.cs.cornell.edu/courses/cs312/2006sp/lectures/lec18.html)
