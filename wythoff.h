#ifndef __WYTHOFF_H__
#define __WYTHOFF_H__

#include <stdint.h>

enum direction {R, B, D};

struct wythoff_state {
    const uint32_t red;
    const uint32_t black;
};

typedef struct wythoff_state state_t;
typedef uint32_t coin_t;

//state_t* new_state(const coin_t red, const coin_t black);
//state_t* dup_state(const state_t *stmp);
state_t **compute_wythoff (state_t ****table, coin_t red, coin_t black);
int is_loosing(state_t **s);
int is_direction_loosing(state_t **s, enum direction d);
int is_direction_notloosing(state_t **s, enum direction d);

#endif

