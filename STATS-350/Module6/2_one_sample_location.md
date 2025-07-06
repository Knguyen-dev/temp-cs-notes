# One Sample Location Problems

To recognize one sample location problems, answer the following questions:
1. What are the experimental units or objects being measured.
2. What population (multiple?) were the experiments drawn from.
3. What measurements were taken on each experimental unit.
4. What random varaibles are relevant to the specified inference. 

## One Sample Normal Location Problems
When we can assume the population is normal, how we deal with hypothesis testing depends on the sample size $n$ and whether or not $\sigma^{2}$ is known:

1. Normal population is sample size is large:
  -  $\sigma^{2}$ is known. Use $Z=\frac{\bar{X}_{n} -\mu_{n}}{\sigma / \sqrt{n}}$. Based on $Z \sim N(0, 1)$
  - $\sigma^{2}$ is unknown. Since $n$ is large, due to the CLT, the test statistic $T=\frac{\bar{X}_{n} - \mu_{0}}{S_{n}/\sqrt{n}}$. Still based on $Z \sim N(0, 1)$
2. Normal population, and sample size n isn't large:
  - $\sigma^{2}$ known: Test statistic $Z=\frac{\bar{X}_{n} - \mu_{0}}{\sigma/\sqrt{n}}$. This follows $Z \sim N(0,1)$.
  - $\sigma^{2}$ unknown: $T=\frac{\bar{X}_{n} - \mu_{0}}{S_{n}/\sqrt{n}}$. Here $S_{n} = \sqrt{\frac{\sum{(X_{i}-\bar{X}_{n})^{2}}}{n-1}}$. Here $T$ follows a T-distribution with $n-1$ degrees of freedom.

## About the T Distribution
The PDF of the T-distribution is symmetric, bell-shaped, and centered at 0. However it has thicker tails than the standard normal, and an extra parameter called the degrees of freedom. This is calculated as $n-1$, where $n$ is the sample size. When the sample size is large enough, the T-distribution is almost identical to the standard normal. So when the sample size is large enough, you can use the normal distribution, else use the T-distribution.


## Student's One Sample T-Test
When the population is normal, the sample size $n$ isn't large, and $\sigma^{2}$ is unknown, we use a 1-sample T-test. Remember this relates back to hypothesis testing as $\mu_{0}$ is the value of the population mean under $H_{0}$. Let's look at the significance probabilities: 
- Left-tailed test: p-value = $P(T \leq t)$
  - In R: `pt(t,df=n-1)`
- Right tailed test: p-value = $P(T \geq t)$
  - In R: `1- pt(t, df=n-1)`
- Two-tailed test (both sides): p-value = $P(T \geq |t|)$
  - In R: `2 * (1-pt(abs(t), df=n-1))`

Reject $H_{0}$ if $p \leq \alpha$. I feel like most of this is a review from last chapter though.