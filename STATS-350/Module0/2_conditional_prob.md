## Conditional Probability with Independence

### Conditional Probability
The probability of something given that something else has already happened. I kind of think of it as narrowing the scope.

If A and B are events, and $P(B) > 0$, then the conditional probability of A given B is denoted as $P(A|B) = \frac{P(A \cap B)}{P(B)}$. Rearranging, we get $P(A \cap B) = P(A|B) * P(B)$. 

Given that the die rolls an odd number, what's the probability of getting at least 3.
- P(Odd) = $\frac{3}{6}$
- P(at least 3) = $\frac{4}{6}$
- Calculate P(at least 3 | Odd)=$\frac{2/6}{3/6}=\frac{2}{3}$

### Independence
Two events A and B are independent IFF $P(A \cap B) = P(A) \cdot P(B)$. This implies:
  - $P(A) = P(A|B) = P(A| B^{c})$
  - $P(B) = P(B|A) = P(B| A^{c})$

If they're independent, then the idea of knowing one doesn't affect the probabillity of knowing the other. So narrowing the scope won't affect the probability since htey're independent of each other.

Remember that **disjoint** is a different idea. It means $P(A \cap B) = 0$, they're mutually exclusively. This is a lot different from independence. So if they're disjoint, then we'll never satisfy that independence equation. So it must be dependent.

### Bayes' Rule and Tree Diagrams
Bayes' rule is basically an extension of the conditional probability formula. Bayes' rule is given by: 

$P(A|B)=\frac{P(A \cap B)}{P(B)} = \frac{P(A) \cdot P(B|A)}{P(A) \cdot P(B|A) + P(A^{c}) \cdot P(B|A^{c})}$

This is actually quite simple. The top part makes sense because that's just the joint probability representing using conditional probabilities. The bottom is the same thing, but we use addition and the idea of marginalization to get the probability of B.

## Examples 

#### Example 1: Hospitals
- $P(+ | D) = 0.62$; The probability of testing positive, given they have the disease.
- $P(-|D^{c})= 0.82$; Probability of testing negative, given that you don't have the disease.

a. Calculate the conditional probability of a false positive, so testing positive, but you don't actually have the disease. This is denoted as $P(+|D^{c})$.
b. Calculate the conditional probability of a false negative, so testing negative, but you have the disease. Denoted as $P(-|D)$.

- **Complement Rule:** $P(+|D^{c}) = 1 - P(-|D^{c}) 0.18$; narrowed the scope to $D^{c}$
- **Again complement rule:** $P(-|D) = 1-P(+|D) = 1-0.62=0.38$

c. Suppose a person is selected from a population where the probabillity of getting the disease is $P(D)=0.05$. What's the tree diagram that describes this experiment?

A **tree diagram** refers to those coin flip branching binary tree diagrams:
```
            origin point
           /          \
          D           D(c)
         / \        /       \
    P(+|D) P(-|D)  P(+|D(c)) P(-|D(c))
```
d. Probability that a selected woman has the disease
You'd use marginalization $P(+) = P(+|D) \cdot P(D) + P(+|D^{c}) \cdot P(D^{c}) = 0.62 * 0.05 + 0.18 \cdot 0.95 = 0.202$.

e. Given that the person tests positive, what's the probability of them having the disease, denoted as $P(D | +)$. 

- $P(D|+) = \frac{P(D \cap +)}{P(+)} = \frac{P(+|D) \cdot P(D)}{0.202} = \frac{0.62 \cdot 0.05}{0.202} = 0.15$ 