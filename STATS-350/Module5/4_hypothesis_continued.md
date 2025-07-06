# Hypothesis Testing: Brief Review


## Tests and P values 
1. Left-tailed test:
  - $H_{0}: \mu \geq \mu_{0}$
  - $H_{1}: \mu < \mu_{0}$
  - $p = P(Z \leq z)$  
  - In R: `pnorm(z)`
2. Right-tailed test: 
  - $H_{0}: \mu \leq \mu_{0}$
  - $H_{1}: \mu > \mu_{0}$
  - $p = P(Z \geq z)$  
  - In R: `1 - pnorm(z)`
3. Two-tailed test (non-directional):
  - $H_{0}: \mu = \mu_{0}$
  - $H_{1}: \mu \neq \mu_{0}$
  - $p = P(Z \neq z)$  
  - In R, `2*(1 - pnorm(abs(z))`

## Formulating Suitable Hypothesis
There are a couple of cases:
- Let's say you're one group of researchers. Here $H_{1}$ has the statement that we want to prove.
- Another situation is where you have two parties, A and B, which have competing/opposite statements. Let's say you want to study the problem from the perspective of party A:
  - Since the problem is viewed from A's perspective, they have a preferential position, so their statement becomes $H_{0}$.
  - Then Party B bears the burden of proof, so B's statement becomes the $H_{1}$.

## Test Statistic Revisited
- When $\sigma$ is known $Z = \frac{\bar{X}_{n} - \mu_{0}}{\sigma / \sqrt{n}}$ and $Z \sim N(0,1)$.
  - There aren't many real life examples where $\sigma$ is konwn. 
  - One special case is when you know the random sample trials are bernoulli trials. As a result you can derive the population mean and variance using the values. So $\mu = p$ and $\sigma^{2} = p(1-p)$. You'd assume the null hypothesis and proceed.
  - Observed Test Statistic: $z= \frac{\bar{x}-\mu_{0}}{\sigma / \sqrt{n}}$

- When $\sigma$ is unknown $T = \frac{\bar{X}_{n} - \mu_{0}}{S_{n}/\sqrt{n}}$ where $S_{n} = \sqrt{\frac{\sum{(X_{i} - \bar{X}_{n})^{2}}}{n-1}}$.
  - Here we follow a T-distribution with $n-1$ degrees of freedom.
  - Observed Test Statistic: $t=\frac{\bar{x}-\mu_{0}}{s / \sqrt{n}}$, where $s=\sqrt{\frac{\sum{(x_{i} - \bar{x})^{2}}}{n-1}}$ is the sample standard deviation.

## Conclusion and interpretation
p-value is the probability of getting that data (observing the sample) assuming that the $H_{0}$ is correct. We'll reject $H_{0}$, if the estimate indicates that it's highly unlikely to observe that data. Here's the procedure:
- Set a significance level $\alpha$ before collecting data.
- After performing the test:
  - If $p \leq \alpha$, reject $H_{0}$ and accept $H_{1}$.
  - If $p > \alpha$, do not reject $H_{0}$.