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

cfbf_stack *cfbf_create_stack(void);
bool cfbf_stack_push(cfbf_stack *stack, uint32_t element);
bool cfbf_stack_pop(cfbf_stack *stack, uint32_t *dest);
bool cfbf_stack_peek(const cfbf_stack *stack, uint32_t *dest);
bool cfbf_stack_is_empty(const cfbf_stack *stack);
void cfbf_destroy_stack(cfbf_stack *stack);
