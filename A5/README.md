# Assignment 5 Value Iteration

The computer chooses its move using value iteration. The state `s` is the running sum before the current player's turn; each action adds 1 or 2. The supplied code treats a sum of 20 or greater as a win, and this implementation follows that rule.

`V[s]` describes the outcome for the player whose turn it is: +1 means a forced win against an optimal opponent, and -1 means a loss against an optimal opponent. Values start at zero.

For each iteration, both actions are evaluated using

$$
q(s,a)=\begin{cases}
1, & s+a\geq20,\\
-V(s+a), & s+a<20.
\end{cases}
\qquad V_{\mathrm{new}}(s)=\max_{a\in\{1,2\}}q(s,a).
$$

After a nonterminal move, it is the opponent's turn, so the next state's value changes sign. This is the zero-sum, two-player form of the Bellman update. The discount factor is 1: every move increases the sum, so every game ends after finitely many moves.

`value_iteration()` uses separate old and new arrays for synchronous updates and stops when no state value changes. It then stores the best action in `policy[s]`. Ties select action 1. `computer()` returns `policy[score]`; it does not use a modulo rule. The supplied `computer2()` remains available for comparison and is not called by the AI.

Build and run from the A5 directory:

```sh
clang -std=c11 -Wall -Wextra -Wpedantic -O2 game.c -o /tmp/4ax3_a5_game
/tmp/4ax3_a5_game
```

Enter 1 to move first or 2 to let the computer move first. On each human turn, enter 1 or 2.

The computed losing states are 2, 5, 8, 11, 14, and 17. A computer moving first at state 0 chooses 2 and can force a win against either human action on every subsequent turn. When the human moves first and plays optimally, the computer cannot guarantee a win.
