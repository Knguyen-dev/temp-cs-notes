# Chi-Squared Goodness of Fit 
The set of all possible outcomes is the sample space $S$. A goodness-of-fit test consists of:
- Partition these outcomes into different subsets called events/cells.
  - $E_{1} \cap E_{2} \cap E_{k} = S$
  - Each event is pairwise disjoint, meaning that if $E_{1}$ contains an outcome, that outcome cannot be stored in other events. 
- Make assumptions about the probabilities of those events.
  - Given $E_{1}, ..., E_{k}$, let $p_{j} = P(E_{j})$. We can then define a vector of cell probabilities $\vec{p} = (p_{1}, ..., p_{k})$
  - Let $\pi$ be the set of all possible probability vectors $\vec{\pi}=(\pi_{1}, ..., \pi_{k})$ as long as two things are satisfied:
    - $\pi_{i} \geq 0$ for all i values.
    - $\sum_{1}^{k}{\pi_{i}} = 1$
- Test whether these assumptions are consistent with the data observed. For example:
  - $H_{0}: \vec{p} \in \pi_{0}$
  - $H_{1}: \vec{p} \in \pi_{1}$

## Observed and Expected Cell Counts 
- Sample: Repeat the experiment $n$ times and let $o_{j}$ be the number of times that $E_{j}$ occurs, we call this the **observed cell count** of $j$.
- Our GOF tests compare the observed cell counts (from our sample) and expected cell counts:
  - Every cell count for $j$, $e_{j}$, is obtained assuming $H_{0}$
  - If $p_{j}$ is the probability of observing $E_{j}$ under $H_{0}$, and the total number of observed values is $n$, then cell $j$'s **expected count** is $e_{j} = p_{j} \cdot n$.

We determine the **expected counts** by assuming $H_{0}$:
- If the probabilities of each cell are clearly stated, then each cell's expected count is simply $e_{j} = p_{j} * n$, where $p_{j}$ is the probability of observing that event and $n$ is the total sample size.
- If the probabilities aren't given, we'll have to use the plug-in principle (or other methods) and our sample estimate the probabilities. The formula becomes $e_{j} = \hat{p}_{j} * n$.

### Example 1: Observed and Expected Counts 
The experiment is to roll a die and observe the number of dots on the top face. We create $S, E_{1}, ..., E_{k}$ and $\vec{p} = (p_{1}, ..., p_{k})$ under the null hypothesis that the 6-sided die is fair.

- You can reason that $E_{1} = \{1 \}$, it's the event where we roll a 1. Then $E_{2} = \{ 2 \}$, the event where we roll a 2. We can generalize this into $E_{j} = \{ j \}$ for $j=1,...,6$
- Assuming that the die is fair ($H_{0}$), then $p_{j} = P(E_{j}) = \frac{1}{6}$. 
- Constructing the hypotheses:
  - $H_{0}: (\frac{1}{6}, ..., \frac{1}{6}) \in \pi_{0}$. Equivalent to $p_{1} = ... = p_{6}$, the dice is fair.
  - $H_{1}:$ At least one $p_{i} \neq \frac{1}{6}$. Meaning the dice isn't fair.

## Goodness-of-fit tests
- Pearson's chi-squared statistic: $X^{2} = \sum_{j=1}^{k}{\frac{(o_{j}-e_{j})^{2}}{e_{j}}}$
  - Here $k$ is the number of cells, again $o_{j}$ and $e_{j}$ are the observed and expected counts for the j's cell.
- Likelihood ratio chi-squared: $G^{2} = 2\sum_{j=i}^{k}{(o_{j}log(\frac{o_{j}}{e_{j}}))}$

You only need one of these for a test. Both follow a Chi-Squared distribution with varying degrees of freedom depending on the test that you're doing.



```R
# Setup observed and expected
observed <- c(3407, 3631, 3176, 2916, 3448, 3422)
n = sum(observed) # total sample size 
p = rep(1/6, 6)   # Create pdf function for probabilities under H_{0} 
expected = n * p # 3333.333, ...

# Setup chi-squared tests
X2 = sum((observed-expected) ^ 2 / expected)  # 94.18 
G2 = sum(2 * observed*log(observed/expected)) # 95.80
```

### Degrees of Freedom
This is calculated as the difference in the dimensions of the unrestricted and restricted sets of possible $p_{1}, ..., p_{k}$.
- Unrestricted set: This has $k-1$ dimensions. There are $k$ probabilities that add up to 1. Under $H_{1}$ the number of cells that can vary is $k-1$, remember that.
- Restricted set: Less than $k-1$ dimensions and is determined by the number of probabilities that are free to vary. You use this with $H_{0}$

### Example 1 Continued: Basic Chi-Squared
- $H_{0}: p_{1} = ... = p_{6} = \frac{1}{6}$. Here $k=6$
- The unrestricted set has $k-1 = 5$ probabilities that are free to vary.

1. $H_{0}$ specifies that the probabilities must be exactly $\frac{1}{6}$. This means that they aren't free to vary, they must stay at a specific point. So the restricted set has zero dimension.
2. The unrestricted set, follows the $H_{1}$ saying at least one $p_{i} \neq \frac{1}{6}$. So it has $k-1=6-1=5$ probabilities, as that one outcome needs to be different.
3. $df = unrestricted - restricted = (6-1) - 0 = 5$. We have all we need to calculate the p-value with some degrees of freedom.
```R 
# Both are zero, and that's good enough for us ot say that there's enough evidence 
# to reject H_{0} and accept H_{1} (concluding that dice is unfair).
df = (6-1) - 0
1 - pchisq(X2, df) # 0
1 - pchisq(G2, df) # 0
```

