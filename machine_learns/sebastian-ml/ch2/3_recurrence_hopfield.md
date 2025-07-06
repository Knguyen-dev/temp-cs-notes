# Hopfields Networks


### 1. Intro to Hopfields and recurrent neural networks 
A neuron takes the weighted summed (signals) from other neurons to determine its own state. It then broadcasts its own state to any neurons it's connected to, so it sends back a signal to the senders. Then you apply some function to that weighted sum, and that's the state of your neuron.

$v = f(w^{T}x)$

That's one neuron in the network, and it gets interesting when you have more neurons, and that creates the network. This is called a recurrent neural network, meaning for any neurons i and j, there will be an edge $w_{ij}$ and $w_{ji}$. So there's a directed connection to and from a given neuron to any other neuron. Relating this to memory, it's the idea that we have a system S.T., after being in some configuration for a certain amount of time, if you change the configuration, it's able to slowly go back to its original configuration. An analogy is a plastic bottle. If crushed, its configuration changes, it may be able to slowly grow back to its original state. So its initial state is memorized. So if we have a neural network that can remember some original/preferred state, and will go back to that state if it is disturbed.

- **time:** The idea of measuring and keeping track of the network's memory over a discrete amount of time. So $t \rightarrow t+1$.
- **learning:** We need the network to learn a preferred state, which is just a matter or configurating states and weights.
- **convergence:** As $t \rightarrow \infty$, the network converges to the **memory state**, what we'll refer to as the original/preferred config.

#### 2. Learning over time
Let the activity/state of a neuron be 1 (active) or -1 (inactive). So the activity changes over time, so the acitvity of a neuron is a function of time, e.g. $v_{3}(t)=-1$, where $v_{3}$ is the 3rd neuron in our network and its inactive at some time-step. Then the state of the network considers the state of all neurons. So When we do $t \rightarrow t+1$, we randomly select a neuron to update, feeding that neuron the weighted sum of the activations from the other neurons. So we just change on neuron and keep the rest the same for this new timestep. Then you just keep increasing the time. Let's see this:

  1. Pick a random node $v_{s}$.
  1. $v_{s}(t) = f(\sum_{i=1}^{16}{w_{i} \cdot v_{i}}(t-1))$; 
  2. Get the weighted sum of the states of the other nodes. Then apply an activation function. THe state of $v_{s}$ is +1 if the weighted sum $x \geq 0$, else -1. Or something like that, you're threshold will probably vary for other situations.

However once it converges to the memory state, things become stable and it doesn't change anymore even if you continue increasing the time. Of course this has some limitations if you want your network to have multiple memory states. But for the simple case, it either converges to the memory stage, anti-memory state, or an inactive state. So in summary, a recurrent neural network is just a dynamically changing vector. The network converges towards a preferred configuration (memory state).

#### 3. How to set the preferred state
Given a network of 3 neurons, denoted by $v \in \mathbb{R}^{3}$, we're looking for a matrix $M_{3x3}$ that contains the weights of the network. You'd do the outer product of vector v with itself. This is also denoted as $w_{ij} = \frac{1}{n}\sum_{k=1}^{N}{q_{i}^{k}q_{j}^{k}}$.Now each element in the matrix is $w_{ij} = v_{i}v_{j}$. You'd set the diagonals to zero to avoid self reinforcement, so $w_{ii} = 0$. This into the idea of Hebbian learning.

- if neurons have the same signed state (both positive or both negative), then the weight between them is positive. 
- If they have differing states, then the weight between them is negative.


### 2: Hopfields in detail:

#### Hebb Rule:
The idea of strengthening a synapse according to how many connections it has. If there's a connection $w_{ij}$ and signals are sent from j to i, which results in boht neurons being simultaneously active, then the weight $w_{ij}$ is reinforced. A formula for weight change could be:

$\nabla{w_{ij}} = nx_{i}x_{j}$, with the constant $n$ (learning rate), which determines the how much the weights changes.

#### Equations and Algorithm:
We're trying to remember/go back to a N pixeled image, that's black and white. We'll represent it as a vector $\vec{q} \in \mathbb{R}^{N}$, where $q_{i} \in {1, -1}$.

We can learn the pattern by calculating all weights with the formula:

- **Weight rule:** $w_{ij} = \frac{1}{n}\sum_{k=1}^{N}{q_{i}^{k}q_{j}^{k}}$

  - If the neurons have the same activation value, then the weight is positive.
  - If they have differing activating values, it's a negative weight.
- **Update Rule:** 
    - $x_{i} = -1$, when $\sum_{j \neq i}^{N}{w_{ij}x_{j}} < 0$
    - Else $x_{i}=1$
- **HopfieldAssoc(q):**
  - Initialize all neurons x = q
  - Repeat until x converges:
    - i = Random(1,n)
    - Update the state of neuron i according to the rule above.
  - Then return x.

If the weight matrix W is symmetric, this process converges to a memory state. The weight matrix is typically symmetrical though. Here's a function called the energy function: $E = -\frac{1}{2}sum_{i,j}{w_{ij}x_{i}x_{j}}$. As long as w is symmetric:
  - The process will converge onto some final state.
  - The state that converges wwill minimize the energy.


## Credits:
- [Hopfield Network Introduction](https://www.youtube.com/watch?v=piF6D6CQxUw)