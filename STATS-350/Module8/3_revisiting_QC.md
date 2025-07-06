# Quick Check 3: Revisiting 1-2 Sample Tests

## Question 1
```R
solar <- matrix(c(2.23, 1.82, 2.55, 2.42, 7.99, 8.26, 4.09, 3.46,
                  9.62, 9.77, 1.59, 1.40, 8.98, 8.88, 0.82, 0.87,
                  10.83, 11.20, 1.54, 1.33, 10.75, 10.32, 5.79, 5.87,
                  5.91, 6.44, 5.79, 5.87, 5.50, 5.30, 9.96, 9.82,
                  1.92, 1.69, 7.38, 7.41),
                nrow = 18, ncol = 2, byrow = TRUE)
solar <- as.data.frame(solar)
colnames(solar) <- c("cell1", "cell2")
```

- The same model system was tested under 18 different weather conditions
- Each weather condition was tested with both Cell 1 and Cell 2
- This creates natural pairs for each weather condition, as we have measurements from both cells.

  - Experimental Unit: a certain weather condition

## Question 2

- $\mu = \mu_{DRTA} - \mu_{Basal}$ is the average of the differences in energy storage of cell1 minus cell 2, for all possible weather conditions.


## Question 3

$H_{0}: c1 \leq c2 = \mu \leq 0$


## Question 4

In a solar power system study experiments were carried out on models in a controlled environment to determine how two photovoltaic cells affected the system's energy storage. The controlled environment could simulate various weather conditions. The model system was subjected to 18 different weather conditions with each type of photovoltaic cell, and the energy stored (in watt-hours) was recorded. Cell 2 was significantly more cost-effective than Cell 1. The question of interest is whether cell 1's energy storage was greater than cell 2's

If using the difference of means, do cell1 minus cell2. Use t.test function to perform the test and find the test statistic
```R
solar <- matrix(c(2.23, 1.82, 2.55, 2.42, 7.99, 8.26, 4.09, 3.46,
                  9.62, 9.77, 1.59, 1.40, 8.98, 8.88, 0.82, 0.87,
                  10.83, 11.20, 1.54, 1.33, 10.75, 10.32, 5.79, 5.87,
                  5.91, 6.44, 5.79, 5.87, 5.50, 5.30, 9.96, 9.82,
                  1.92, 1.69, 7.38, 7.41),
                nrow = 18, ncol = 2, byrow = TRUE)
solar <- as.data.frame(solar)

# 1 sample paired test
t.test(x=solar$V1, solar$V2, paired=TRUE, alternative="greater")

```
t = 0.901

## Question 5
Fail to reject H0:

p=0.189

## Question 6
We only fail to reject the null hypothesis. It doesn't prove any conclusion though.


## Question 7
Is DRTA teaching method better than the traditional method of Basal? We're assuming that the data is random and that the sample of students using DRTA is independent of those using Basal?

- $\delta = Drta - Basal$

- Hypotheses:
  - Not $H_{0}: \delta \leq 0$
  - $H_{0}: \delta \geq 0$


We're comparing the mean improvement between the two groups. I mean it's not paired as there's no natural pairing between students in the basal and the DRTA group. Not multiple measurements, jsut one per student (post-pretest) to measure improvement.


## Question 8
```R
data.Basal = subset(carData::Baumann, subset=(group=="Basal"))
data.DRTA = subset(carData::Baumann, subset=(group=="DRTA"))


# Calculate improvement for each group (post.test.1 - pretest.1)
improvement.Basal <- data.Basal$post.test.1 - data.Basal$pretest.1
improvement.DRTA <- data.DRTA$post.test.1 - data.DRTA$pretest.1

# 2 Sample T-Test 
# H0: mean improvement DRTA <= mean improvement Basal
# H1: mean improvement DRTA > mean improvement Basasl

test_results = t.test(
  x=improvement.DRTA, 
  y=improvement.Basal, 
  alternative="greater",
  )

test_results

```

## Question 9
Near 0

## Question 10
```R
test_results = t.test(
  x=improvement.DRTA, 
  y=improvement.Basal, 
  alternative="two.sided",
  conf.level=0.99
  )

test_results

lb = 1.78 
```