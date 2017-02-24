#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "wythoff.h"

int is_direction_loosing(state_t **s, enum direction d) {
    return s && !s[d];
}

int is_direction_notloosing(state_t **s, enum direction d) {
    return s && s[d];
}

int is_loosing(state_t **s) {
    return is_direction_loosing(s, R)
        && is_direction_loosing(s, B)
        && is_direction_loosing(s, D);
}

state_t** new_game_state() {
    return calloc(3, sizeof(state_t*));
}

state_t* new_state(const coin_t red, const coin_t black) {
    state_t stmp = { .red = red, .black = black };
    state_t *s = malloc(sizeof(state_t));
    if (s == NULL) {
        abort();
    }
    memcpy(s, &stmp, sizeof(state_t));
    return s;
}

state_t* dup_state(const state_t *stmp) {
    state_t *s = malloc(sizeof(state_t));
    if (s == NULL) {
        abort();
    }
    memcpy(s, stmp, sizeof(state_t));
    return s;
}

state_t **compute_wythoff (state_t ****table, coin_t red, coin_t black) {
    if (table[red][black]) {
        return table[red][black];
    }
    table[red][black] = new_game_state(); 
    state_t **red_state = (red > 0) ? compute_wythoff(table, red - 1, black) : NULL;
    state_t **black_state = (black > 0) ? compute_wythoff(table, red, black - 1) : NULL;
    state_t **both_state = 
        (red > 0 && black > 0) ? compute_wythoff(table, red - 1, black - 1) : NULL;
    if (is_loosing(red_state))
        table[red][black][R] = new_state(red - 1, black);  
    if (is_loosing(black_state))
        table[red][black][B] = new_state(red, black - 1);  
    if (is_loosing(both_state))
        table[red][black][D] = new_state(red - 1, black - 1);  
    if (is_direction_notloosing(red_state, R))
        table[red][black][R] = dup_state(red_state[R]);
    if (is_direction_notloosing(black_state, B))
        table[red][black][B] = dup_state(black_state[B]);
    if (is_direction_notloosing(both_state, D))
        table[red][black][D] = dup_state(both_state[D]);
    return table[red][black];
}
