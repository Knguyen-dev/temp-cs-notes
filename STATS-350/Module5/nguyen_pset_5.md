# Question 1

## Part a 
Start with some basic info:
- $E(X_{i}) = 4.6 = \mu$; theoretical expected value from drawing the urn.
- $Var(X_{i}) = 14.64 = \sigma^{2}$
- $E(Y) = n\mu = 40(4.6) = 184$; Use properties of expected value and variance. But this is the theoretical sum.
- $Var(Y) = n\sigma^{2} = 585.1$

She's justified with the Weak Law of Large Numbers. Let $Y_{1}, Y_{2}, ..., Y_{m}$ be independent simulations/observations of $Y$. Let $I$ be a random variable that assigns 1 when assign 1 when $170.5 < Y_{i} < 199.5$ and 0 otherwise. The sample mean $\bar{I}_{m} = \frac{1}{m}{\sum_{i=1}{I_{i}}}$ is the proportion of samples/simulations where $Y_{i} \in (170.5, 199.5)$.

As $m \to \infty$ (number of experiments increases), by the Weak Law of Large numbers, the sample mean of $\bar{I}_{n}$ converges in probability to the expected value $E(I_{i})$, which is the true population probability we're trying to estimate.

## Part b
The sum $Y$ is a total of 40 independent draws from the same distribution, which can be denoted as $Y = X_{1} + X_{2} + ... + X_{40}$. A single sample is of $n=40$ then. The Central Limit Theorems indicates that as the sample size increases, the distribution of their sum tends to a normal distribution, even if $X_{i}$ isn't normal. Since $n=40 \geq 30$, the distribution of $Y$ can be said to be approximately normal. As a result, the student's usage of `pnorm` is justified. 

## Part c 
I think the simulation approach will be more accurate. We can be assured that the sampling distribution in the simulation approach converges to the true mean due to the large sample size that's used. However while the student does meet the $n \geq 30$ condition, I'm assuming that the fact that they just barely meet the condition can have slight affects on how normal the sampling distribution truly is, and therefore how accurate.



# Question 2 
- $\mu_{c} = 351, \sigma_{c} = 1, n_{c}=40$
- $\mu_{p} = 350, \sigma_{p} = 1, n_{p}=43$

a. 
  - $E(\bar{X}_{40}) = \mu = 351$
  - $Var(\bar{X}_{40}) = \frac{\sigma^{2}}{n} = \frac{1}{40} = 0.025$

b.
  - $E(\bar{Y}_{43}) = \mu = 350$
  - $Var(\bar{Y}_{43}) = \frac{\sigma^{2}}{n} = \frac{1}{43} = 0.023$

c. 
  - $E(X_{1}) = \mu_{c} = 351$
  - $Var(X_{1}) = \sigma_{c}^{2} = 1^{2}=1$
  - $P(X_{1} > 351.4) = 0.3445$:
```r 
# P (X1 > 351.4) = 1 - P(X1 <= 351.4)
1 - pnorm(q=351.4, mean=351, sd=1)
```

d. $P(\bar{X}_40 > 351.4) = 0.005$: We already have the mean and standard deviation for random variable $\bar{X}_{40}$. By the central limit theorem, since $n=40 \geq 30$, the distribution plotted by random variable $\bar{X}_{40}$ is approximately normal. As a result, we can treat $\bar{X}_{40}$ as normally distributed $P(X_{40} > 351.4) = 1 - P(X_{40} <= 351.4)$.
```r
1- pnorm(q=351.4, mean=351, sd=sqrt(0.025))
```
  
e. Let $Z - \bar{X}_{40} - \bar{Y}_{43}. Then calculate $ $P(Z = \bar{X}_{40} - \bar{Y}_{43} > 0)$. 
  - $E(Z) = E(\bar{X}_40) - E(\bar{Y}_{43}) = 351-350 = 1$
  - $Var(Z) = Var(\bar{X}_{40}) + Var(\bar{Y}_{43}) = \frac{1}{40} + \frac{1}{43} = \frac{83}{1720}$
  - $P(Z) = 1 - P(X \leq 0) = 1$

```r
# P(Z > 0) = 1 - P(Z <= 0) = 1
1 - pnorm(q=0, mean=1, sd=0.2196)
```

# Question 3
1. Note that this is a paired sample because we are testing the same patient twice. In any case, this should just be a simple one-sample t-test. T-test because we don't know $\sigma$, so we have to calculate t as the test statistic.
2. Let $\mu$ be the true mean differnece in information units between morning and afternoon:
  - $H_{0}: \mu = 0$, which indicates there's no difference between describing A in the morning and B in the afternoon.
  - $H_{1}: \mu \neq 0$, which indicates that there's a difference.
  - Assume the null hypothesis, so $\mu_{0} = 0$.

3. Calculate the t-statistic:
  - $t = \frac{\bar{x} - \mu_{0}}{s / \sqrt{n}} = \frac{-0.1833 - 0}{5.18633 / \sqrt{60}} = -0.2737$
4. Calculate $p = P(X \neq t ) = 2 \cdot P(X > t) = 2 \cdot (1- P(X \leq |t|)) = 0.785$
```r
one_tail_probability = 1 - pt(q=0.2737, df=59)
p_value = 2 * one_tail_probability
p_value # p = 0.785
```
5. Since $p > \alpha$, we fail to reject $H_{0}$.

