# Revisiting Sample Location Problems


## `t.test()` function

R's `t.test()` function lets you do 1 or 2 sample t-tests:
```R
t.test(
  # Corresponds to the first sample needed for the test, if it's the only
  # sample then this function performs a 1-sample test
  x,

  # Corresponds to the second sample for the test. If both samples are 
  # provided, it does a two-sample test delta = mu_x - mu_y
  y=NULL,

  # The inequality in the alternative hypothesis.
  alternative = c("two.sided", "less", "greater"),

  # The level for a confidence. If you're doing a confidence interval
  # then make sure to make alternative a two.sided test since confidence intervals are always two sided.
  conf.level=0.95
)
```

## Revisiting Seedling Problem
Pairs of seedlings of the same age, one produced through cross-fertilization and the other by self-fertilization, grow together so that each ones were raised under nearly identical situations. The idea is to show that cross-fertilization was better. Let's list some knowledge:

- Experimental unit: A pair of seedlinsg of the smae age. THe measurements taken on each unit are the final height of the cross-fertilized and self-fertilized plant. It's a one-sample problem.
- $X_{i}$ be the difference in heights
- Hypotheses:
  - $H_{0}: \mu \leq 0$
  - $H_{1}: \mu > 0$

```R
seedlings = read.table("https://mtrosset.pages.iu.edu/StatInfeR/Data/seedlings.dat")
diff = seedlings[, 1] - seedlings[, 2]

# T test; calculates p value too so that's good
t.test(x=diff, alternative="greater")


# 90% confience interval 
t.test(x=diff, conf.level=0.90)
```

## Reviewing "Etruscan Skulls" Example
Were the skull sizes of ancient Etruscans different from the skull sizes of modern Italians? We used the 2 sample Welch's T-test. Remember Welch's t-test was specifically for 2 samples. Let's solve it using the `t.test()` function.

1. Each experiment unit is a skull
2. The skulls are sampled from ancient Etruscans (population 1) and ancient italians (population 2).
    - 84 Etruscans and 70 Italians
    - A two sample problem
3. One measurement is taken on each skull, the breadth in milimeters. Let $\delta = \mu_{1} - \mu_{2}$
4. Hypotheses:
  - $H_{0}: \delta = 0$; skulls are equal
  - $H_{1}: \delta \neq 0$; skulls aren't equal
5. Data seems somewhat symmetric for both samples and sample sizes are reasonably size.

```R
vec.skulls = scan("https://mtrosset.pages.iu.edu/StatInfeR/Data/skulls.dat")
etruscan = vec.skulls[1:84]
italian = vec.skulls[85:154]


# confidence level 95% for an interval as it'll calculate both
# for the t-test and t-interval.

t.test(x=etruscan, y=italian, alternative="two.sided", conf.level=0.95)
```
