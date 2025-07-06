# Point Estimation

## What is statistical inference?
Any procedure where we're extracting info about the population probability distribution from an observed sample. One thing we could try to estimate is the population mean $\mu$. A sample could be used to answer the following questions 
- What's the best guess we can make about $\mu$?
- Assume a claim about $\mu$, does our sample provide evidence that the claim is wrong? This is called hypothesis testing "can we reject or not reject the hypothesis/claim".
- What are the plausible values of $\mu$?

There are 3 types of estimations:
  - Point estimation: Use a formula or equation to estimate the value of $\mu$.
  - Hypothesis testing: Determine whether or not a claim about $\mu$ can be rejected based on the data.
  - Set estimation: Provide an interval to estimate $\mu$.

We're going to be covering point estimation in this write-up.


## Point Estimation

### Terminology and Motivation
When working with data, we almost never get to observe the whole population. 
  - We still want to undersatnd the true value of something e.g. the population mean $\mu$.
  - But all we have is a sample, so we build estimators that give us estimates of the poplation parameters (the estimands).
- **Estimand:** The parameter we want to know. E.g. "What's the average number of hours all students at a university sleep?" This unknown population mean is the estimand $\mu$.
- **Estimator:** The method or formula you use on a sample. An example is randomly asking 100 students and computing their average sleep hours. The function $\bar{X}_{n} = \frac{1}{n}\sum_{i=1}^{n}{X_{i}}$ is the estimator.
- **Estimate:** The actual result we get from the estimator. For example, you sample 100 students and find they average 6.8 hours. That's your estimate, a number e.g. 6.8.


### Using an analogy: Treasure Hunting
Imagine you have a hidden treasure chest (**the population**). You want to know exactly how much gold is in it (**the estimand**). You can't open the whole chest, byut you're allowed to grab a few handfuls (**a sample**). Then based on these handfuls, we'll try to guess the total gold.
- **Estimand (The actual amount of teasure):** It's the true, fixed, but unknown value you're trying to figure out .It existsly independently of any data you get. It's a characteristic of the population. This is similar to other estimands from other examples:
  - The true average height of all adult males in Canada ($\mu$)
  - The true proportion of voters who will vote for a particular candidate in the next election ($p$)
  - The true standard seviation of test scores for all students in a district ($\sigma$).
- **Estimator (Your strategy/tool):** It's the rule, formula, or procedure we use to amke a guess about the estimand based on our sample. It's like the specific mmethod youuse to weigh your handfuls of gold and extrapolate to the total chest amount. The estimator itself is a random variable since its value dependso n the random sample you happen to draw. Here are some real examples:
  - To estimate $\mu$, we use the sample mean $\bar{X}_{n} = \frac{1}{n}\sum_{i=1}^{n}{X_{i}}$.
  - To estimate the population proportion $p$ you'd use the sample proportion $\hat{P}_{n}$.
  - To estimate the population variance, you may use the sample variance.
- **Estimate (The Guess):** The specific number we get when we apply our estimator to a particular sample. It is the result of your strategy.

---
### Properties
- **Unbiased:** An estimator is unbiased if its expected value is equal to the estimand. So the mean of the sampling distribution of the estimator is equal to the true parameter value. In english "If on average it hits the true paramater value".

Imagine doing 1000 samples, if the average of those estimates is the true value, then the estimator is unbiased. For example $\bar{X}_{n}$ is unbiased since its expected value is the population parameter $\mu$. Indicated by $E(\bar{X}_{n}) = \mu$. The reason we care is because even if we get an weird sample that doesn't fit in, in the long run (if we keep sampling), this method will get the population parameter.

- **Consistent:** An estimator is consistent if it converges in probability to the estimand. Essentially, as the sample size increases the estimates (produced by the estimator) "converge" to the true value of the parameter being estimated. Or more technically, as the sample size increases, the sampling distribution of the estimator becomes incresaingly concentrated at the true parameter value.

The reason this matters is because, eve nif the method isn't perfect for small samples, you can reason that as you get more data, you're going to be closer to the true value. 


### Properties: Analogy
- Is the estimator generally close to the true estimand (unbiasedness)? 
- Does this estimator produce estimates that are consistency close to each other across different samples?
- Does this estimator get better as my sample size increases (consistency)?


### Example: Illustrating Unbiased and Consistency
Suppose someone creates a weird estimator for $\mu$ that always return 3 no matter the data. 

- Unbiased?: If unbiased this would mean the $E(\bar{X}_{n}) = \mu$. Well, if $\mu = 3$, yes that's unbiased, but that's probably not going to happen, so this is biased.
- Consistent? If consistent, then it means that increasing the sample size would make it so the estimator gets closer to the true value. Well that doesn't happen here because regardless of the sample size, it'll always return 3.

Now suppose someone makes another estimator that slightly overestimates for small samples, but gets more accurate as $n$ increases.
  - Unbiased? Probably not because for small sample sizes, the average of the sample means is gonna slightly overestimate the population parameter. 
  - Consistent? Yeah because as you increase the sample size, the expected value of the population parameter will slowly start to converge to the true value. So you'd use this with larger sample sizes.

### Example: Talking about variance and STDEV
```r
# Slight underatimation if you use plugin



# Very close
set.seed(101)
vec_sample_sd = replicate(10^4, sd(rnorm(n, mu sigma)))
```