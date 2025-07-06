# Quick Check: Two Sample Location Problems

## Question 1
We wish to determine whether the time (in hours) IU seniors spend studying during finals week is greater than the IU freshmen time. A random sample of 45 freshmen and another sample of 45 seniors were obtained. Then each one was asked how many hours they spend studying during finals week. You are planning to work with seniors as group 1 and freshmen as group 2. What is the experimental unit?

1. Remember the experimental unit is the object being measured, rather than the parameter.
2. The object being measured is a student.

## Question 2
Were the samples drawn from normal distributions?

1. Since we were given the data, we can load the data into R and do the qqnorm and qqline plots on them. If it's a straight line, it's drawn fro ma normal distribution:
```R
setwd("C:\\Users\\KNguy\\OneDrive\\Desktop\\Comp Sci\\Spring2025\\STATS-350\\Module6")
getwd()

data <- read.table("qc_data.txt", header=TRUE)

# Both seem approximately normal, the departures aren't too extreme.
qqnorm(data$freshmen)
qqline(data$freshmen)

qqnorm(data$seniors)
qqline(data$seniors)

```

## Question 3
Is this a 1 or 2 sample problem?

1. 2-sample problem. You can tell that from this and the seed example, the sample sizes aren't equal, there's no idea that a pair of students. 

## Question 4
How many measurements were taken on each experimental unit?

1. Only one measurement is ever taken.

## Question 5
What is the parameter of interest?

1. The parameter of interest seems to be related to the difference in the population means $\Delta = \mu_{1} - \mu_{2}$, with 1 being seniors and the other being freshmen. 

## Question 6 and 7
What are the null and alternative hypotheses?

- The alternative is the idea that seniors spend more time studying, which is indicated by $\delta = \mu_{1}-\mu_{2} > 0$. Then the null hypothesis would be less than or equal:
  - $H_{0}: \Delta \leq 0 $
  - $H_{1}: \Delta > 0 $

## Question 7
Which test would you use?

1. You probably can't use z-tests because we don't know the population variances of the study times, which means you'll calculate a t-statistic and do  t test. It's probably not a one-sample t-test because it seems we have two populations.
2. Via the CLT, I think you can assume the populations $\bar{X}_{1}$ and $\bar{X}_{2}$ (sample means) are approx normal via the CLT.
3. You wouldn't really use student's one sample t-test because in that situation you'd needto say that the sample size is not large, but the sample size is $\geq 30$ here, so meets the CLT. Also it's one sample, and we have two populations, so probs no.
4. Welch's test should probably be used as it's needed to do a hypothesis test with two samples, when variances are unknown, the sample sizes are kind of large ($\geq 30$)

## Question 8 and 9
Let's calculate the test-statistic $t$ and estimated degrees of freedom $\hat{\nu}$:

1. The formula is $T_{w} = \frac{\hat{\Delta} - \Delta_{0}}{\sqrt{\frac{s^{2}_{1}}{n_{1}} + \frac{s^{2}_{2}}{n_{2}}}}$
```r
setwd("C:\\Users\\KNguy\\OneDrive\\Desktop\\Comp Sci\\Spring2025\\STATS-350\\Module6")
getwd()

data <- read.table("qc_data.txt", header=TRUE)

xbar1 = mean(data$seniors)
xbar2 = mean(data$freshmen)

n1 = length(data$seniors)
n2 = length(data$freshmen)

s1 = sd(data$seniors)
s2 = sd(data$freshmen)

Delta_hat = xbar1-xbar2     # 2.22
Delta_0 = 0 # Assumes H_{0}
se = sqrt(s1^2/n1 + s2^2/n2) # 1.28
t_welch = (Delta_hat - Delta_0) / se # 1.726
nu_hat = (s1^2/n1 + s2^2/n2)^2/((s1^2/n1)^2/(n1-1) + (s2^2/n2)^2/(n2-1)) # 87.965
```

## Question 10
Let's say you calculate the p-value and do a significance test with $\alpha = 0.03$, what's the conclusion?

1. Now let's assume the null hypothesis is true. Since the alternative hypothesis is greater, we're doing a right tailed test. We've already calculated the t statistic and estimated degrees of freedom. The p-value is $P(T \geq t)$. If the probability we found is less than alpha, then we have probable cause.
2. The math:
```r
# P(T > t_welch): So we're calculating the probability that the t_welch quantile encloses 
# to its right. We can say P(T > t_welch) = 1 - P(T <= t_welch)
pvalue = 1 - pt(q=t_welch, nu_hat) # 0.043
``` 
3. Since $p > \alpha$, we fail to reject the null hypothesis. This means we didn't find enough evidence to conclude that there's a significant difference in the average time that seniors and freshmen spend on studying.
4. "We did not find enough evidence to conclude that seniors study more time than freshmen on average".

## Question 11 and 12
Do a 98% confidence interval for the parameter of interest. What are the lower and upper bounds? (2 decimals).

1. $\alpha = 1 - confidence = 0.02$. Since we're doing a confidence interval, it means that both tails are in play, with 1% at each tail. You can understand it as putting intervals on the $\hat{\Delta}$ distribution. 
2. Confidence interval formula: $\hat{\Delta} \pm q \cdot \sqrt{\frac{s^{2}_{1}}{n_{1}} + \frac{s^{2}_{2}}{n_{2}}}$
3. Again the critical value (quantile) for a confidence interval is $a = (1-\frac{\alpha}{2})$. For a t-distribution, you'll need to consider the degrees of freedom. Normally with a one-sample t-interval or t-test, you could do $n-1$, but for a 2-sample interval you calculate the estimated degrees of freedom using a longer formula.
```r
alpha = 1-0.95
# Finds the quantile that encloses 2% of the data to the right
q = qt(1-alpha/2, df=nuhat)
lower = Delta_hat - q*se # -0.82
upper = Delta_hat + q*se # 5.272
```
