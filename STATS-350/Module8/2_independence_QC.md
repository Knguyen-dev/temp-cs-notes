# Quick Check 2: Independence

## Question 1
A study of a disease classified $n=538$ patients by "histological type" and response to treatment:
- Histological Type: LP, NS, MC, LD
- Response to Treatment: positive, partial None.

This creates a 4 by 3 contingency table:

```R
obs <- matrix(c(74, 68, 154, 18, 
                    18, 16, 54, 10, 
                    12, 12, 58, 44),
              nrow = 4, ncol = 3)
rownames(obs) <- c("LP", "NS", "MC", "LD")
colnames(obs) <- c("Positive", "Partial", "None")

#       Positive Partial None
# LP          74      18   12
# NS          68      16   12
# MC         154      54   58
# LD          18      10   44

# Let's just use R to get this over with

exp = rowSums(obs)%o%colSums(obs)/sum(obs)


# Print out a table with the expected values for those cells.
#     Positive  Partial     None
# LP  60.69888 18.94424 24.35688
# NS  56.02974 17.48699 22.48327
# MC 155.24907 48.45353 62.29740
# LD  42.02230 13.11524 16.86245
```
155.24

## Question 2
```R
# Calculating the Test Statistic
G2 = 2*sum(obs*log(obs/exp)) # 68.295
```

## Question 3
Calculating the degrees of freedom:
- $r = 4, c=3$
- $df = (r-1)(c-1) = 3(2) = 6$ 

## Question 4
Calculating the p value. Again you're trying to find the area to the right of the test statistic for the Chi-Squared Distribution:
```R
p = 1-pchisq(G2, df=6) # 9.129e-13, basically 0
```

## Question 5
You're going to reject $H_{0}$ and accept $H_{1}$. We can conclude that histological type and repsonse to treatment are related.