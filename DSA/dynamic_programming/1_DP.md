
# Dynamic Programming 

## What is it?
A design technique that combines the correctness of brute force and the efficiency of greedy algorithms. There are multiple use cases, but some common ones are:
1. Finding an optimal solution.
2. Counting the total number of solutions for a problem.

With dynamic programming, there are two main ways of solving problems:
  1. **Recursion (possibly with memoization):** We can use recursion to traverse through our problem like it's a tree. This can sometimes be too long for certain problems, which is why we can memoize function results if we've already seen them. This prevents us from doing extra calculations. This is usually called the **Top-down approach**.
  2. **Bottom-up strategy:** An iterative approach to solving the problem. This can be used as well, and can help when the recursive solution produces too many function calls on the call stack.

But in general, your main bread and butter will be using recursion and memoization together. The biggest challenge you'll face, is how to think about the problems and separate them into sub-problems with recursion.

### Problem 1: Fibonacci Sequence
We want to create a function that yields the nth fibonacci number.

---
#### Naive Fibonacci Sequence
Here's the regular fibonacci function that finds the nth fibonacci number.
```Python
def fib(n):
  if (n==1 or n==2):
    return 1
  return fib(n-1) + fib(n-2)
```

Now for bigger numbers this gets a lot slower. Notice that if you treat this problem like you were traversing a binary tree, you'll notice that you're still re-calculating and recurring for values that you've already seen. For example if you've already recurred to get `fib(3)`, you already know it's value, so let's just cache it, and the use the cached value moving forward.

The time complexity is actually close about $O(2^{n})$. This is because in each step, in each n, you call your fib function twice. This will eventually amount to a $2^{n}$. If you visualize this in tree form, to calculate the nth number, this would generate maximum of $2^{n-1}$ leaf nodes, or $2^{n-1}$ function calls.

---
#### Using memoization
Let's use an array of length `n+1` that caches the values for us. So if trying to find the 5th fibonacci number, then we'd have a array of size 6. Once we recur down teh tree and get to `fib(1) = 1`, we'll store the value 1 in our 1st index. For `fib(2) = 1`, we'd store that in our second index. For `fib(3)`, we'd store the result in our third index. So what we're doing? We're storing the result of the nth fibonacci number in the nth index of our array. Now when recurring, we'll first check if we've already have the value in our array, if not then we'll recur. 
```Python
def fib(n, memo):
  # if already in the cache, use it
  if memo[n] != null:
    return memo[n]
  
  # Base cases
  if n == 1 or n == 2:
    return 1

  # Recursively calculate fibonacci for the nth sequence.
  result = fib(n-1) + fib(n-2)

  # After calculating, storing the result of the nth fibonacci number
  memo[n] = result

  # Now return it 
  return result
```
This should be a lot faster because now we always have a reference th our array where we store cached values. For that very first traversal down the tree, it's going to recur until the base case `fib(1)` and `fib(2)`. But after caching, this is all the function needs to calculate the rest of the sequence. It bubbles up back to n, resulting in each value from `fib(1) `to `fib(n)` to be computed exactly once. The time complexity for this is about 2n+1 = 2n = n, which is just $O(n)$

The only way this will fail, is when your fibonacci number is really big, and it makes our recursion function crash because we put too many function calls on the call stack. To solve this we can use the **bottom-up** approach.

---
#### Bottom up approach:
Another way to solve dynamic programming tasks is to use the bottom up approach. In our last example, we built our array from left to right recursively. We could also just use iteration to build our array. This is where we solve our sub-problems first. If you remember treating solving fibonacci like recurring down a tree, this is like starting from the bottom of that tree, and moving up.

```Python
def fib_bottom_up(n):

  if n == 1 or n == 2:
    return 1
  
  arr = new int[n+1]

  # Initialize 1st and 2nd fibonacci numbers
  arr[1] = 1
  arr[2] = 1

  # Iteratively construct the 3rd to nth numbers 
  for i in range(3, n+1):
    arr[i] = arr[i-1] + arr[i-2]
  
  # Return the nth number, which should be the last one in your array
  return arr[n]
```
And there we go our iterative, bottom-up approach to solving the problem. Of course you don't actually need an array for this, you can just achieve this probably 3 numbers that you keep updating.

