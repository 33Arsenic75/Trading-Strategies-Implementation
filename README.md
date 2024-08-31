### COP290 ASSIGNMENT1 PART-C

This assignment aims to implement common trading strategies under contraints and assumptions.

The following trading strategies have been implemented :
## Optimizations

### 1. Basic Strategy, DMA Strategy:

- **Algorithmic Efficiency:** 
### Optimized Approach - O(n + T)
Efficient algorithm with time complexity O(n + T), using Sliding Window concept, where `n` is days for monotonicity, and `T` is total days.
### Brute Force Approach - O(T * n)
Less efficient algorithm with time complexity O(T * n), checking monotonicity for each day in total time period.

### 2. DMA++ Strategy:

- **Algorithmic Efficiency:** 
Same as above, also used queues to Buy/Sell the the stocks while implementing stop loss.

### 3. Mean-Reverting_StopLoss Strategy:

- **Algorithmic Efficiency:** 
Used heap to Buy/Sell stocks based on spread while implementing range based stop loss.

## Intuitions Developed for Stock Market Trading Strategies

### 1. Efficient Use of Historical Data
Strategies should focus on extracting meaningful insights from the past to anticipate potential future market movements.
### 2. Adaptive and Dynamic Thresholds
The intuition is that market conditions are dynamic and constantly changing. Strategies should dynamically adjust thresholds based on current volatility or other indicators to effectively adapt to varying market environments, enhancing the strategy's robustness.
### 3. Risk Management as a Priority
Intuitively, effective trading strategies prioritize risk management. This involves optimizing position sizing, setting stop-loss levels, and incorporating risk controls to protect against significant losses.

## Insights
Submitted in Bonus Part.