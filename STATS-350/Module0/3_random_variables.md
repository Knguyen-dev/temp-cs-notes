
### Section 1:  Random Variables
A random variable (RV) is a function that assigns a real number to each outcome in an experiment:
- Use uppercase letters, sometimes with indices, to denote random variables e.g. $X, Y_{1}$
- If X is a random variable, we can write $X : S \rightarrow$
- Range of a random variable is the set of all numbers assigned to each possible outcome. So if X is the random variable and S is the sample space, then $X(S)$ is the range of X.

Let $S$ be the sample space and $X$ a random variable. $\forall y \in \mathbb{R}$, we define the event: 

{$s \in S : X(s) \leq y$}

This literally means, the set of all outcomes (s) in my sample space, S.T. the RV assigned to that outcome is less than y. Our experiment is tossing a coin twice and the sample space $S = \{HH, HT, TH, TT\}$. Then $X$ is an RV that assigns each total its total number of heads so that's the X function explained:
- X(HH) = 2
- X(HT) = 1
- X(TH) = 1
- X(TT) = 0

Again the range is the set of all numbers assigned to each possible outcome. So range $X(S) = \{0, 1, 2\}$. Now let's see how we can define events. The idea is "Give me the y value that we're setting, and then we can calculate the set of all $s$ (outcomes) that satisfy the equation (all that get past the filter). 
- If $y=10$, then $P(\{ s \in S : X(s) \leq 10 \}) = P({HH, HT, TH, TT}) = 1$. I mean all of those events are assigned a number that's less than 10.
- If $y=-3$, all of our events are associated with values greater than that, so the set we'll get is the empty set, 0 probability.
- If $y=1$, then $P({TT})$ because that's the only outcome that's less than 1. The probabliity of that outcome $P({TT}) = 0.25$

### Section 2: Cumulative Distribution Function (CDF)
Let $X$ be a RV. The CDF of $X$ is a function $F : \mathbb{R} \rightarrow [0,1]$, defined by the rule:

$F(y) = P(\{ s \in S : X(s) \leq y \})$, for any $y \in \mathbb{R}$.

So given a certain threshold $y$, the CDF calculates the probability of selecting an event, such that the value it's mapped to is less than or equal to that threshold. Because the inner value is the set that gives those. Note that this is cumulative because we're including all $s$ S.T. $X(s) = y$, but also all below it.


## Examples:

#### Example 1: Tickets
Draw 2 tickets ***with replacement*** from a raffle. Let the values of said tickets be [1,1,1,1,1,3,3,3,7,7]. Then let $Y$ be the RV that assigns the sum of both tickets. Assume this is permutation problem.
 n
a. ***Calculate the sample space (Set of all possible outcomes or ticket arrangements)***
Set of all possible ways (arrangements) to pick two tickets with replacement. This is denoted as $S = \{ (1,1), (1,3), (1,7), (3,1), (3,3), (3,7), (7,1), (7,3), (7,7) \}$.

b. ***Calculate the range. The set of all values that the RV can take.***
$Y(S) = \{ 2, 4, 6, 8, 10, 14\}$. So this is all possible sums of tickets that can happen.

Then the next question that you'll ask yourself, "what's the probability of us getting a sum of 6 then? Or something similar". We go over this in the next question.

c. ***Calculate $F(3\pi)$***
So we're trying to calculate the probability that the RV takes on a value that's at most 9.42. These are 2, 4, 6, and 8. Then we need to find the probabillity the RV taking on each of these values and add them up:
1. $P(X=2)$
  1. With outcome (1,1). The probability of picking this is $0.5 * 0.5 = 0.25$
2. $P(X=4)$
  1. (1,3), $P(1,3) = 0.5 * 0.3 = 0.15$
  2. (3,1), $P(3,1) = 0.3 * 0.5 = 0.15$
  3. $P(X=4)=0.3$
2. $P(X=6)$
  1. (3,3), $P(3,3) = 0.3 * 0.3 = 0.09$
  2. $P(X=6)=0.09$
2. $P(X=8)$
  1. (1,7), $P(1,7) = 0.5 * 0.2 = 0.10$
  2. (7,1), $P(7,1) = 0.2 * 0.5 = 0.10$
  3. $P(X=8)=0.2$

$P(X \leq 3\pi) = P(X=2)+P(X=6)+P(X=8)= 0.25+0.3+0.09+0.2=0.84$

Finding the probability of a given 

- $F(3\pi)$ would evaluate to the probability of getting an outcome that is mapped to a value of $3\pi$ or lower. We look at the range $Y(S)$ and find the proportion of elements that are equal to this value or lower. These outcomes are everything except (7,3) and (7,7), so the probability is $\frac{7}{9}$

#### Example 2: Ticket Sequel
Draw 2 tickets ***with replacement*** from the urn = [1,1,1,1,1,3,3,3,3,4] and let $X$ be a RV that assigns the sum of these two tickets.

a. ***What's P({(1,4)}), the probability of first getting a 1 and then a 4?***
$\frac{5}{10} \cdot \frac{1}{10} = 0.05$

b. ***Calculate F(4)***

$S = \{(a,b) | a \in \{1,3,4}\, b \in \{1,3,4\}\}$. 

- $S = \{ (1,1), (1,3), (1,4), (3,1), (3,3), (3,4), (4,1), (4,3), (4,4)\}$.
- $X(S) = \{ 2, 4, 5, 6, 7, 8\}$, the possible values the RV can actually take.

Then $F(4) = \{ s \in S : X(s) \leq 4 \} = P(X \leq 4)$. So the only range values that fit this are 2 and 4. So we would need to calculate P(X=2) and P(X=4).


P(X=2) only happens when we draw two 1s. So $P(1,1) = 0.5 \cdot 0.5 = 0.25$. However the $P(X=4)$ can happen in two cases:
1. $P(1,3) = 0.5 \cdot 0.4 = 0.2$
2. $P(3,1) = 0.4 \cdot 0.5 = 0.2$

This is a 0.4 probability. First we identified the correct outcomes that satisfy our equation, but then we need to calculate the probability of those outcomes happening. The final answer:

$F(4) = P(X \leq 4) = P(X=2) + P(X=4) = 0.25 + 0.4 = 0.65$


