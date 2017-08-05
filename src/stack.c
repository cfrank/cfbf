#include <stdio.h>
#include <string.h>
#include "stack.h"

cfbf_stack *cfbf_create_stack(void)
{
        cfbf_stack *stack = malloc(sizeof(cfbf_stack));

        if (stack == NULL) {
                fprintf(stderr, "There was an error allocating %lu bytes for the"
                                " stack",
                        sizeof(cfbf_stack));
                return NULL;
        }

        stack->head = stack->tail = NULL;
        stack->length = 0;

        return stack;
}

bool cfbf_stack_push(cfbf_stack *stack, uint32_t element)
{
        cfbf_stack_item *item = malloc(sizeof(cfbf_stack_item));

        if (item == NULL) {
                fprintf(stderr, "Could not allocate %lu bytes for a stack item",
                        sizeof(cfbf_stack_item));
                return false;
        }

        item->value = element;
        item->prev = stack->head;
        stack->head = item;

        // If this is the first element the head and tail are the same
        if (stack->tail == NULL) {
                stack->tail = stack->head;
        }

        ++stack->length;

        return true;
}

bool cfbf_stack_pop(cfbf_stack *stack, uint32_t *dest)
{
        if (cfbf_stack_is_empty(stack)) {
                return false;
        }

        cfbf_stack_item *stack_item = stack->head;
        memcpy(dest, &stack_item->value, sizeof(uint32_t));

        stack->head = stack->head->prev;
        --stack->length;

        free(stack_item);

        return true;
}

bool cfbf_stack_peek(cfbf_stack *stack, uint32_t *dest)
{
        if (cfbf_stack_is_empty(stack)) {
                return false;
        }

        memcpy(dest, &stack->head->value, sizeof(uint32_t));

        return true;
}

bool cfbf_stack_is_empty(cfbf_stack *stack)
{
        return stack->length == 0;
}

void cfbf_destroy_stack(cfbf_stack *stack)
{
        if (cfbf_stack_is_empty(stack)) {
                goto destroy_stack;
        }

        cfbf_stack_item *item;

        while (stack->head != NULL) {
                item = stack->head;
                stack->head = item->prev;

                free(item);
        }

        goto destroy_stack;

destroy_stack:
        free(stack);
}