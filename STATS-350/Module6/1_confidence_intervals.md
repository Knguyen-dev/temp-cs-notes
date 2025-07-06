# Set Estimation: Confidence Intervals

## Confidence Intervals 
Again, point estimators are just estimators for a parameter. For exmaple the sample mean $\bar{X}_{n}$ is a point estimator for the population mean $\mu$. However of course, the realization of $\bar{X}_{n}$ may not be equal to $\mu$, there's a range. This leads into the set/interval estimator, where we'll consider a set of values that $\bar{X}_{n}$ could fall into.

Reviewing normal distributions, about 95% of the area is 2 (actually 1.96) standard deviations away from the mean. $\bar{X}_{n}$ follows an approximately normal distribution, so if you take a sample and get $\bar{x}$, there's a $95%$ chance that the observed sample mean is within 1.96 standard deviations (also called standard errors) from the population mean $\mu$ (which is the center of the distribution). 

We'll create an interval centered at $\bar{x}$ and extend it 1.96 standard errors to both sides. If you keep drawing samples, you'll keep plotting new intervals. The interval can be denoted as:

$\bar{x} \pm 1.96 SE_{\bar{X}_{n}}$

Some intervals may contain $\mu$ (our population mean), but others won't. If it does contain $\mu$, it would mean all the values in that particular interval are within 1.96 standard errors of $\mu$. By this construction, there's a 95% chance of obtaining a sample that creates an interval that contains $\mu$.

This is called a **confidence interval** since we attach a confidence level to the interval we propose. It's the probability that the method produces an interval that contains the population parameter $\mu$.

### Confidence Interval for population mean (Known Variance)
When $\sigma$ is known, a 95% confidence interval for $\mu$ is given by:

- $I = (\bar{X}-q\cdot\frac{\sigma}{\sqrt{n}}, \bar{X}+q\cdot\frac{\sigma}{\sqrt{n}})$
- $q = 1-\frac{\alpha}{2}$ quantile. So that's actually 1.96.
- $P(\mu \in I) = 1 - \alpha$

This interval is centered on $\bar{X}_{n}$. The standard error is $\frac{\sigma}{\sqrt{n}}$ and the margin of error is $1.96 \cdot \frac{\sigma}{\sqrt{n}}$. Then getting a random sample, find the sample mean $\bar{x}$ so now $\bar{X} = \bar{x}$, so plug in your numbers to that above equation.

Let's look at an example in R. We'll use a dataset from the the General Social Survey, which contains a sample. We'll estimate the average number of hours worked in the US and assume that $\sigma = 15$: 
```r
library(infer)
vec1 = gss$hours
xbar = mean(vec1)   # 41.382
n = length(vec1)    # 500

sigma = 15
c(xbar - 1.96*sigma/sqrt(n), xbar + 1.96*sigma/sqrt(n))
```
The sample mean is 41.38 hours using a sample size $n=500$. Since we assumed that $\sigma=15$, we can use the above formula to construct the confidence interval. We're 95% confident that the average number of weekly hours worked in the US is between $40.07$ and $42.70$ hours.

### Confidence Interval for population mean (Unknown Variance)
When $\sigma^{2}$ is unknown the equation for a 95% confidence interval for $\mu$ will use the sample standard deviation instead of the population one. Simple as that:

- $(\bar{X}-q\cdot\frac{S_{n}}{\sqrt{n}}, \bar{X}+q\cdot\frac{S_{n}}{\sqrt{n}})$

Again, $S_{n}$ is an unbiased and consistent estimator. While the distribution and the number 1.96 changes slightly, for large enough sample sizes it doesn't make a difference.
```r
s = sd(vec1)
c(xbar - 1.96*s/sqrt(n), xbar + 1.96*s/sqrt(n))
```

### Example: 97% confidence interval
Find a 0.97-level confidence interval if $\bar{x}=15.3, s=2.5, n=150$. Assume that $s = \sigma$.

1. Find confidence interval with known variance then $s^{2} = \sigma^{2}$

```r
alpha = 1-0.97 # alpha = 1 - confidence level (alpha)
q <- qnorm(1- alpha/2)

lower_bound = 15.3 - q*2.5/sqrt(150) 
upper_bound = 15.3 + q*2.5/sqrt(150) 
```
in
## Confidence Intervals and Hypothesis Testing 
Reject $H_{0}$ is $p < \alpha$. Here $q$ is the quantile representing the critical point for rejecting and failing to reject $H_{0}$. Imagine it as:
  - $+q:$ If your sample mean $\bar{x} > q$, then reject null hypothesis.
  - $-q:$ Or if less than $-q$ then reject the null hypothesis. 

