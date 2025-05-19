
# H2SCalculator
The H2SCalculatorOnline component allows users to run experiments to calculate H₂S solubility under various conditions. It renders a chart of the results and provides downloadable CSV files for each experiment.

### Key Features
- Users can select a system, temperature, and NaCl concentration.
- Results are plotted on a scatter chart with customizable x and y axes.
- Multiple experiments can be run and visualized simultaneously.
- Results of each experiment can be downloaded as separate CSV files.

### Design Decisions
- **Important State:** 
  - The `graphHistory` state stores data for all experiments, allowing users to view multiple results simultaneously. Each element in that array is a separate simulation that they ran.
  - The `downloadData` is an array containing CSV data (as strings) for each experiment. You can view `graphHistory` and `downloadData` as parallel lists in that sense because `graphHistory[i]` is the ith experiment we're going to render, and downloadData[i] is the csv data for that ith experiment that the user can download.


- **Backend Limitations and Considerations:**
 - When running an experiment the backend will typically provide a matrix with 3 rows. On the client-side, the user will be able to select any axes to render the data. However, for a simulation with system `CO₂-H₂S-H₂O-NaCl`, the backend will provide a matrix with only one matrix. As a result, if you try to graph the data for this experiment using  "ρ, kg/m3" or "λH2S", it won't work since the backend doesn't provide data for those configurations. In the old system, this would break the system. But in the new system, we'll just skip the rendering of these experiments when these configs are applied. A disclaimer is displayed to the user for this exact situation.
 - Besides taking this into consideration when graphing the data, we take this into consideration when preparing the CSV data too.


- We considered doing the H2S Calculations on the client side. Due to time constraints and also difficulty, I decided not to. The Scipy griddata would be quite difficult to implement from scratch from a surface view of it.

