# Quick Check: Hypothesis Testin 

## Question 1
Blaise Pascal, argues that we can't know whether or not God exists, but we should behave as though we do. He believes that the consequences of behaving as though God doesn't exist are greater than when behaving as though God does exist. He concludes to better live on the side of caution and act as though God exists. What is his null hypothesis?

1. Apparently the null hypothesis is that God does exist.
  - $H_{0}:$ God does exist 
  - $H_{1}:$ God doesn't exist

## Question 2
What is type 1 error?
1. Type 1 would be rejecting the null hypothesis even though it's true.
2. Concluding that god doesn't exist when in reality God exists.

## Question 3
Where is the burden of proof?

1. On nonbelievers.

## Question 4
Which hypothesis receives a privileged status under the Neyman-Pearson formulation?

1. The null hypothesis, with the idea that you don't have to prove it.


## Question 5
What is your decision if the significance probability (p-value) is 0.02?

1. We need a significance level before we can decide what to do.

## Question 6
To test $H_{0}: \mu = 0.5$ versus $H_{0}: \mu \neq .05$ at $\alpha = 0.03$. We performed $n=1000$ trials and observed 450 successes. Calculate the observed proportion of successes.

1. $\frac{450}{1000}=0.45$


## Question 7
Now let $n=700$ trials and we observed 322 successes. Calculate the test statistic. 

1. Assuming the null hypothesis, we'll let population parameter $p=0.5$. These are bernoulli trials, so the population variance is $\sigma^{2} = p(1-p) = 0.5(1-0.5) = 0.25$. 
2. Calculating the sample standard deviation, we'll first do sample variance. $Var(\bar{X}_{700}) = \frac{\sigma^{2}}{n} = \frac{0.25}{700}$. Then take the square root to get $\sigma_{\hat{p}}$ or $\sigma_{\bar{X}_{700}} = 0.01889$

3. We'll our population standard deviation $\sigma$ is known since we're assuming the $H_{0}$. Since that's known, we can calculate z statistic instead of trying to calculate a T-statistic.
  - $\hat{p} = \frac{322}{700} = 0.46$

  - $z = \frac{\hat{p} - p_{0}}{\sigma / \sqrt{n}} = \frac{0.46 - 0.5}{0.01889} = -2.117$

## Question 8
What is the significance probability (p-value)?

1. Since $\neq$, we're calculating probabilities at both tails. So we need probability of data being lower than -2.117 standard deviations and higher than 2.117 standard deviations. We can do this in R and assume a normal distribution and remember that this is z-score so this works with the $Z \sim Normal(0, 1)$ distribution. You can think of this as $p = P(Z \geq |z|)$

```r

# P(Z >= z_score) = 1 - P(Z <= z_score)
one_tail_p = 1- pnorm(2.117)
2 * one_tail_p # 0.034
```

## Question 9
Since our p value $0.034$ isn't less than $\alpha$ we can't reject the null hypothesis!

1. Fail to reject the null hypothesis.

## Question 10
To test $H_{0}:\mu = 10$ versus $H_{1}:\mu \neq 10$ at signifcance level $\alpha =0.1$, we collect $n=225$ observations, observing $\bar{x}=11.93$ and $s_{n}=17.45$. What is the test statistic (`t*`)? Answer with 2 decimals

1. We assume the null hypothesis, so for now $\mu = 10$. I don't think we can calculate the population variance like we did in the last one. So since we don't know $\sigma$, we should probably use a t-score.
2. $t = \frac{\bar{X}_{n} - \mu_{0}}{S_{n} / \sqrt{n}} = \frac{11.93 - 10}{17.45 / \sqrt{225}} = 1.659$

**Note:** Again that denominator is the sample standard deviation, aka as the standard error. So if someone asks you about the standard error, it's just the result of the denominator. 

## Question 11
Calculate the significance probability.

1. Since we're trying to get data for $\neq$, it wants to calculate the probability at both tails. So $p = P(T \geq 1.65) + P(T \leq -1.65)$. Or more simply, $p=2⋅P(T>∣t∣)$

2. The degrees of freedom $df = n - 1 = 225-1 = 224$. This will be useful later for calculating the p-value with a T distribution.

3. Now the R code should be pretty simple:
```r
t_stat <- 1.659
df <- 224

p_value = <- 2 * (1- pt(t_stat, df))
p_value # 0.098
```

## Question 12
Since $\alpha = 0.1$ and $p < \alpha$, we can reject $H_{0}$ and accept $H_{1}$