#### Dependency Order Of Sub-Problems
When computing the ith fibonacci number, we're assuming that that i-1 and i-2 are already solved. This indicates that we solving our sub-problems, we must do them in a certain order and put some thought it that. 

Getting more theoretical, the sub-problems need to be solvable in a topologically sorted order. Meaning that if our problems were like a problem, there wouldn't be a cycle. This makes sense because that means there's a clear start and end for us.

### Problem 2: Minimum coins

---
#### Description and Greedy review
Given a set of coin values coins={c1, c2, ..., ck} and a target sum of money m, calculate the minimum number of coins that form the sum `m`.

For example, with euro coins = {1,2,5,10,20, 50, 100, 200} with target 734. The optimal solution is:
{200, 200, 200, 100, 20, 10, 1, 1}. Here we use the largest possible coin we can, which is called the greedy approach since we're taking the maximum (best) path we can see in front of us.

However as we know, greedy algorithms don't always produce the optimal approach. Imagine we had coins = {1,4,5} and target 13. The greedy approach yields {5,5,1,1,1} but the optimal solution is {4,4,5}.

---
#### Approaching a problem with dynamic programming 

What sub-problems do we need to solve? Sometimes your final question that you want to solve is your sub-problem. So our sub-problem involves finding the minimum number of coins to hit the target.

If this were recursion, the base case could be m=0, and we'd return 0 since none of the coins from our list would be needed. So if our target was 13, our tree would have root 13, and paths 1, 4, and 5 (weights) sticking out of them with respective nodes 12, 9, and 8. Of course, we only create possible paths. 

For each node, try all possible coins and take possible solutions. So here's the theoretical formula:
  - solution(0) = 0; m=0 won't yield any coins
  - solution(m) = minimum of solution(m-c) + 1; So each recursion adds +1 coins, and our function is going to result in a couple combinations of coins (paths). We're going to pick the path that took the least amount of coins/vertices, which will be the optimal path.


---
#### Naive Minimum Coins

```Python
# Some sub-problems may not have a solution, so we return none
# E.g. coins={10}, target = 9, it's going to return None since it's not going to finish.
def min_ignore_none(a,b):
  if a is None:
    return b
  if b is None:
    return a
  return min(a,b)

def minimum_coins(m, coins):
  # Base case: zero money, so zero coins needed
  if m == 0:
    return 0
  
  answer = None

  # Sum up 
  for coin in coins:
    
    # New subproblem/state we can expand
    subproblem = m - coin

    # If new state is invalid, then we can skip it to avoid processing it
    if subproblem < 0:
      continue
    
    # New state is valid, so continue recurring, and get the amount result (amount of coins needed for that path)
    result = minimum_coins(subproblem, coins) + 1

    # Update minimum if this new path was shorter than what's currently in answer
    # NOTE: Some paths may not result in an answer on how many coins, so we ignore them
    answer = min_ignore_none(answer, result)

  # After checking the lower sub-problems, return our answer.
  return answer
```

Let coins=[1,4,5]. Calculating when target=13 is easy, as we get 3. However doing this for target=150 is very slow, similar to fibonacci. The time complexity would probably be around $O(c^{m})$, where c is the number of coins, and m is our target.
  - At this step, the worse case is the function branches into $c$ recursive calls.
  - Then in the worse case we continue branching until $m=0$.
  - This leads to an exponential number of calls $O(c^{m})$

---
#### Memoized Minimum Coins
We can very easily add memoization by using an array on the outside

