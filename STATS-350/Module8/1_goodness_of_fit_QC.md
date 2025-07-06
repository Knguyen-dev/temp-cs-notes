# Quick Check 1: Goodness of Fit 

## Question 1
Assume that the Mars company claims that it mixes their M&M candies in the following proportions:
- (Brown= 0.13), (Yellow= 0.14), (Red = 0.13), (Blue: 0.24), (Orange =  0.20), and (Green= 0.16)
- We got sample data:
  - Brown: 121
  - Yellow: 84
  - Red: 118
  - Blue: 226
  - Orange: 226
  - Green: 123

Get the expected cell count for blue M&Ms:
```R
# Claimed population probabilites and sample data
p <- c(0.13, 0.14, 0.13, 0.24, 0.20, 0.16)
observed_counts <- c(121, 84, 118, 226, 226, 123)

# 1. Assuming the null hypothesis, they already gave us the expected probabilities. Using 
# these probabilities we can get the expected cell counts by multiplying each by the 
# total number of samples 
n = sum(observed_counts)
expected_counts = p*n
expected_counts # 116.74 125.72 116.74 215.52 179.60 143.68

expected_counts[4] # 215.52 is the expected count for blue m&ms
```

## Question 2
Calculate the likelihood ratio test

```R
G2 = sum(2 * observed_counts*log(observed_counts/expected_counts)) 
```


## Question 3
Calculate the degrees of freedom?

Degrees of freedom are calculated as the difference in the dimensions of the unrestricted and restricted sets. You can see it as the number of categories minus 1 (6-1=5). However other times you'll hear about restricted vs unrestricted sets.
- Unrestricted model: Allows each color to have any proportion, each category's probability ss free to vary
- Restricted model: Assumes proportions are fixed according to the company's claims.

In an unrestricted setting you estimate 6 probabilities, but they must sum up to 1, so you only have 5 free parameters, or probabilities to choose from. Since that last one is defaulted to sum up to one. Now under $H_{0}$, these proportions are fully specified, they tell us what the probabilities are. So there are 0 free parameters.

## Question 4
```R

# Again we're trying to find the probabillity to the right of the test stat
1 - pchisq(G2, 5) # p = 1.14e-05, basically 0
```

## Question 5
Assuming $\alpha=0.01$, since $p < \alpha$, we can reject $H_{0}$ and accept $H_{1}$. This shows that there's significant statistical evidence to conclude that the claimed proportions by the claim are not credible.

## Question 6
A screen has resolution $324 \times 480$ pixels. We claim each pixel will be colored black with probability $p=0.29$ and white with $1-p=0.71$. To validate the probabilities, we select $n=1000$ $4 \times 4$ squares of pixels and count the number of blac kpixels in each square. Let $X_{i}$ denote the number of black pixels in the ith square.

Ideally, $X_{i} \sim Binomial(16, 0.29)$. Then $P(X_{i} = j), j \in X(S) = \{0, ..., 16\}$, which basically is the probability of some $i$-th square containing $j$ black pixels. Finally, we'll partition the range:
- $E_{1} = \{ 0,1 \}$; event that merges outcomes for 0 and 1 black pixels
- $E_{j} = \{ j \}, j=2,...,8$
- $E_{9} = \{ 9, ..., 16\}$; event that merges outcomes for >= 9 black pixels

1. $H_{0}: X_{i} \sim Binomial(16,0.29)$. The number of black pixels in each 4 by 4 square follows a binomial distribution that we've talked about above.

## Question 7
Calculate the expected count for cell $E_{1}$. 

1. So we're calculating the expected number of 4 by 4s that have either 0 or 1 black pixels?
2. First we should calculate the $P(X_{i}=0) + P(X_{i} = 1)$. Probability of a given 4 by 4 landing in $E_{1}$. Then multiply that by our total sample size


```R
cells <- 1:9
observed_counts <- c(30,93,159, 184, 195, 171, 92, 45, 31)
n = sum(observed_counts)

# P(E1) = P(X=0) + P(X=1); Probability that a square has 1 or 1 black pixels
pe1 = dbinom(x=0, size=16, prob=0.29) + dbinom(x=1, size=16, prob=0.29)

expected_count = n * pe1 # 31.421, on average 31.421 squares will be in this range
```

## Question 8
Calculate the likelihood ratio test statistic


```R
cells <- 1:9
observed_counts <- c(30,93,159, 184, 195, 171, 92, 45, 31)
n = sum(observed_counts)

# Calculate the probability of each possible outcome for X_{i} (0 to 16 black pixels)
prob_outcomes = dbinom(0:16, size=16, prob=0.29)

# Calculate the probability for each cell
cell_probs = numeric(9)

# P(E1) = P(X=0) + P(X=1)
cell_probs[1] = dbinom(x=0, size=16, prob=0.29) + dbinom(x=1, size=16, prob=0.29)

# Cells E2 to E8: individual outcomes {2}, {3}, ..., {8}
for (j in 2:8) {
  # P(X=j), R indexing starts at 1. So prob_outcomes[1] is P(X=1), prob_outcomes[2] is P(X=2)
  # Then we can reason that prob_outcomes[j+1] is P(X=j)
  cell_probs[j] = prob_outcomes[j+1]
}

# Cell E9: outcomes {9, 10, 11, 12, 13, 14, 15, 16}
cell_probs[9] <- sum(prob_outcomes[10:17])  # P(X=9) + ... + P(X=16)

# Calculate expected counts and finally you should have enough for 
expected_counts = n * cell_probs
G2 = sum(2 * observed_counts*log(observed_counts/expected_counts)) # 11.96
```

## Question 9
Note that we have 9 cells. Calculate the degrees of freedom?


1. df = (number of cells) - 1 - (number of estimated parameters)
2. There are 9 cells. There are 0 estimated parameters because under the null hypothesis, we're given the probability. So estimated parameters needed.
3. df = 9-1-0 = 8

## Question 10
Calculate the p value?


```R
1 - pchisq(q=G2, df=8) # 0.152
```


## Question 11
Let $\alpha = 0.07$, since $p > \alpha$, we fail to reject $H_{0}$. We don't have enough evidence to conclude that the algorithm doesn't perform as intended

## Question 12
Assume that we change it so that we aren't given the probability. However we do still estimate $\hat{p} = 0.29$.

The only thing that would change about our problem is that the degrees of freedom.
1. df = num_cells - 1 - (num_estimated_parameters) = 9 - 1 - 0 = 8
2. However now in our new problem setup, we did have to estimate a parameter $\hat{p}$. So now the equation is df = 9 - 1 - 1 = 7
3. The dimension of the restricted set changes from 8 to 7. THe null hypothesis space has one fewer dimension as a parameter went from being given from claim, to having to be estimated.

