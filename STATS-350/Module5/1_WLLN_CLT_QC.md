# WLLN and CLT Quick Check

## Question 1
Let $X_{1}$ be a discrete random variable with probability mass function:
  - 0.6, x=3
  - 0.3, x=4
  - 0.1, x=10
  - 0, otherwise

Then let $X_{1}, X_{2}, ..., X_{n} \sim \mathbb{P}$. Each $X_{i}$ is independent and have the same distributions (IID). Let $\bar{X}_{n}$ be the sample mean, calculate $E(X_{1})$.

1. The expected value of a discrete random variable is $\sum_{x\in X(S)}{x * f(x)}$.
2. We can say $E(X_{1}) = 3(0.6)+4(0.3)+10(0.1) = 4$

## Question 2
Calculate $Var(X_{1})$

1. The variance of a discrete RV is $\sum_{x \in X(S)}{(x-\mu)^2\cdot f(x)}$
2. $Var(X_{1}) = (3-4)^2(0.6) + (4-4)^2(0.3)+(10-4)^2(0.1) = 0.6 + 0 + 3.6 = 4.2$

## Question 3
Let $n=80$, calculate $E(\bar{X}_{80})$.

1. They want us to calculate the expected value of the sample mean $\bar{X}_{80}$. Each $X_{i}$ has an expected value, and we have to calculate the average for $i=1, 2,..., 80$. Remembering our properties, this is just $\mu$ the expected value from any given $X_{i}$. 
2. $E(\bar{X}_{n}) = E(\frac{1}{n}\sum_{i=1}^{n}{X_{i}}) = \frac{1}{n}\sum_{i=1}^{n}$
3. $E(\bar{X}_{80}) = E(\frac{1}{80}\sum_{i=1}^{80}{X_{i}}) = \frac{1}{80}\sum_{i=1}^{80}{E(X_{i})} = \frac{1}{n}\sum_{i=1}^{80}{\mu} = \mu$
4. Now let's interpret this result. The expected value of the sample mean, should be exactly equal to $\mu$. Where $\mu$ is the mean of any given individual random variable $X_{i}$.
5. $E(\bar{X}_{80}) = \mu = 4$
One last connection but the sample size 80 doesn't affect the expected value that we calculate. It only affects variance and distribution shape.

## Question 4
Let $n=80$, calculate $Var(\bar{X}_{80})$.

1. $Var(\bar{X}_{80}) = \frac{\sigma^{2}}{80}$. You can use the properties that we found above. Good, now we can just plugin the variance for a single RV.
2. $Var(\bar{X}_{80}) = \frac{4.2}{80} = 0.0525$

## Question 5
Let $n=80$. Based on the CLT, approximate $P(3.8 < \bar{X}_{80} \leq 4.1)$.

1. Since $n \geq 30$, we can use the CLT, which means we can treat $\bar{X}_{n}$ or our distribution of sample means as approximately normal. As a result, this lets us use normal distribution operation to find the probability. 

2. $P(3.8 < \bar{X}_{80} \leq 4.1) = P(\bar{X}_{80} \leq 4.1) - P(\bar{X}_{80} \leq 3.8)$
3. Let's write the R code:
```r
# Do normal distribution calculations 

mu = 4 # same as the mean of a given variable
sigma2 = 0.0525 # different from the variance of a given variable
pnorm(q=4.1, mean=mu, sd=sqrt(sigma2)) - pnorm(q=3.8, mean=mu, sd=sqrt(sigma2)) # 0.47
```
4. To recap, we have 80 different random variables in our sample. Then we sum up all 80 and divide to get the sample mean $4$. This is just one value, but the idea is that if you repeat this sample of 80 many times, and plot each one's sample mean, the distribution of those sample means becomes approximately normal. 