```Python
def min_ignore_none(a,b):
  if a is None:
    return b
  if b is None:
    return a
  return min(a,b)

memo = {}

def minimum_coins(m, coins):
  # If it's cached, return the cached version, else do the recursion logic.
  if m in memo:
    return memo[m]

  if m == 0:
    return 0
  answer = None
  for coin in coins:
    subproblem = m - coin
    if subproblem < 0:
      continue
        result = minimum_coins(subproblem, coins) + 1
    answer = min_ignore_none(answer, result)

  # Sub-problems done; Found the min amount of coins for a given value m, so cache it for future use
  # before returning it.
  memo[m] = answer

  return answer
```
With memoization, we cached the values. So `min_coins(2)` is only calculated once and never again. The time complexity would be now just multiplicative $O(c*m)$. This makes sense the same way the fibonacci version made sense. For your first traversal down, it's essentially a single iteration that calculates all the necessary values at m, and of course each has c branches. But after that, things are cached and aren't re-calculated.

---
#### Bottom-up for minimum coins:
```Python
def minimum_coins(m, coins):
  memo = {}

  # Kind of like our base case.
  memo[0] = 0

  # for all possible m values
  for i in range(1, m+1):

    for c in coins:
      subproblem = i - c
      
      # If it's not a possible state to expand, skip it
      if subproblem < 0:
        continue 
      
      # Calculate the coins for the subproblem, then include +1 for our current m
      answer = memo.get(subproblem) + 1

      # Get the minimum answer
      memo[i] = min_ignore_none(memo.get(i), answer)

  return memo[m]
```
Well this works because we're always starting from i=1, the smaller sub-problems and working our way up. 


### Problem 3: Coin problem, how many ways?
Given a set of coin values coins={c1, c2, ..., ck}, and a target sum m, how many ways (permutations) we can reach the sum using m coins.

For example, coins={1,4,5} and target = 5. There are 4 permutations of coins that sum up to the target:
1. {1,1,1,1,1}
2. {1,4}
3. {4,1}
4. {5}

Again split the problem into sub-problems. For the base case m = 0, there are no ways, but it 's a good marker for the end. We can return 1 for the base case to signal a complete way.

If you draw the recursive solution out like a tree, we keep expanding until all leaf nodes are 0, which indicate a valid permutation. Then bubble these back up the tree, summing up the number of paths for a given m. This results in our answer. Now let's do the bottom up approach.

#### Recursive and Memoized Approach
```Python
memo = {}
def how_many_ways(m, coins):

  # If the number of permutations at m is already in the cache, then use it, rather than
  # recurring down the tree more.
  if m in memo:
    return memo[m]
  
  # Base case: end of a path; think of it as "only one way to make 0, by choosing no coins"
  if m == 0:
    return 1

  # Start off with zero; for invalid paths, you can treat them as 0, as they don't add to our count of permutations.
  # NOTE: So if no coins created a new valid state, then this stays 0, returns a 0, which is good as it doesn't count towards the number of permutations
  ways = 0

  for c in coins:
    
    # Calculate new state to branch out to
    subproblem = m - c

    # If invalid, skip it
    if subproblem < 0:
      continue

    # Since it's valid expand it, recur it further to calculate the amount of permutations/paths 
    # from our subproblem
    sub_ways = how_many_ways(subproblem, coins)

    # Sum up the number of paths from our sub-problems
    ways += sub_ways
  
  # Don't forget to cache it
  memo[m] = ways

  return ways






  
```

---
#### Bottom up approach
```Python
from collections import defaultdict

def how_many_ways(m, coins):

  # Any missing key will automatically return 0
  memo = defaultdict(lambda _ : 0)

  # Our base case
  memo[0] = 1

  # Starting from the lowest m value, start calculating the ways to reach m
  for i in range(1, m+1):
    
    # Initialize the number of ways for this m value to be zero (default)
    memo[i] = 0

    for c in coins:
      # New state and branching out
      subproblem = i - c

      # State is invalid for this problem, skip it
      if subproblem < 0:
        continue 
      memo[i] = memo[i] + memo[subproblem]
  return memo[m]
```

