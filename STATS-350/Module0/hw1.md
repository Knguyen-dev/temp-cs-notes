#### Example 3: World series
In Major League Baseball’s World Series, the winners of the National ( N ) and American ( A ) League pennants play a sequence of games. The ﬁrst team to win four games wins the Series. Thus, the Series must last at least four games and can last no more than seven games. Let us deﬁne an outcome of the World Series by identifying which League’s pennant winner won each game. For example, the outcome of the 2019 World Series, in which the Washington Nationals represented the National League and the Houston Astros represented the American League, was NNAAANN. 

a. ***How many World Series outcomes are possible?***

- Ends when one team wins 4 game
- Can last between 4 to 7 games. The technique is to count the number of outcomes where a team wins $k$ games, where $k=4,5,6,7$. You can reason that we will ensure the opponent has $k-4$ loses in each scenario. So when counting a game with length of 6, we'll fix 2 games to say that team A won. As a result ,we'll 
- Now let's calculate the number of outcomes for A winning (N losing), and then multiply by 2 to consider the mirror dimension when N wins but A loses. The formula is $C(k-1, 3)$. The idea is that we'll fix the last game to be a win for the winning team, and then chose the other 3 games. This is because you know someone is going to win if we knwo it's a four lengthed game.
- I think another reason why this is more a combinations problem is because the idea is that you care about outcomes which have different # of wins and loses for the two teams. But you don't care about in what order they win. 

4-game series:
When one team wins all four games. So this is either "AAAA" or "NNNN", with two outcomes. So let's fix the last game for A, leaving us to choose for the remaining 3 games. So we need to place 3 wins out of 3 games. $C(3,3) = 1$.

5-game series:
Again fix the last win for A, allowing us to choose from the remaining 4 games. Then we need to place 3 wins for A. $C(4, 3) = 4$.

6-game series:
Fix the last win for A, so we only have 5 other games to choose from. Now for A to win, we need to place 3 wins out of those 5. $C(5, 3) = 10$

7-game series:
Fix the last win for A, so we only have 6 other games to choose from. For A to win, we'll place 3 wins out of those 6, $C(6,3)=20$

Summary:
- Number of outcomes for A is 35
- As a result, number of outcomes for B is 35.
- Total in the series is 70


b. ***How many outcomes are possible if the third game is won always by the American League?***
However you can think about it as fixing the third position to A. Doing the mirror is case

4-game series:
- Case 1: A wins 4-0, with the sequence "AAAA"
- Case 2?: N wins 4-0? Not possible since we're fixing A (NNAN).
- 1 outcome

5-game series:
There are four non-fixed positions as the last one is fixed. Now A needs to win 3 games, and N needs to lose just 1. So any easy to calculate this is to choose the one loss position out of those 4 C(4,1) = 4. 
However one of these sequences "AANA" + A, is invalid, so we subtract this. Total 3 outcomes.

Still four non-fixed positions, N needs to win 3 games, and A needs to lose 1. Do the same calculation. Now consider the sequences "ANNN", "NANN", "NNAN" and "NNNA", and see which is invalid due to the constraint. Only "NNAN" + "N" is valid, only one outcome.

6-game series: 

Now we have 5-non-fixed positions. For N to win, they need 4 wins whilst A gets 2 losses (including position 3). Distribute 4 wins for N and one more win nfor A amongst the 5 non-fixed positions

C(5,1) = 5; 5 ways to choose 1 position for A's other win amongst the 5 position

For A to win they need 4 wins (they already have 1 in position 3) and N gets 2. Distribute 3 more wins for A and 2 for N amongst the positions. C(5,2) = 10, ways to choose 2 positions for N's wins

7-game series:



