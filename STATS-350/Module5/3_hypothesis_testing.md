# Hypothesis testing

## Motivation Behind Hypothesis Testing

- Claim is made about parameter (e.g. $\mu$). Claim is also known as the null hypothesis.
- A random sample from this population is obtained. Here the sample mean $\bar{X}_{n}$ is calculated.
- Assuming the claim about the parameter is true, how likely is it to determine the probability to observe a value as or more extreme than $\bar{X}_{n}$?
- If getting $\bar{X}_{n}$ is very unlikely, we can reject the claim (reject null hypothesis)

### Example: Coin Toss
- Claim a coin is fair when tossed
- Toss the coin 100 samples and obtain 39 heads.
- Assuming the claim is fair, what's the chance of getting a sample where we get 39 heads?

1. Think of 100 different RVs, each toss is an indepndent bernoulli trial $X_{1}, X_{2}, X_{100} \sim Bernoulli(p)$. Where p is the probabilt yof getting heads. Remember the expected value for a bernoulli trial is $E(X_{i}) = \mu = p$ and $Var(X_{i}) = \sigma^{2} = p(1-p)$ for all i values. 

2. Sample mean $\bar{X}_{n} = \frac{39}{100}$, just the proportion of heads observed. We can say $E(\bar{X}_{100}) = \mu = p = 0.5$. Again expected value of sample mean, is just the expected value of a single trial
3. Sample Variance $Var(\bar{X}_{100}) =  \frac{\sigma^{2}}{n} = \frac{0.5(0.5)}{100} = 0.0025$.
4. Through the central limit theorem, we can say the distribution of sample means is approximately normal. (Due to sample size $\geq 30$).
5. Now what is the probability of getting a sample proportion of at most $0.39$. Mathematically, we can represent this as, given a $Y \sim Normal(\mu=0.5, \sigma^{2} = 0.0025)$, find $P(Y \leq 0.39)$?

```r
# P(Y <= 0.39) = 0.0139; So the likelihood of observing 39 heads or LESS is about 1.39%
pnorm(q=0.39, mean=0.5, sd=sqrt(0.0025))
```
So if the claim really is true, the probabillity of observing 39 heads or less is about 1%. So we'll reject this claim, saying that it's false.

## Formal Hypothesis Testing

### Null and Alternative Hypotheses
A hypothesis test is a procedure that uses data to make decisions about 2 competing statements. The null hypothesis, $H_{0}$, and the alternative hypothesis, $H_{1}$, are two opposing statements about a population parameter.

For example, the statement could be that the population mean is equal to 5 (null hypothesis), or it is not $\mu \neq 5$ (alternative hypothesis). For this we can write:
- $H_{0}: \mu = 5$. For procedural reasons, we always incldue the equal sign in $H_{0}$.
- $H_{1}: \mu \neq 5$

When doing a hypothesis test you always assume that the null hypothesis is true and interpret your results based on that. The result of the test ends with 2 options:
  1. The probability of getting the data is not unlikely when we assume $H_{0}$ is true. So you can't reject the null hypothesis, meaning the null hypothesis is still in the running, still has the possibly of being true.
  2. Here the probability of getting this data is very unlikely when assuming the null hypothesis is true. So since we got that unlikely data so easily, it may be an indicator that the null hypothesis is false. So here you reject the null hypothesis and accept the alternative hypothesis.

It's analogous to a court system. The person is innocent until proven guilty. So we start with the assumption that they are innocent (assume null hypothesis is true), and it only shown through overwhelming evidence that they are guilty (reject $H_{0}$ and accept $H_{a}$).


### Type 1, Type 2 Error, and Power
- **Type 1 Error:** When you reject the null hypothesis even though the null hypothesis is true.
  - Typically you don't know whether or not your null hypothesis is true. If you knew this you wouldn't be performing a hypothesis test in the first place. 
  - We can denote the probability of committing a Type 1 error as $\alpha$. This is also known as the significance level.
- **Type 2 Error:** The probability of failing to reject a false $H_{0}$. This is the probability of making a Type 2 error, denoted as $\beta$.
- **Power of the Test:** Calculated as $power = 1 - \beta$. This is the probability of correctly rejecting a false $H_{0}$. "Probability of not making a type 2 error".

