# Quick Check: Hypothesis Continued 

## Question 1
Company advertises that their lightbulbs burn an average of 800 hours before dying. However consumer organizations believe that this isn't true, however they need to have some statistical evidence for this. They aren't going to allocate funds for a campaign if there's no evidence. We get a sample of $n=100$ lightbulbs. Then we calculate the sample mean $\bar{x} = 745.1$ hours before dying and a sample standard deviation of $s = 238.0$.

Who bears the burden of proof?

1. Consumer agency


## Question 2

We can say that the $H_{0} \geq 800$. This makes the most sense for the situation.
- $H_{0}: \mu \geq 800$
- $H_{1}: \mu < 800$ 


## Question 3
Obtain a test statistic.

1. We don't know $\sigma$, and even if we assume the null hypothesis, we can't derive it. So instead of a Z, we use a T distribution. 

2. $t = \frac{\bar{x} - \mu}{s / \sqrt{n}} = \frac{745.1 - 800}{238 / \sqrt{100}} = -2.30$

## Question 4
Calculate the significance probability (p-value):

1. I'm assuming this is a left-tail test as the alternative hypothesis wants to prove the average is actualy smaller than $800$. 
  - Now we're going to calculate the probability that $P(T \leq -2.30)$. 
  - $df = n -1 = 99$

2. Here's the R code:
```r
# P(T <= -2.30)
pt(q=-2.30, df=99) = 0.011
```

## Question 5
If $\alpha = 0.01$, what can we conclude?

1. Since $p > \alpha$, we cannot reject the null hypothesis.