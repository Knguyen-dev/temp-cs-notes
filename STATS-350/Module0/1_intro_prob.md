
## Section 1: Probability Model
Model based on Kolmogorov's axioms. Probability involves events (sets) rather than individual outcomes (elements in set). E.g. the probability of the samplespace P(S) = 1

- Sample Space :Set of all possible outcomes for a particular expeirment 
- Event: A subset wtihni the sample space, can include one or more outcomes, or even none.
- Probabillity Measure: A function that assigns the probability to each event. 

## Section 2: Probability Rules and Conventions

### Finite Additivity
If two events A and b are disjoint, then $P(A \cup B) = P(A) + P(B)$. The idea of disjoint events meaning they don't overlap with each other. Though if they're joint, we'll have to take into subtract the joint.

- Probability is between 0 and 1. The probability of the sample space is always 1 since it encompasses everything..
- $P(A^{c}) = 1 - P(A)$
- If $A \subset B \rightarrow P(A) \leq P(B)$ 

### Exercises

#### Example 1: Probabilities 
Toss a fair coin twice. Here are the events we'll talk about:
- A = {HH}
- B = {first coin is heads}
- C = {At least one tail}
- S = {HH, HT, TH, TT}

1. $P(A^{C}) = \frac{3}{4}$; there are 3 other outcomes
2. $P(A \cup B) = P(A) + P(B) - P(A n B) = \frac{1}{4} + \frac{2}{4} - \frac{1}{4} = \frac{2}{4} = 0.5$
3. $P((A \cap C)^{c}) = 1$; The inner expression resolves to 0, the nullset. And the opposite of that is everything.


#### Example 2: Finite Sample Spaces, combinatorics
Julia is in a class of 30 students (including herself) that are randomly selected to for ma committee of 3 people.
a. **If the committee needs 1 president, vice president, and secretary, what's the probability that she's selected as president? Order matters so this is a permutation problem.**

- Sample Space = 30 * 29 * 28 = 24360
- Favorable outcomes: All arrangements where Julia is president. So Julia is chosne first, then the two others from the remaining 29 are chosen for the rest of the roles. This is 1 * 29 * 28 = 812.
- $P(Julia President) = \frac{812}{24360}$

b. **What is the probability of Julia being a part of the committe in general?**
Julia can be in any of the three positions. The sample space is the same, but hte favorable outcomes can be modeled differently:
- President: $1 * 29 * 28$
- Vice president: $29* 1 * 28$
- Secretary: $29 * 28 * 1$

So we'll calculate the number of arrangements where Julia is in any of those positions. All them all up to get $3 * 29 * 28 = 2436$ permutations. Now we can just divide this by the sample space. This ends up being $\frac{1}{10}$ 

c. **What if the committee has no roles. Just 3 people chosen randomly? What's the probability that she's in it?**
Now it's a combination problem. We'll change up the sample space:
- Sample Space: $C(n=30,r=3) = \frac{30!}{3!(27!)} 4060$; 

Now we want all outcomes that include Julia. To do this you fix Julia in the group and then pick 2 more from the remaining 29:
- $1 * C(n=29, r=2) = 406$
- So it's like we're already assuming Julia was picked, find all remaining pairs excluding julia.
- Dividing $\frac{406}{4060}=0.10$

#### Example 3: Probability, Marginzliation, etc.
- P(A) = 0.7
- P(B) = 0.5 
1. $P(A \cap B) + P(A^{c} \cap B) = 0.7 \cdot 0.5 + (1-0.7) \cdot 0.5$
2. $0.35 + 0.15$

New problem:
- $P(A)=0.7$
- $P(B) = 0.4$
- $P(A^{c} \cap B) = 0.1$
- Calculate $P(A^{c} \cap B^{c})=?$

Break this down into 4 mutually exclusive events:
1. $A \cap B$
2. $A \cap B^{c}$; 
3. $A^{c} \cap B$
4. $A^{c} \cap B^{c}$

We can call in the law of marginalization:
- $P(A \cup B) = $ The sum of all four of these regions.
- Then use the complement $P(A^{c} \cap B^{c}) = 1 - P(A \cup B)$

1. **Marginalization:** $P(B) = P(A \cap B) + P(A^{c} \cap B)$; 
  1. $P(A \cap B) = P(B) - P(A^{c} \cap B)$; rearranging the equation and plugging in knowns.
  2. This equates to $P(A \cap B) = 0.3$.
2. **Finite Additivity:**: Find $P(A \cup B)$. Use the additive property $P(A \cup B) = P(A) + P(B) - P(A \cap B) = 0.7+0.4-0.3= 0.8$
3. **Complement:** $P(A^{c} \cap B^{c}) = 1 - P(A \cup B)$ = 0.2

#### Example: Regular Probability
Let's select numbers from the set S={2,3,4,5,6,7}. If I select two of these numbers without replacement. What's the probability that both numbers are not prime? Use three decimals.

- Total outcomes $C(n=6,r=2) = 15$ total pairs of numbers you can pick
- Favorable outcomes: The set of non-primes {4,6}, the idea is that you have to pick both of these so fix them. C(n=2,r=2), out of those 2 primes, you must pick 2, there's only one way to do that.
- 1/15 is the probability
