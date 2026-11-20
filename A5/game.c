/* Who says 20 first - value iteration */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int score;
char in[20];
int V[20];
int policy[20];

void value_iteration(void)
{
    int next[20];
    int changed;

    /* V[s] is the outcome for the player whose turn it is. */
    for (int s = 0; s < 20; ++s)
        V[s] = 0;

    do {
        changed = 0;
        for (int s = 0; s < 20; ++s) {
            int best = -2;
            for (int a = 1; a <= 2; ++a) {
                int q = (s + a >= 20) ? 1 : -V[s + a];
                if (q > best)
                    best = q;
            }
            next[s] = best;
            if (next[s] != V[s])
                changed = 1;
        }
        for (int s = 0; s < 20; ++s)
            V[s] = next[s];
    } while (changed);

    /* Choose an action from the converged values. */
    for (int s = 0; s < 20; ++s) {
        int best = -2;
        for (int a = 1; a <= 2; ++a) {
            int q = (s + a >= 20) ? 1 : -V[s + a];
            if (q > best) {
                best = q;
                policy[s] = a;
            }
        }
    }
}

int user(void)
{
    int i = 0;
    while (i != 1 && i != 2) {
        printf("We are at %-2d, add 1 or 2 ? ", score);
        if (!fgets(in, sizeof in, stdin))
            exit(0);
        i = atoi(in);
    }
    return i;
}

int computer(void)
{
    int i = policy[score];
    printf("We are at %-2d Computer adds %d\n", score, i);
    return i;
}

/* The supplied opponent is retained for comparison. */
int computer2(void)
{
    if (score % 3 == 1)
        return 1;
    if (score % 3 == 0)
        return 2;
    return rand() % 2 + 1;
}

int main(void)
{
    srand((unsigned)time(NULL));
    value_iteration();

    int i = 0;
    score = 0;
    printf("Who says 20 first\n\n");
    while (i != 1 && i != 2) {
        printf("Who goes first: you=1 computer=2 ? ");
        if (!fgets(in, sizeof in, stdin))
            return 0;
        i = atoi(in);
    }
    if (i == 2)
        score = computer();

    while (score < 20) {
        score += user();
        if (score >= 20) {
            printf("YOU WIN !!\n");
            break;
        }
        score += computer();
        if (score >= 20) {
            printf("I WIN !!\n");
            break;
        }
    }
    return 0;
}