# Question 4
a.
  - $H_{0}: p = 0.5$. This means they are just guessing and would guess about 0.5 in the long run.
  - $H_{1}: p > 0.5$. This means they are actually psychic and can do better than randomly guessing.
b. 
  - When working with p-values in the past, the p-value represented the probability 
  of getting a sample mean that's this extreme. However the p-value in this case, probability that a student guesses 13 or above correctly. We include "or above" because the $H_{1}$ leads towards a right-tailed test. If this is the case then we can write the following R Code:
```r
# P(Y >= 13) = 1 - P(Y < 13) = 1 - P(Y <= 12)
p_value = 1-pbinom(q=12, size=20, prob=0.5) # 0.131
p_value
```
To decide whether this is intriguing evidence, we need an $\alpha$, which was not provided, so we cannot say. For exmaple's sake, let's say $\alpha = 0.05$. In that case since $p > \alpha$, we fail to reject the null hypothesis and there is no significant evidence that the student has psychic powers.

c. Now we just calculate $P(Y \geq 18) = 2 \cdot 10^{-5}$
```r
# P(Y >= 18) = 1 - P(Y < 18) = 1 - P(Y <= 17) = 0.0002
p_value = 1-pbinom(q=17, size=20, prob=0.5) # 0.0002
p_value
```
Here $p=0.0002$. For example's sake, say $\alpha = 0.01$. Since $p < \alpha$, this is "statistically significant", allowing us to reject $H_{0}$ and accept $H_{1}$.

# Question 5
a.
  - $H_{0}: \mu \leq 0.23$
  - $H_{1}: \mu > 0.23$
  - The consumers believe the company violates FDA standards. So the consumer would prefer to assume this, and let the company be the one that shows evidence of the opposite. An easy way to think about this is that the $H_{0}$ is what the consumers believe in.

b. 
  - $H_{0}: \mu \geq 0.23$. This is what the company believes. I think you can set $=$ and $\neq$,, but you should just make sure you only get the left tail. 
  - $H_{1}: \mu < 0.23$
  - The company believes that they aren't violating standards so $H_{0}$ implies the proportion is at least $0.23$. They want to put the burden of proof on the consumers instead.

c.
  1. $\bar{x} = 0.228, s=0.03$. 
  2. $t=\frac{\bar{x} - \mu_{0}}{s/\sqrt{n}} = \frac{0.228 - 0.23}{0.03 / \sqrt{225}} = -1$

d. 
  - From the consumer's perspective $p = P(X \geq  -1) = 0.84$. Since $p > \alpha$, we fail to reject $H_{0}$. As a result, there's not enough evidence to reject that the company violates FDA regulations.
```r
# P(X >= -1) = 1 - P(X < -1) = 1 - P(X <= -1); t-distribution
1- pt(q=-1, df=224)
```
e. 
  - From the company's perspective you're trying to do left tail test. $p = P(X \leq -1) = 0.159$. Since $p > \alpha$, we can't reject $H_{0}$, the consumers couldn't find enough evidence that the company violates FDA standards. 
```r
pt(q=-1, df=224)
```

# Question 6
a. 
- $\mu = 0.234$
- $t = \frac{0.234 - 0.23}{0.03 / 15} = 2$
- This means that in reality, There was enough evidence to reject $H_{0}$ and accept $H_{1}$. That means that in part 4d we committed a Type 2 error, because failed to reject $H_{0}$ when $H_{0}$ was false.
```r
# p(X >= 2) = 1 - p(x <= 2) = 0.023
1-pt(q=2, df=224)
```

b. 
  - First we'll re-calculate the p-value to get $p=0.97$. So here the result remains the same since $p > \alpha$ still holds. So we made the correct decision here.
```r
pt(q=2, df=224)
```

c. 
  - $n=800$ and also assuming that $\mu = 0.234$
  - $t = \frac{0.234 - 0.23}{0.03 / \sqrt{800}} = 3.77$
  - Now the data is 3.77 standard deviations above the mean, and since this is a left side test, the probability enclosed will be about 1. Since $p=1 > \alpha$, our result does not change from part e.
```
pt(q=3.77, df=799)
```

d. Again, our answer did not change in part 4e, when we did question 6d. 

e. 
  - $\sigma = 0.03, n=250, \mu=0.234, \alpha = 0.04$; 
  - Let $H_{0}: , H_{1}:$
  - Calculate $\beta$ (probability of committing a Type 2 error) and the power of the test. 
  - P(Type 2 Error) = $\beta$ = 0.947
  - Power = $1 - \beta = 0.053$
```r

# These are equivalent for calculating the quantile.
# Just wanted to show both approaches.
qnorm(p=0.96)
pnorm(q=0.23 + 1.75*0.03, mean=0.23, sd=0.03)


pnorm(q=0.2825, mean=0.234, sd=0.03)
```

**Extra notes:** Looking at our distribution with our true mean, let's find P(Type 2 Error) = P(Don't Reject H0 | mu=0.234). Remember we set the rule that we reject H0 when sample mean >= 0.2825. So we don't reject H0 when sample mean < 0.2825. Again, we'll do calculations on the real distribution (H1). But it's the idea that we're not rejecting H0 on the H1 distribution, which is akin to P(Don't reject H0 | H0 is actaully false) P(X <= 0.2825) = P(Rejecting H0 | H0 is false) = 0.947.