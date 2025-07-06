## Question 1
When a plug-in estimator is used to estimate a given parameter, what type of statistical inference are we conducting?

1. We're conducting point estimation. 

## Question 2
Which property is illustrated by the WLLN if we use the sample mean $\bar{X}_{n}$ as a point estimator of $\mu$?

1. This is consistency. WLLN says that $\bar{X}_{n}$ converges in probability to the population mean $\mu$ as sample size $n \to \infty$. This is the definition of a consistent estimator.

## Question 3 
What are the properties of the plug-in standard deviation if used as an estimator of the population standard deviation? Select all that apply:

- The plug-in standard deviation is biased. It will on average, underestimate the population standard deviation $\sigma$. If it were unbiased, it would on average get it exact, but that's not happening.
- It is consistent. The plug-in standard deviation estimator converges in probability to the true population standard deviation $\sigma$ as your sample size $n \to \infty$. By the law of large numbers 



## Question 4 
What are the properties of the sample standard deviation if used as an estimator of population standard deviation?

We're estimation the population standard deviation $\sigma$ using the sample deviation $s$: $s = \sqrt{\frac{1}{n-1}\sum_{i=1}^{n}{(X_{i} - \bar{X})^{2}}}$.
- The sample variance (with $n-1$) is an unbiased estimator since $E(s^{2}) = \sigma^{2}$. However the sample standard deviation $s = \sqrt{s^{2}}$ isn't an unbiased estimator of $\sigma$. The sqrt is non-linear, as the expected value of $s$ is slightly less than $\sigma$.
- It is consistent. As $n \to \infty$, the sample standard deviation $s$ will converge in probability to the population standard deviation $\sigma$. 
- It is not exactly equal to $\sigma^{2}$. Remember $s$ estimates $\sigma$ not $\sigma^{2}$.

Basically it's unbiased and consistent. So on average, doing using the sample standard deviation to estimate the population standard deviation will get it right. It's also consistent, so as $n \to \infty$, your sample standard deviation will converge $\sigma$.



## Question 5
A procedure that uses data to test whether a claim seems consistent with the data observed corresponds to what type of inferential method?

1. Hypothesis testing