There's an inverse relationship: as $\alpha$ decreases, $\beta$ tends to increase, assuming everything else stays constant. Remember there are four situations:
  1. $H_{0}$ true and we rejected it, that's an error. This is a type 1 error since we shouldn't reject the null hypothesis when it's true. Again this probability is $\alpha$.
  2. $H_{0}$ is false and we reject it, that's good!
  3. $H_{0}$ is true and we fail to reject it, that's good as well.
  4. $H_{0}$ is true, and we reject it. That's bad, $H_{0}$ is true, we shouldn't be rejecting it. This is a type 2 error, denoted by $\beta$


Let's say that $H_{0}: \mu = \mu_{1}$ and $H_{1}: \mu \neq \mu_{1}$. When you're assuming that the $H_{0}$ is true, you probably have a relatively normal distribution. Again if you decrease your sample size, the distribution is wider, else it gets more narrow. That significance level $\alpha$, is the probability rejecting $H_{0}$ even if it is true.

How to increase the power?
  - Increase $\alpha$. However increasing $\alpha$ will also increase P(Type 1 Error). For a lot of researchers, if the chances of getting a type 2 error are more serious, we'll likely increase the $\alpha$ to increase power, and therefore decrease P(Type 2 Error).
  - Increase sample size, which amkes both distributions get narrower. There's a lot less overlap between the two sampling distributions. 
  - Less variability means narrower distributions, and so the power increases as well.
  - If the true parameter is far from the null hypothesis, then that increases power. Now these past 2 aren't really under your control. 

### Hypothesis Configurations

1. A one-sided left-tailed test:
  - $H_{0}: \mu \geq \mu_{0}$, $H_{1}: \mu < \mu_{0}$.
  - So you'd find probability at or lower than the test statistic. 
2. A one-sided right-tailed test:
  - $H_{0}: \mu \leq \mu_{0}$, $H_{1}: \mu > mu_{0}$
  - Find probability at or higher than the test statistic.
3. A two-sided or two-tailed test:
  - $H_{0}: \mu = \mu_{0}$ versus $H_{1}: \mu \neq \mu_{0}$
  - You'd find probability at both ends. $2 * P(Z > |z|)$. 

Here $\mu_{0}$ is a constant provided as a part of the original claim. Notice how the equal equal is always part of the null hypothesis. Here $\mu$ is our true population proportion, so it's the idea that we're saying the true population parameter is different than the one that they initially gave us.

### The Test Statistic
A test statistic is based on an estimator and is constructred to follow a known distributed. Here we have one called the Wald test statistic:

$test statistic = \frac{estimator - parameter under H_{o}}{standard error}$

It's in that form. When constructing a test statistic for the sample mean $\bar{X}_{n}$, we have two cases"

- First is when population standard deviation $\sigma$ is known. Here $Z \sim Normal(0, 1)$, due to the CLT. Once data is collected the observed test statistics is:
  - Test Statistic: $Z = \frac{\bar{X}_{n} - \mu_{0}}{\sigma / \sqrt{n}} = \frac{\bar{X}_{n} - \mu_{0}}{\sigma_{\hat{\mu}}}$
  - Observed Test Statistic: $z = \frac{\bar{x} - \mu_{0}}{\sigma / \sqrt{n}}$ 

  - The standard deviation is equivalent to the standard error, just different names due to the context change.

- Second is when $\sigma$ is unknown, which is true for most situations. 
  - Test Statistics: $T = \frac{\bar{X}_{n} - \mu_{0}}{S_{n} / \sqrt{n}}$
  - Sample Standard deviation: $S_{n} = \sqrt{\frac{\sum{(X_{i} - \bar{X}_{n})^{2}}}{n-1}}$
  - Observed Test Statistic: $t = \frac{\bar{x} - \mu_{0}}{s/\sqrt{n}}$
  - Observed sample standard deviation: $s = \sqrt{\frac{\sum{(x_{i} - \bar{x})^{2}}}{n-1}}$
  

Here $T$ is no longer normal, but rather it follows a T-distribution wtih n-1 degrees of freedom. So $T \sim T_{n-1}$ For a T-distribution, all you need to know is that it's very similar to the standard normal distribution, just a little thicker at the tails.

The reason we include those formulas for the observed statistics and whatnot is because the idea is that we take one random sample. Then we use its information to calculate the data!

To explain n degrees of freedom. Let's say sample mean is 10, and you have 100 observations (n=100). 99 of those values could be any values, but the 100th value has to make the sample mean equal to 10. I mean how many values can we choose to have any value? Well we can pick 99 values to be anything, it's just that 100th value. So that's the idea of 1 degree of freedom.

One last thing, the larger your $n$ value, the more similar it will be to the standard normal distribution. But still we'll use the proper functions for the T-distribution.

