# Two Sample Location Problems 

The idea of performing inferential statistics with two samples. Let's review the framework for doing one-sample problems:
1. Let $X_{1}, X_{2}, ..., X_{n} \sim \mathbb{P}$ and they're identical distributed (same probabilities) and independent. Let $E(X_{i})=\mu$ and $Var(X_{i})=\sigma^{2}$.
2. The parameter of interest is the population mean $\mu$ and for a large enough $n$, due to the CLT, we know that $\bar{X}_{n} \sim N(\mu, \frac{\sigma^{2}}{n})$. 
3. The idea that the sample mean is also a random variable, and as $n \to \infty$, that random variable has certain properties.
4. Then we calculate some test statistic like $Z=\frac{\bar{X}_{n} -\mu_{0}}{\sigma/\sqrt{n}}$. Then we can either do a test and try to find a p value on the $\bar{X}_{n}$, or we'll calculate a confidence interval.

## Two Sample location framework
Let $X_{1}, ..., X_{n_{1}} \sim \mathbb{P}_{1}$ and $Y_{1}, ..., Y_{n_{2}} \sim \mathbb{P}_{2}$ are both idd. The frameworks are the same, and we also have a additional condition saying that both samples should be independent from each other.

We can reason that:

- $\bar{X} = \bar{X}_{n_{1}} = \frac{1}{n_{1}}\sum_{i=1}^{n_{1}}{X_{i}}$
- $\bar{Y} = \bar{X}_{n_{2}} = \frac{1}{n_{2}}\sum_{i=1}^{n_{2}}{Y_{i}}$

Our parameter of interest is the difference in population means $\Delta = \mu_{1} - \mu_{2}$. The estimator of the difference of means is $\hat{\Delta} = \bar{X}-\bar{Y}$. We know that $\bar{X} \sim N(\mu_{1}, \frac{\sigma^{2}_{1}}{n_{1}})$ and $\bar{Y} \sim N(\mu_{2}, \frac{\sigma^{2}_{2}}{n_{2}})$. The random variable representing the difference between two normal distributions will stay normal. That distribution is denoted as:

$\hat{\Delta} \sim N(\Delta, \frac{\sigma^{2}_{1}}{n_{1}} + \frac{\sigma^{2}_{2}}{n_{2}})$.

Then since $\Delta = \bar{X} - \bar{Y}$ we can apply our formulas of expected value and variance:
  1. $E(\Delta) = E(\bar{X}-\bar{Y})$
  2. $Var(\Delta) = Var(\bar{X}) + Var(\bar{Y})$

## The Test
The test is a direct extension of one-sample problem. For example, given the hypotheses $H_{0}: \Delta = 0$ versus $H_{1}: \Delta \neq 0$ and assuming that $\sigma^{2}_{1}$ and $\sigma^{2}_{2}$. The test statistic is:

$Z=\frac{\hat{\Delta} - 0}{\sqrt{\frac{\sigma^{2}_{1}}{n_{1}} + \frac{\sigma^{2}_{2}}{n_{2}}}} \sim N(0, 1)$


## One or Two Sample Problems 
We start with those questions that help us determine whether this is a one-or-two sample location problem:
- What is the experimental unit? The units must be independent.
- From how many population were the experimental units drawn from. 
  - How many units were from each population?
  - Is this a 1- or 2- sample problem?
- How many measurements were taken on each experimental unit? 
- What are the parameter(s)? $\mu$ 1-sample problems or $\Delta$ for 2-sample problems. 
- Do you need to do a significance test? If so, what are the $H_{0}$ and $H_{1}$.
  - In a one-sample location problem, the hypotheses should be statements about $\mu$ (or $\theta$).
  - In a two sample location problem, the hypotheses should be statements about $\Delta$.


### Example 1: Two Sample Problem
A drug is supposed to lower blood pressure. To determine if it does $n_{1} + n_{2}$ hypertensive patients are recruited to participate in a double blind study. The patients are randomly assigned to a treatment group of $n_{1}$ patients. Each patients in the treatment group receives the drug for two months; each patients in the control group receives a placebo for the same period. Each patient's blood pressure is measured before and after the two month period, so (pair test), and neither the patient nor the technician know which group the patient was assigned.

1. Experimental Unit: A patient
2. There are two different p opulations (control and treatment group). Individuals are assigned randomly. Samples are independent and people in the same sample are independent. So 2-sample problem
3. Then let's create a variables that represents the difference between the blood pressure before and after for a given patient. However we need to do this for both the treatment and the control group: 
  1. Difference in blood pressure (treatment) $X_{i} = B_{i} - A_{i}, i = 1, 2, ..., n_{1}$.
  1. Difference in blood pressure (control) $Y_{i} = B_{i} - A_{i}, i = 1, 2, ..., n_{2}$.

### Example 2: One Sample Problem
Pairs of seedling of the same age, one produced by cross-fertilization and the other by self-fertilization, were grown together in the same condition. We wanted to demonstrate the greater vigor of the cross-fertilized plants. 

1. The pair is a singular unit. So we're going to have 1 population. The fact that they are dependent on each other makes them a dependent unit. Also we aren't calculating two different population means, so that should also clear things up.
2. $X_{i} = C_{i} - S_{i}$. One sample for the difference in means.

## Welch's Two Sample T-Test

### Three Possible Cases for 2-Sample Normal Location
When you have two samples from approx. normal populations, there are different scenarios:
1. When variances $\sigma^{2}_{1}$ and are known:
  - This rarely happens since you're unlikely to have the knowledge of the true variances.
2. When the variances are unknown but equal:
  - You'd do a 2-sample Student's T-test. 
  - We rarely even know if the population varainces are equal or not, so this type of test could be bad. Prefer step 3 then.