We can focus on one side by saying:  If $\bar{x} \geq |q|$, then reject $H_{0}$. So here are some mathematical reasonings and connections:

- Reject $H_{0}$ if $|z| >= q$. Equivalent to $\mu_{0} \notin (\bar{x} - q\frac{\sigma}{\sqrt{n}}, \bar{x}+q\frac{\sigma}{\sqrt{n}})$. This makes sense since if your assumed mean is not falling in your estimated range you get from your samples, hey maybe it's not a correct assumption.
- Fail to reject $H_{0}$ if $\mu_{0} \in (\bar{x} - q\frac{\sigma}{\sqrt{n}}, \bar{x}+q\frac{\sigma}{\sqrt{n}})$. This makes sense as if your assumed mean is in your estimated range you got from your sample, hey that means there's a good chance that it's actually the population mean.

## Effects on the length of an interval 

### Effects of the Level of Confidence
In general an $(1-\alpha) * 100%$ confidence levle interval is given by:

- $(\bar{X}-q\cdot\frac{\sigma}{\sqrt{n}}, \bar{X}+q\cdot\frac{\sigma}{\sqrt{n}})$, where q (**critical value**) is the $(1-\frac{\alpha}{2})$-quantile of the normal distribution.

For a 95% confidence interval, we want teh middle area of the PDF to be $0.95$, then the areas on the tails was a total of $0.05$, with $0.025$ on each tail. The quantiles for these areas are:
```r
qnorm(0.025) # - 1.959
qnorm(0.975) # 1.959
```
So $q=1.96$. To re-emphasize, the middle of the pdf is $1-\alpha$, the tails should have $1-\frac{\alpha}{2}$ areas each. Now let's do a 0.98 confidence interval calculation:
```r
# We have 0.02 area at the tails. Each tail should have 0.01 area
confidence = 0.98
alpha = 1 - confidence

# Calculate the quantiles for each tail. Here we'll calculate the quantile 
# that encloses 99% of the data, so the right quantile. 
qnorm((1 - alpha/2)) # 2.32 for the right quantile, we'll just negate for the left

# Of course define xbar, sigma, etc. but this is just a toy example

# Find the confidence interval
c(xbar-q*sigma/sqrt(n), xbar+q*sigma/sqrt(n))
```

### Effects of the Sample Size
Again the margin of error for a confidence interval is $q\cdot\frac{\sigma}{\sqrt{n}}$ and the length of a confidence interval is twice the margin of error $L = 2\cdot q \cdot \frac{\sigma}{\sqrt{n}}$

We can control the sample size $n$, and that has a direct effect on the confidence interval. A useful equation is as follows:

- $n = (\frac{2\cdot q \cdot \sigma}{L})^{2}$, or $L=2q\frac{\sigma}{\sqrt{n}}$

- The expression $q\cdot\frac{\sigma}{\sqrt{n}}$ is called the margin of error to emphasize.

With this, we need to get a certain sample size for a confidence interval of a given length. For example: Calculate the length of a 95% confidence interval for the average workly hours to be at most 1 hour, using $\sigma = 15$:
```r
L = 1
sigma = 15

# Each tail should have 0.025 area beyond it, so this is -1.959.
# We negate it to get the right side quantile
# Note: I don't show it much, but negating seems to be the usual thing.
q = -qnorm(0.025) 
n = (2*q*sigma/L)^2 # 3457.313
```
You should round up. So we'd need a random sample of 3458 people to produce an interval of the desired length.

### Example 1: Confidence Intervals
A mountain as a reputed elevation of 9453 feet. A club decides to construct a confidence interval for $\mu$ (the true elevation of the mountain). We acquire a tool that has an expected value of $\mu$ and a standard deviation of 6 feet. Calculate the number of measurements ($n$) if we want to construct a 0.99-confidence interval for $\mu$, that has a length of 2 feet.

1. Confidence is 0.99, which means $\alpha = 1-confidence = 0.01$. Confidence intervals are two way, so we'll consider areas of both tails, which in total enclose 0.01 area away from the center. Each tail has 0.005 area beyond it. We can also reason that we don't know $\sigma$ so we'll be using the sample standard deviation of 6. Finally $L=2$. We're calculating for $n$!
```r
sigma = 6
L = 2
alpha = 1 - 0.99
q = qnorm(1-alpha/2)
n = (2*q*sigma/L)^2 # 238.85, round up always.
```
2. You need to take a sample of $n=239$ in order to get that confidence interval.