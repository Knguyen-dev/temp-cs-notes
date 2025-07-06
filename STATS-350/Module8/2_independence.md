# Chi-Squared Test for Independence

A test for independence/association between two variables.

Let $S$ be the sample space of our experiment:
- $A_{1}, ..., A_{r}$ partition S into $r$ cells
- $B_{1}, ..., B_{c}$ partition S into $c$ cells.
- Think of A and B as two variables with different categories.

Two partitions of criminals, one by type of crime (arson, violence, stealing), adn the other by alcohol consumption (drinker, abstrainer).
|                  | Drink   | Abstain | Row Total |
| ---------------- | ------- | ------- | --------- |
| Arson            | 50      | 43      | 93        |
| Rape             | 88      | 62      | 150       |
| Violence         | 155     | 110     | 265       |
| Stealing         | 379     | 300     | 679       |
| Coining          | 18      | 14      | 32        |
| Fraud            | 63      | 144     | 207       |
| **Column Total** | **753** | **673** | **1426**  |

The grand total $t=1426$. Are these variables influencing each other? We care about the relationship betwee As and Bs. So define a third partition:

- $E_{ij} = A_{i} \cap B_{j} = \frac{(Row Total of i) \cdot (Column total of J)}{t}$
- The partitions $A_{i}, ..., A_{r}$ and $B_{1}, ..., B_{c}$ are mutually independent IFF $P(E_{ij}) = P(A_{i}) \cdot P(B_{j})$ for each $ij$ pair.
- We'll use chi-squared methods developed to check for independence.

Take the Arson-Drink cell:
- Row total: 93
- Column total: 753
- $E_{ij} = \frac{93 \cdot 753}{1426} = 49.12$

```R
# Does that math for every cell
exp = rowSums(observed)%o%colSums(observed)/sum(observed)

#            drink abstain
# arson     49.109  43.8912
# rape      79.208  70.792
# violence 139.933 125.067
# stealing 358.546 320.454
# coining   16.898  15.102
# fraud    109.306  97.694

# Same idea for the the statistic 
G2 = 2*sum(obs*log(obs/exp))

# The number of rows r = 6
# The number of columns c = 2
# df = (r-1)(c-1) = (6-1)(2-1) = 5


# unrestricted set: rc-1
# restricted set: (r-1) + (c-1)
# unrestricted - restricted = (r-1)(c-1)
```
In our example:
- $H_{0}$ variables are independent.
- $H_{1}$ variables are related/dependent.