## Example 2: Die Variation
How about testing claims about individual probabilities e.g. $p_{1} = p_{6}$:
- $H_{0}: p_{1} = p_{6}, p_{2} = p_{5}$ and $p_{3} = p_{4}$
- $H_{1}: p_{1} \neq p_{6}, p_{2} \neq p_{5}$ or $p_{3} \neq p_{4}$

Note that we aren't given specific values of probabilities, but are talking about a claim whether certain probabilities are equal or not. Since we're not given info, we need to estimate the probabilities from our sample:
```R
# Find the means of the relevant observed cells, and assign the results as the expected
# counts of those cells.
exp_16 <- mean(obs[c(1,6)])
exp_25 <- mean(obs[c(2,5)])
exp_34 <- mean(obs[c(3,4)])
exp <- c(exp_16, exp_25, exp_34, exp_34, exp_25, exp_16)

G2 = 2*sum(obs*log(obs/exp)) # 8.1645
```
- Unrestricted set: $6-1=5$
- Restricted set: Probabilities $p_{1},p_{2},p_{3}$ can vary as long as they're equal to $p_{6},p_{5},p_{4}$. So 3 things can vary, $3-1 = 2$.
- $df = unrestricted - restricted = 5 - 2 = 3$

Let $\alpha = 0.07$: 
```R 
1 - pchisq(q=G2, df=3) # p = 0.043
```
This means we reject $H_{0}$ and accept $H_{1}$. We have significant statistical evidence to conclude that at least one of the conditions are true:
- $p_{1} \neq p_{6}$
- $p_{2} \neq p_{5}$
- $p_{3} \neq p_{4}$

## Example 3: Poisson Distribution
Let $X(S) = \{ 0, 1, 2, ... \}$. Let $X$ follow a Poisson distribution with intensity parameter $\mu \in (0, \infty)$ if $X$ has a pmf given by:

$f(x) = P(X=x) = \frac{\mu^{x}e^{-\mu}}{x!}$

This means $X \sim Poisson(\mu)$ and this means $E(x) = Var(X) = \mu$. Probabilities for any $x$ value are defined by pmf once $\mu$ is given. So once $\mu$ is known, you will know your probabilities. You typically see Poisson when counting "arrivals" in a fixed time interval. We're using $\hat{\mu}$ to find:
- Expected counts under $H_{0}$
- The test statistic
- Degrees of freedom
- THe conclusion of the test

In 1910, they counted alpha-particles and observed each of the $n=2608$ intervals. Determine whether or not the number of alpha-particles on the interval follow the Poisson distribution.
- $S = \{ 1, 2, ...\}$. RV $X$ represents the number of alpha-particles on the interval.
- Partition $X(S)$ by setting $E_{j} = \{ j -1 \}$ for $j=1,...,10$ and $E_{11} = 10, 11, 12, ...$
- $H_{0}: X \sim Poisson(\mu)$
- $H_{1}$ X doesn't follow a Poisson distribution.


1. Assuming $H_{0}$, we know the alpha particle scintillion follows a Poisson distribution. We aren't given the direct pmf values, but since it's a poisson dist. we can use the poisson pmf: 
```R
# 1. Let this be our sample data and counts for each event
# Like 15 events and the counts for each event.
alphacount = 0:14
obs = c(57, 203, 383, 525, 532, 408, 273, 139, 45, 27, 10, 4, 0, 1, 1)
table(rep(alphacount, obs))

# 2. No parameter mu was given, so we need to estimate it (plugin principle)
n = sum(obs)
x = 0:14
fx = obs / n
muhat = sum(f*fx) # 3.8715

# 3. Set E_{j} = {j-1} for j=1,...,10, and E_{11} = {10, 11, 12, ...}
# I'm just constructing the cells/events as they wanted. 
obs_alpha = c(obs[1:10], sum(obs[11:15])) 

# 4. Use the Poisson distribution and our estimate of the mean to get the probabilities for each cell.
# Note: dpois and ppois are the PMF and CDF functions in R respectively. Lambda is the 
# intensity parameter and p is the estimated probabillities of the cells in the problem.
p_1_10 = dpois(x = 0:9, lambda = muhat) 
p11 = 1 - ppois(q=9, lambda = muhat) # 1 -cdf to count everything above 9
p = c(p_1_10, p11)

# 1. Using estimated probabilities, multiply them by total sample size to get expected counts
# 2. Then calculate the test statistic for the likelihood ratio test statistic
n = sum(obs_alpha)
exp_alpha = p*n
G2 = 2 * sum(obs_alpha*log(obs_alpha/exp_alpha))

# 6. 
# 1. Dimension of unrestricted (H1): number of cells - 1 = 11 - 1 = 10
# 2. Since the intensity parameter of the Poisson wasn't given, the prob. are free to vary. Once this parameter is given though, the probabilities will be fully determined.
# 3. df = (11 - 1) - 1 = 9

df = 9
1-pchisq(G2, df) # 0.125

# If we used a alpha = 0.04, we fail to reject H_{0}. We don't have enough evidence to reject 
# the count of alpha particle scintillations follow a Poisson distribution.
```