### Significance Probability (p-value)
The significance probability or p-value is the probability of observing a test statistics as extreme as or more extreme than the one observed in our data when we assume $H_{0}$ is true. The p-value is obtained using: $p = P(Z \geq |z|)$

In R, we obtain the p value with 
```r 
# Gets the probability gained from both tails. 
2 * (1-pnorm(abs(z)))
```

### Conclusion and Interpretation of Results
When the data (sample) obtained produces a test statistics with a p-value that's small, it just means it's highly unlikely to get the data you got if $H_{0}$ is actually true. Therefore we reject $H_{0}$.
- Your threshold on how small the p-value will be based on the significance level $\alpha$ (probability of type 1 error). You usually pick a small $\alpha$, in range $0.001 \leq \alpha \leq 0.1$. You should definitely decide on your $\alpha$ before you start your experiments.
- After performing test, if $p-value \leq \alpha$, then reject $H_{0}$ and accept $H_{1}$. Else this means $p > \alpha$ so we fail to reject $H_{0}$.

### Tying it together: Step By Step 
1. State your null and alternative hypotheses.
2. Calculate your test statistics
3. Obtain the significance probability or p-value.
4. Provide a conclusion for the test 


## Example 1: Hypothesis Test
Our hypotheses:
  - $H_{0}: \mu \leq 3$
  - $H_{1}: \mu > 3$

Then our sample data:
  - $\bar{x} = 3.2$
  - $n = 100$
  - $\sigma = 1$

1. You can also tell that this is a right-tailed test since we're trying to see if it's greater.  
2. $Var(\bar{X}_{100}) = \frac{\sigma^{2}}{n} = \frac{1}{100} = 0.01$.
3. $SD(\bar{X}_{100}) = 0.1$.
4. Assuming that the population mean $\mu = 3$, what is the likelihood of us getting a sample that's 3.2 or more? I say or more because the alternative hypothesis is suggesting that $\mu > 3$. The probability of getting this data or something more extreme is the p-value.
5. We can easily do the calculations by converting this to the standard normal $z = \frac{\bar{X}_{100} - \mu}{\sigma / \sqrt{n}} = \frac{3.2 - 3}{1 / 10} = 2$. So our z-score is 2, meaning our sample is 2 standard deviation the mean. Our last question is what's the probability of observing data that's 2 standard deviations or more above the mean.

## Example 2: Hypothesis test from two perspectives
A group of parents want speed bumps to be installed in front of a school, but the city traffic office is reluctant. If the average speed of all motorists exceeds 15 miles per hour during school session, then it'll be installed. Let $\mu$ denote the average speed of all motorists in question. We have a random sample of $n=150$ motorists who were observed with a sample mean $\bar{x} = 15.3$ with a sample standard deviation of $s = 2.5$ mph. 

1. Setup the hypotheses for the parents:
  - $H_{0}: \mu \geq 15 $; This is what the parents believe!
  - $H_{1}: \mu < 15$; They would need the City traffic office to come up with evidence that this isn't true. The burden is on the city traffic office so that we can reject null and accept the alternative.
2. Setup the hypotheses for city traffic:
  - $H_{0}: \mu \leq 15$. The city traffic office believes that the cars are going at most 15.
  - $H_{1}: \mu > 15$. Here we want the citizens to come up with proof that the null hypothesis is false. It's the citizens that bear the burden of proof.

3. Calculate test statistic. Population standard deviation $\sigma$ is unknown so you typically you use the t-statistic, but here since $n$ is pretty big, he calculated the z-score since it was almost like a normal dist.
  - $z = \frac{\bar{x} - \mu}{s / \sqrt{n}} = \frac{15.3 - 15}{2.5 / \sqrt{150}} = 1.469$. So this sample mean is 1.469 standard deviations above the mean.
4. Assume the parents are willing to risk a 1% chance of a Type 1 error. From the parent's perspective, they're asking whether we have enough evidence to reject $H_{0}$. So this means $\alpha = 0.01$, and we want to know about finding values that are less than 15, to the left of $z=1.469$.

```r
# P(z < 1.469) = 0.92; Makes sense. Our sample mean is 15.3, that's not going to provide evidence that the true mean is actually less than 15
pnorm(z)
```
4. Adopting the city traffic office's perspective, they're willing to risk a 10% chance of committing a Type 1 error. Here it's a one sided right test
```r
1 - pnorm(z) # 0.0708 <= 0.10, so reject the null hypothesis.
```

