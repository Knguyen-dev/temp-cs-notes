# Quick Check 1: Correlation

## Question 1
1. Left Plot: $\rho=-1$
2. Middle Plot: $\rho=0$
3. Right Plot: $\rho=0.8$

## Question 2
Seems like a very weak positive association.

## Question 3
Using the data frame from `alr4::BGSgirls`. Look at `WT18` and `HT18`. Using R, standardize the values of weight and height, and report the first standardized value in the dataset.

1. Remember that when they say "standardize", they mean "normalize" make the data values fit on a standard normal distribution. In essence, apply the function $z = \frac{x - \mu}{\sigma}$. Since you don't have the data for the entire population for all 18 year old chlidren, you'll probably just use the $\bar{x}$ and $s_{n}$. Now the formula is $z = \frac{x-\bar{x}}{s}$.
2. Let's look at the R Code:

```R
install.packages("alr4")
data1 = alr4::BGSgirls

standardize = function(data) {
  xbar = mean(data, na.rm=TRUE)
  sd = sd(data, na.rm=TRUE)
  standardized_data = (data - xbar) / sd
  return (standardized_data)
}

wt18_z = standardize(data1$WT18)
ht18_z = standardize(data1$HT18)

c(wt18_z[1], ht18_z[1]) # -0.332 and -1.258
```
- First weight is $4.246$. Why is this wrong? **It's wrong since you used BGSall instead of BGSgirls**.
- First height is $0.726$

## Question 4
Show the estimated contour using `binorm.scatter()`

```R
# Left graph, you can tell by the axes; so I guess don't normalize?
weight = data1$WT18
height = data1$HT18
binorm.scatter(cbind(weight, height))
```

## Question 5 and 6
Calculate the pearson's correlation coefficient for ages 2, 9 and 18 for weight and height. Then order the associations from strongest to weakest.

```R
calculate_pearson = function(x, y) {
  xbar = mean(x)
  ybar = mean(y)
  s_x = sd(x)
  s_y = sd(y)
  n = length(x)
  z_x = (x - xbar) / s_x
  z_y = (y - ybar) / s_y 
  r = sum(z_x * z_y) / (n-1)
  return (r)
}
bgs_data_set = alr4::BGSgirls
wt_2 = bgs_data_set$WT2
ht_2 = bgs_data_set$HT2
r_2 = calculate_pearson(wt_2, ht_2)
wt_9 = bgs_data_set$WT9
ht_9 = bgs_data_set$HT9
r_9 = calculate_pearson(wt_9, ht_9)
wt_18 = bgs_data_set$WT18
ht_18 = bgs_data_set$HT18
r_18 = calculate_pearson(wt_18, ht_18)
c(r_2, r_9, r_18) # 0.644, 0.727, and 0.497

c(r_2 - r_18) # 0.146
```
- From strongest to weakest: 9, 2, 18
- Now calculate the difference $r_{2} - r_{18} = 0.146$