---
### Problem 4: Maze
In our final problem, we have an N by M grid. How many ways can a rabbit get from the top-left to the bottom right corner, assuming it can only move down or right?

So the sub-problem is getting the number of ways (paths) it can reach the end, for some N by M grid. This makes sense because as the rabbit moves right or down, the grid that it can make moves in is shrinking. Here's some more math:
  - If it moves right, the new grid is N by M-1 as the number of columns M decreases.
    - If M = 1 (last column, but potentially many rows), the rabbit can only move to the right now.
  - If it moves down, the new grid is N-1 by M as the number of rows N decreases.
    - If N = 1 (last row, but potentially many columns), the rabbit can only move down now.

Some final considerations:
  - We're always reducing the problem to a smaller grid. And when it's a 1 by 1, we can return 1 to say that the rabbit just finished. However we can expand this to make our math easier. When M=1 or N=1, the rabbit can only move down or right. So we can definitely say that at that point, it's basically finished, there are no new paths that we need to count. This expands on our base case. This fixes a math issue which prevents us from recurring to negative rows or columns.
  - The rabbit can move both ways, so sum the amount of paths from moving down and moving right.
  
Mathematically it'd look like this:
  - solution(1,m) = solution(n,1) = 1
  - solution(n,m) = solution(n-1, m) + solution(n, m-1)
So like for a given grid n by m, we're going to try both paths, and you can definitely visualize this working until we reach one of our base cases. When we do, that's a unique path, so we bubble up that return value and sum up the return values for a given N by M, which can kind of be thought of as a position.

---
#### Recursive and memoized approach

```Python

memo = {}
def grid_paths(n,m):
  # If we already have the number of paths for a given grid in the cache, return it. 
  # Rather than actually doing the recursion again.
  if (n,m) in memo:
    return memo[(n,m)]

  # If we reached the base case:
  if n == 1 or m == 1:
    return 1

  # Calculate the number of paths from both directions.
  num_paths = grid_paths(n-1,m) + grid_paths(n,m-1)

  # Now we know the number of paths to the end from some N by M. Cache it and then return it
  memo[(n,m)] = num_paths
  return numPaths

```

---
#### Bottom-up approach
Here we're going to calculate the number of paths, but we're starting with the smallest grids and moving up.

```Python
def grid_paths(n,m):
  memo = {}

  # Initialize the base case for scenarios when either n = 1 or m = 1
  for i in range(1 n+1):
    memo[(i,1)] = 1
  for j in range(1, m+1):
    memo[(1,j)] = 1
  
  # For all grid sizes where n, m > 1
  for i in range(2 n+1):
    for j in range(2, m+1):
      # Calculate the number of paths for a i by j, which is just sum of the number of paths 
      # from the grids when you move right AND when you move down.
      num_paths = memo[(i-1,j)] + memo[(i,j-1)]

      # Cache the result
      memo[(i, j)] = num_paths
  
  # Then return the final result we built up to
  return memo[(n,m)]
```

## Key takeaways
Tackling DP problems require us to think about three main things:
- Define the sub-problem or state? How many recursions per each state? Graph it out in a tree form, that's actually a really good tip. You'll be able to see each node as like a game state, you'll understand backtracking, etc.
- What's the base case? when do we stop recurring and start bubbling values up the tree.
- Try to find a recursive formula that pushes the general case closer to our base case. This is the hardest one, trying to reduce the given state towards the base case.


### Credits 
1. [Mastering Dynamic Programming Part 1 - Tech with Nikola](https://www.youtube.com/watch?v=Hdr64lKQ3e4)
2. [Dynamic Programming Explained - CS Dojo](https://youtu.be/vYquumk4nWw?si=SM93F1GHdJiMTA6c)
3. [Five steps to solve dynamic programming problems - Reducible](https://youtu.be/aPQY__2H3tE?si=6ZqKAnJnZoz-UvTh)
4. [Dynamic Programming and Memoization - HackerRank](https://youtu.be/P8Xa2BitN3I?si=9za_blXSELIpxPlH)