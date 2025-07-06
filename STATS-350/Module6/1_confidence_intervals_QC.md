# Quick Check 1: Confidence Intervals

## Question 1
Using the faithful dataframe in R. The dataframe contains the waiting times between eruptions and the duration of the eruption for the Old Faithful geyser. Focus on the waiting time in minutes. Calculate the point estimate for the average waiting time between eruptions:

1. When they say "point estimate", they're just asking: Given a single sample, what's an estimate for the average waiting time. So we can say $E(\bar{X}_{n}) = \mu$. So first calculate the sample mean, and the expected value of the sample mean (as you take more samples), it averages out to the population mean.
```r
waiting_minutes = faithful$waiting 
mean(waiting_minutes) # 70.89
```

## Question 2
Assume $\sigma=13$ minutes. Construct a 95% confidence interval for the average waiting time in minutes. For the answer, provide the lower bound rounded to two decimals.

1. Confidence = 0.95, $\alpha = 1- confidence = 0.05$. Again, confidence intervals involve two tails, so each tail encloses 0.025 area away from the center. 
2. Remember $I = (\bar{X}-q\cdot\frac{\sigma}{\sqrt{n}}, \bar{X}+q\cdot\frac{\sigma}{\sqrt{n}})$
3. Here's the R code.
```r
waiting_minutes = faithful$waiting 
sigma = 13
sample_mean = mean(waiting_minutes) # 70.89
n = length(waiting_minutes)         # 272
q = -qnorm(0.025)                   # 1.95, just negated so I can get the positive version (right side quant).

c(sample_mean-q*sigma/sqrt(n), sample_mean+q*sigma/sqrt(n))
```
Interval is $(69.35214 72.44198)$

## Question 3
Assuming the population standard deviation is unknown, construct a 95% confidence interval.

1. Since we don't know the population standard deviation, we'll just have to use the sample standard deviation. 
```r
waiting_minutes = faithful$waiting 
sigma = sd(waiting_minutes)         # 13.59
sample_mean = mean(waiting_minutes) # 70.89
n = length(waiting_minutes)         # 272
q = -qnorm(0.025)                   # 1.95, just negated so I can get the positive version (right side quant).
c(sample_mean-q*sigma/sqrt(n), sample_mean+q*sigma/sqrt(n))
```
Interval is $(69.28143, 72.51269)$.

## Question 4
Let's say you want to construct a 97% confidence interval, what is the critical value (quantile) $q$ that you need to use? Report the positive $q$ rounding.
```r
# The middle pdf should have 0.97 area, and the tails have 0.03 area. Each tail encloses 
# 0.015 area away from the middle.
confidence = 0.97
alpha = 1-confidence

# Here I'll calculate the quantile for the left tail
q1 = qnorm(0.015) # -2.17009

# Here I'll calculate the quantile for the right tail
q2 = qnorm(1-0.015) # 2.17009
```

## Question 5, 6, 7, and 8
Continuing from question 4, this time calculate the confidence interval. Assume $\sigma=13$, and then return the lower and upper bounds rounded to two decimals.

```r
confidence = 0.97
alpha = 1-confidence
q = qnorm(1-0.015) # 2.17009

waiting_minutes = faithful$waiting 
sigma = 13
sample_mean = mean(waiting_minutes) # 70.89
n = length(waiting_minutes)

c(sample_mean-q*sigma/sqrt(n), sample_mean+q*sigma/sqrt(n))
```
The interval is $(69.18651, 72.60761)$. The rounded version is $(69.19, 72.61)$

## Question 9
Let $\sigma=13$, we're still constructing the 97% confidence interval. What's the minimum sample size needed if we want to create a confidence interval with a length at most of 2 minutes?

1. Here we know $L=2$ and we're calculating $n$. The formula for this is $ n = (\frac{2\cdot q \cdot \sigma}{L})^{2}$.
2. $n = (\frac{2 \cdot 2.17 \cdot 13}{2})^{2} = 795.87$
3. So you need to have a sample size of 796 for your interval to be 2 minutes. 


## Question 10

We are 95% confident that the average waiting time betwee neruptions is between 69 and 72 minutes. What does 95% confidence mean in this case?

- Out of all hte possible intervals obtained using our procedure, 95% of them contain the true average waiting time $\mu$.