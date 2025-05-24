# DP Part 2 Continued
Here we're just going to go over different problems, and learn 

### Problem 1: Minimum Edit Distance 
Given two strings `word1` and `word2`, return the minimum number of operations required to convert `word1` into `word2`. So it's implied that you can't really change `word2`, so your operations are done on `word1`.

You have the following three operations permitted on a word:
- Insert a character, so you'd be inserting a character into `word1`.
- Delete a character, means you're deleting a character from `word1`
- Replace a character: Changing one character into another from `word1`.

#### Analysis and cases
If word1 = "horse" and word2 = "ros", here are our operations:
1. word1[0] = r; word1 = rorse
2. Remove word1[4] = 'e'; word1 = rors
3. Remove word1[2] = r; word1 = ros

That's a total of 3 operations. That we need to do.


Now let's look at some **base cases**:
- If words are empty: If word1 = word2 = "", then it takes zero operations.

- If words are equal: If word1 = word2 = "abc", then there are no operations to do. To think about is word1[0] = word2[0]. They're the same, so exclude them and let's consider the rest of the string. 

- If one is empty and other isn't: In the case that word1="abc" and word2="", then we would need to do 3 deletion operations. for them to match. The key thing here is that, the number of operations (num deletions) we needed to do, was just the length of `word1`. If word1 = "" and word2 = "abc", then we'd just do len(word2) insertions on word1, like copying 

At this point, the words just have equal lengths:
- Subproblem: Return the number of operations needed for a given word1 == word2. We need to reduce this sub-problem down to the base case.
- Recursive formula: 
  a. If the characters are equal, then we're going to recurse with the new subproblem being word1[i+1] and word2[j+1]. We wouldn't add +1 here when doing the call since this didn't require an operation.
  b. Else this means that the first characters aren't equal. An example is like "abd" and "acd":
    1. **Insertion:** In this case, we're copying word2[j] to word1[i-1], as this makes the front characters match and is our best guess at making the words match. So like "bd" and "cd" would be "cdb" and "cd". Now you can do `j+1` because we basically copied over j. This takes one operation. While insertion won't work for maybe word1="map" word2="bap", it will work for word1="ap" and word2="map".  
    2. **Deletion:** How about do word1[i] = b and word2[j] = c. Here we're guessing that maybe if we delete the char at i, and hopefully the remainder of word1 can match up. This takes one operation and i is shifted whilst j stays the same. Of course it won't work for "bca" and "cca", but it will work for "bca" and "ca".
    3. **Replacement:** Maybe if we replace the character it will match. We'd want word1 to match word2, so we'd do `word1[i] = word2[j]`, which matches the ith character from word1 to the jth character in word2. Won't work for "cab" and "lark", but will work for "cab" and "lab". But anyways, we've forced the two characters to match, so increment both pointers to get our new subproblem. Notice that replace does the exact same thing as if our characters were equal we're forcing them to be equal by using an operation. So the difference from the base case is that we're using an operation, so when recurring, we're going to do +1 to indicate a replacement operation was done.

We need to handle caching because at worse, this is $O(3^{n}) $ due to the potential of having 3 recursive calls per operation. This is 3 nodes, to 9, to 27 different subproblems.

A given subproblem is just calculating the min number of operations for a given word1 and word2, so like for word1="panda" and word2="skanra" a subproblem could be when i and j are enclosing "nda" and "nra". and this is going to be probably re-calculated multiple times, so an easy thing would be have the key of our cache be a tuple of the i and j pointers (which can be used to derive what substring they were enclosing), with the value as the min distance.

#### Visualizing it
It looks like a grid. An explanation of the grid is [here](https://www.youtube.com/watch?v=XYi2-LPrwm4). But essentially, how we increment our pointers is how how we're traversing said grid. Let i be the row index and j be the column index. In this case, incrementing i means we're going down, j means right, whilst both means diagonal. Then in each grid slot, we want to store the min edit distance for that position/state.

### Implementations
So all the dynamic programming problems we've seen so far, we start from the bottom of our tree, grid, etc. and then build to the top.

---
#### Approach 1: Recursive Approach
```Python
class Solution: 
  def __init__(self): 
    self.cache = {} 

  def helper(word1, word2, i, j) -> int:
    # If already in the cache

    # If first characters are equal, then 
    # the new subproblem is i+1 and j+1; no operations 
    if (word1[i] == word2[j]):
      return helper(word1, word2, i+1, j+1)
  
    # If first word is empty
    if (word1 == ""):
      return len(word2)

    # If second word is empty
    if (word2 == ""):
      return len(word1)

    '''
    Neither are empty strings and first characters don't match, so 
    calculate the minimum edit distances for inserting, deleting, and replacing.

    1. Insertion: The idea of inserting jth character in word1. So 
    2. Deletion: Idea of deleting the ith letter in word1.
    3. Replacement: Idea that we replace word1[i] with word1[j]

    All of these cost one operation that we'll accumulate.
    After all operations were done, get the minimum one
    '''
    minDist = 0
    insertionDist = 1 + helper(word1, word2, i, j+1)
    deletionDist = 1 + helper(word1, word2, i+1, j)
    replacementDist = 1 + helper(word1, word2, i+1, j+1)

    minDist = min(insertionDist, deletionDist)
    minDist = min(minDist, replacementDist)

    # Cache the result somehow 
    
    # Return the result?
    return minDist

  def minEditDistance(word1, word2) -> int:
    # Clear cache before starting
    self.cache = {}
    # Start the recursive function
    return helper(word1, word2, 0, 0)
```


#### Approach 2: Bottom up
```Python

```

## Credits:
https://www.youtube.com/watch?v=XYi2-LPrwm4
https://www.youtube.com/watch?v=nLmhmB6NzcM