5. Then you use R calculation to calculate $P(Z \geq 1.469)$
```r
# P(Z >= 1.469) = 1 - P(Z < 1.4.69) = 1 - P(Z <= 1.4.69) = 0.0708
1 - pnorm(z=1.469)
```

## Example 3: Alzheimer's Experiment (Paired Test)
To study Alzheimer’s, a scientist administers two tasks neuropsychological to $n=60$. patients. The idea is that we show the patients a picture and tell them to describe it. We measure the quality of their responses using "information units". There are two tasks:
  - Showing Picture A in the morning.
  - Showing Picture B in the afternoon

We're wondering if asking the same patient to describe picture A in the morning is equivalent to telling the same patient to describe picture B in the afternoon. To investigate, she computes the number of information units for Picture A minus the number of information units for Picture B for
each patient. She finds $\bar{x}=-0.1833, s = 5.18633$ Formulate null and alternative hypotheses that are appropriate for this situation. Calculate a significance probability. Do these results warrant rejecting the null hypothesis at a significance level of $\alpha = 0.05$?

1. Note that this is a paired sample because we are testing the same patient twice. Once in the morning and again in the afternoon. In any case, this should just be a simple one-sample t-test. T-test because we don't know $\sigma$, so we have to calculate t as the test statistic.
2. Let $\mu$ be the true mean differnece in information units between morning and afternoon:
  - $H_{0}: \mu = 0$, which indicates there's no difference between describing A in the morning and B in the afternoon.
  - $H_{1}: \mu \neq 0$, which indicates that there's a difference.
  - Assume the null hypothesis, so $\mu_{0} = 0$.

3. Calculate the t-statistic:
  - $t = \frac{\bar{x} - \mu_{0}}{s / \sqrt{n}} = \frac{-0.1833 - 0}{5.18633 / \sqrt{60}} = -0.2737$
4. Calculate $p = P(X \neq t ) = 2 \cdot P(X > t) = 2 \cdot (1- P(X \leq |t|)) = 0.785$
```r
# Don't forget about df = n - 1 = 59; 
# Note: We calculate the right tail initially, but regardless you should get the same answer.
one_tail_probability = 1 - pt(q=0.2737, df=59)
p_value = 2 * one_tail_probability
p_value # p = 0.785
```
5. Since $p > \alpha$, we fail to reject $H_{0}$.

## Example 4: Calculating Power and Beta 
We do an SRS of $n=36$ from a normally distributed population where $\sigma=21$, but $\mu$ is unknown. We're going to test:
  - $H_{0}: \mu = 50$
  - $H_{1}: \mu < 50$; left side test
  - At $\alpha = 0.09$
  - Calculate the values of Z that we can use to reject $H_{0}$. Remember since we have $\sigma$ and it's a normally distributed population, we can use a z-statistic. If our calculated z-value from the sample is less than the z-value that corresponds with the 9% quantile, we'd reject $H_{0}$.

1. You can reason that $\alpha$ is just quantile that encloses 0.09 area (from standard normal). Calculate $z_{\alpha} = -1.34$. So reject $H_{0}$ if z value from our sample is less than $-1.34$. Now that we have a threshold from the standard normal, we should be able to apply this to our own distribution. However, what's the sample mean corresponding to that z-score threshold? Doing some algebra, we get $\bar{x} = \mu_{0}+\frac{\sigma}{\sqrt{n}}z = 50+\frac{21}{\sqrt{36}}()(-1.34) = 45.31$. Reject $H_{0}$ if $\bar{x} \leq 45.31$. 

3. That $\alpha$-quantile also encloses the probability of doing a type 1 error. When we reject the $H_{0}$, there's the chance we could be committing Type 1 Error. There's only 0.09 probability for rejecting $H_{0}$, and therefore the same chance for rejecting it erroneously.

4. Let's say we find out that $\mu = 43$, what is P(Type 2)? 
  - Here $H_{0}: \mu =50, H_{1}: \mu < 50$
  - Now P(Type 2 error) = P(Do not reject $H_{0} | \mu = 43$). So given that the true mean is something else, that $H_{1}$ is true, what's the chances of not rejecting the false $H_{0}$, is what this is saying.
  - Remember we reject $H_{0}$ when $\bar{x} \leq 45.31$. So we'll fail to reject $H_{0}$ when $\bar{x} > 45.31$. Then we can say the probabillity of not rejecting is $P(\bar{x} > 45.31 | \mu=43)$. Again we say given 
   
