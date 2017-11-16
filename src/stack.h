#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct cfbf_stack_item {
        uint32_t value;
        struct cfbf_stack_item *prev;
} cfbf_stack_item;

typedef struct cfbf_stack {
        cfbf_stack_item *head;
        cfbf_stack_item *tail;
        int32_t length;
} cfbf_stack;

extern cfbf_stack *cfbf_create_stack(void);
extern bool cfbf_stack_push(cfbf_stack *stack, uint32_t element);
extern bool cfbf_stack_pop(cfbf_stack *stack, uint32_t *dest);
extern bool cfbf_stack_peek(cfbf_stack *stack, uint32_t *dest);
extern bool cfbf_stack_is_empty(cfbf_stack *stack);
extern void cfbf_destroy_stack(cfbf_stack *stack);