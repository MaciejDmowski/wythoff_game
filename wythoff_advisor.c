#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>
#include "wythoff.h"

char *pretty_message(state_t **a, uint32_t size, coin_t r, coin_t b) {
    const int msg_size = 600;
    char message[msg_size];
    if (r >= size || b >= size) {
        const char *dunno  = "NOT ENOUGH MEMORY (%"PRIu32", %"PRIu32") --> ?\n\n\n";
        sprintf(message, dunno, r, b);
    } else if (is_loosing(a)) {
        const char *lose  = "LOOSING (%"PRIu32", %"PRIu32") --> ?\n\n\n";
        sprintf(message, lose, r, b);
    } else {
        const char *fmsg = "%s%s%s\n\n";
        const char *fmove = "WINNING (%"PRIu32",%"PRIu32") --> (%"PRIu32",%"PRIu32")\n";
        char rmove[msg_size];
        char bmove[msg_size];
        char dmove[msg_size];
        rmove[0] = bmove[0] = dmove[0] = 0;
        if (!is_direction_loosing(a, R))
            sprintf(rmove, fmove, r, b, a[R]->red, a[R]->black);
        if (!is_direction_loosing(a, B))
            sprintf(bmove, fmove, r, b, a[B]->red, a[B]->black);
        if (!is_direction_loosing(a, D))
            sprintf(dmove, fmove, r, b, a[D]->red, a[D]->black);
        sprintf(message, fmsg, rmove, bmove, dmove);
    }
    return strndup(message, msg_size);
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s mem\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    /* We expect wythoff program to allocate O(argv[1]^2)
     * bytes memory. If we ask about state that is beyond
     * our knowledge we simply say that we don't know;
     */
    /*
     * We decide to use sscanf to convert char* into coin_t
     * a.k.a uint32_t;
     */
    coin_t arg;
    sscanf(argv[1], "%"SCNu32, &arg);
    /*
     * We create const coin_t variable to store size of
     * problem we solve. Use this variable from now to
     * dentote size of given problem;
     */
    const coin_t size = arg;
    /* We allocate size x size matrix to store answers
     * to wythoff's game problem. We initialize our
     * ans table to null pointers to denote we have not
     * figure out problem yet;
     */
    state_t ****ans = malloc(size * sizeof(state_t ***));
    for (uint32_t i = 0; i < size; ++i) {
        ans[i] = calloc(size, sizeof(state_t **));
    }
    /* We use r, and b variable to store query parameters
     */
    coin_t r;
    coin_t b;
    /* main loop we try to answer querries */
    while (scanf("%"SCNu32" %"SCNu32, &r, &b) == 2) {
        state_t **a = NULL;
        if (r < size && b < size)
            a = compute_wythoff(ans, r, b);
        char *msg = pretty_message(a, size, r, b);
        printf(msg);
        free(msg);
    }
    for (uint32_t i = 0; i < size; ++i) {
        for (uint32_t j = 0; j < size; ++j) {
            if (ans[i][j]){
                free(ans[i][j][R]);
                free(ans[i][j][B]);
                free(ans[i][j][D]);
                free(ans[i][j]);
            }
        }
        free(ans[i]);
    }
    free(ans);
    return EXIT_SUCCESS;
}