3. When variances are unknown:
  - Use Welch's two-sample T-test. You use this when the populations are normal, so as long as the samples are resaonably large, they will be somewhat normal.

### Hypothesis Testing with Welch's T-Test
Here $H_{0}: \Delta = \Delta_{0}, H_{1}: \Delta \neq \Delta_{0}$ when doing the two-sided case. In the one sided case you'd $\Delta = \mu_{1} - \mu_{2}$ to do a difference in population means, where $\mu_{0}$ is your assumed difference from the null hypothesis.

Welch's two-sample T-test statistic is $T_{W} = \frac{\hat{\Delta}-\Delta_{0}}{\sqrt{\frac{s^{2}_{1}}{n_{1}} + \frac{s^{2}_{2}}{n_{2}}}}$. Here $\hat{\Delta}$ is the point estimator of $\Delta$, the $\Delta_{0}$ is the difference in means under $H_{0}$ and teh denominator is the standard error of $\hat{\Delta}$

Here's how you calculate the degrees of freedom: $\hat{v} = \frac{(\frac{S^{2}_{1}}{n_{1}} + \frac{S^{2}_{2}}{n_{2}})^{2}}{ \frac{(S^{2}_{1}/n_{1})^{2}}{n_{1}-1} + \frac{(S^{2}_{2}/n_{1})^{2}}{n_{2}-1} }$

### Confidence Interval with Welch
The confidence interval for $\Delta$ is:

$\hat{\Delta} \pm q_{t}\sqrt{\frac{S^{2}_{1}}{n_{1}} + \frac{S^{2}_{2}}{n_{2}}}$, where $q_{t}=qt(1-\alpha/2, \hat{v})$ is the critical value (quantile) needed from a t-distribution with $v$ degrees of freedom.


### Limitations of Welch's t-Test
It assumes two independent samples from normally distributed population, but there are some leniencies:
  1. You can still use it if the distributions aren't normal, just make your sample size large, symmetric, and without extreme outliers.
  2. However if these conditions don't apply you'd have to do some transformations and stuff, which are outside of the scope of 350.


## Case Study and Practice - Etruscan Skulls
Let's see if the skull sizse of ancient Etruscans were different from those of modern Italians.
```R
vec.skulls = scan("https://mtrosset.pages.iu.edu/StatInfeR/Data/skulls.dat")
etruscan = vec.skulls[1:84] 
italian = vec.skulls[85:154]

boxplot(etruscan)
boxplot(italian)
```
First 84 are the skulls of ancient Etruscan men. The remaining 70 are from ancient iteration are from ancient Italian men. Assume the data is random:. From a box plot, we can see that Etrusian skulls a generall biggerthan ancient skulls: 
  - Experiment unit is a skull.
  - 84 units drawn from Etrusians and 70 for the Italians. So this is a two sample prbolem.
  - One measurement on each skull is the breadth in mm.
  - The parameter of interest here is $\Delta = \mu_{1} - \mu_{2}$, we want to know about the true difference in average breadth:
    
    - $H_{0}: \Delta = 0 \rightarrow \mu_{1} = \mu_{2}$ 
    - $H_{0}: \Delta \neq 0 \rightarrow \mu_{1} \neq \mu_{2}$

  - We can reason that $X_{1}, ..., X_{84} \sim \mathbb{P}$ and they're identical distributions and independent. Herre $E(X_{i}) = \mu_{1}$. Also we know $Y_{1}, ..., Y_{70} \sim \mathbb{P_{2}}$. Boxplots are approx. symmetrical and don't have outliers. As a result, we can assume the distribution of the sample means can we modeled can be denoted as:
    - $\bar{X}_{84} \sim N(\mu_{1}, \frac{\sigma^{2}_{1}}{n_{1}})$ and $\bar{Y}_{70} \sim N(\mu_{2}, \frac{\sigma^{2}_{2}}{n_{2}})$. 
    - $\hat{\Delta}=\bar{X}_{84}-\bar{Y}_{70} \sim N(\Delta, \frac{\sigma^{2}_{1}}{n_{1}} + \frac{\sigma^{2}_{2}}{n_{2}})$.

  6. Since the data is symmetric, sample sizes are large, and also we don't know the variances, we should do Welch's 2 sample t-test.
```R
# Calculate the simple statistics for  
xbar = mean(etruscan)
ybar = mean(italian)
n1 = length(etruscan)
n2 = length(italian)
s1 = sd(etruscan)
s2 = sd(italian)

# Calculate the difference in sample means, standard error
Delta_hat = xbar - ybar
se = sqrt(s1^2/n1 + s2^2/n2)
c(Delta_hat, se) # 11.33, 0.946

# Assuming the null hypothesis, that means we're assuming Delta_0 = 0, believing that there is no 
# significant difference. Now let's calculate T statistic for Welch's two sample test.
t.Welch = (Delta_hat - 0) / se # 11.96

# Calculate the approximate degrees of freedom, used for t-test and t-interval
nuhat = (s1^2/n1 + s2^2/n2)^2/((s1^2/n1)^2/(n1-1) + (s2^2/n2)^2/(n2-1)) # 148.81

# About 0. Of course it's never actually zero, it's so small that R shows 0. We have strong evidence to 
# condlude that there is a difference in the average breadth of Etruscan and Italian skulls. This is 
# doing a two sided test though.
pvalue = 2*(1 - pt(abs(t.Welch), df=nu))

# We can also calculate a 95% confidence interval for 2 samples
alpha = 1-0.95
q = qt(1-alpha/2, df=nuhat-1)
lower = Delta_hat - q*se
upper = Delta_hat + q*se
```