Visually you'd have two distributions:
  1. Distribution of $\bar{X}$ if $\mu = 43$.
  2. Distribution of $\bar{X}$ if $\mu=50$. Here $P(X \leq 45.31)$ is when we reject $H_{0}$ and then $P(X > 45.31)$ is not rejecting $H_{0}$. This makes sense I guess.
  3. Now the probability of a type 2 error is actually the area to the right of $45.31$, assuming that $\mu=43$. So on your $\mu=43$ distribution (alternative hypothesis dist.) do $P(X > 45.31)$. Imagine this as still calculating the probability of rejecting $H_{0}$, but now when $\mu$ was actually 43. This shows the P(Didn't reject H0 | H0 was false). The probability of Type 2 **error ($\beta$)**.  
  4. Now calculate $P(\bar{x} > 45.31 | \mu = 43)$. You can just standardize this with $P(z > \frac{45.31-43}{21/\sqrt{36}}) = 0.255 = \beta$. Remember we standardized, so do the calculations based on the standard normal distribution.
  5. Then power = $1-\beta = 1-0.255 = 0.745$ 


**Note:** 
  1. Setup $H_{0}$ and $H_{1}$. For plot them out for visualizations.
  2. For $H_{0}$, find $\alpha-quantile$. You know that beyond that point, you reject $H_{0}$. Label the sides on what you're going to do.
  3. Now you should be using that true mean, and finding the P(Type 2 Error). Now to do this remember that P(Type 2) = P(Don't reject H0 | true mean you got (under $H_{1}$ I guess))). It's the probability that you're not rejecting $H_{0}$ given that $H_{0}$ is false. Here you're drawing that second distribution, and seeing what area of the $H_{1}$ overlaps with that "Do not reject" area of the $H_{0}$ distribution. That's all it is.
  4. You should have $\beta$ now and you should be able to easily calculate power.
  
## Example 5: Bird Influenza Hypothesis Test 
We're dealing with a serious virus such as bird flu. Suppose a kid has already died from a sick bird and we've already identified several sick birds. The scientists would prefer to be cautious and destroy all diseased birds, but the farmers don't want this to happen unless it's absolutely necessary. The farmesr and scientists agree that all chickens should be destroyed if more than 2 percent of the populatiion is diseased. They take a random smaple of $n=1000$ which has 40 diseased chickens.

a. Let $X_{i}=1$ if the ith chicken is diseased and $X_{i} = 0$ if not. Assume $X_{1}, ..., X_{n} \sim P$.
We can say $P$ is a family of bernoulli trials. We can show:
  - $\mu = E(X_{i}) = p$
  - $Var(X_{i}) = p(1-p) = \mu(1-\mu))$; idea of p and mu being the same value.
  - Sample statistics $E(\bar{X}_{n}) = \mu = p$ and $Var(\bar{X}_{n}) = \frac{\mu(1-\mu)}{n}$

b. Let's talk about the null and alternative hypotheses from the perspective of the health officials. So the health officials would prefer to assume that the chickens are infected, it's the default for them. What would be news to them is that there were less than 2 percent infected. This is a one-sided left tailed test because evidence against the null would appear on the left tail of the distribution:
  - $H_{0}: \mu \geq 0.02$
  - $H_{1}: \mu < 0.02$

c. From the perspective of the farmers, they prefer to assume that there are less than 2% of chickens infected. That's their default, and it would be news to them if it was more than 2 percent. So the idea is that here we would need to provide proof against that null hypothesis. This is also a right-tailed test:
  - $H_{0}: \mu \leq 0.02$
  - $H_{1}: \mu > 0.02$

d. Use the $\mu_{0}$ value from the hypotheses (should be the same value of 0.02) to compute $\sigma_{2}$ under $H_{0}$. Then compute $z$ the test statistic:
  - $\sigma^{2} = Var(\bar{X}_{n}) = \frac{\mu_{0}(1-\mu_{0})}{n} = \frac{0.02(1-0.0.2)}{1000} = 1.96 \cdot 10^{-5}$. We know $\bar{X}$ is a sample of Bernoulli trials, so we don't ned to estimate the variance as a function of the mean.
  - $$





## Credits
- [Introduction to Type 1 and Type 2 errors - Khan Academy](https://www.youtube.com/watch?v=Hdbbx7DIweQ)
- [Introduction to power in significance tests - Khan Academy](https://youtu.be/6_Cuz0QqRWc?si=JQSrCeadhP8oWT0g)
- [Calculating Power and the Probability of a Type 2 Error - jbstatistics](https://youtu.be/BJZpx7Mdde4?si=ps5ik_dR757Qg_ay)
