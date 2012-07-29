/*
 * binding.c - Resurrection button and key binding routines.
 * Copyright (C) 2006 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

int
R_add_binding(struct R_binding *binding,
              struct R_window *window)
{
    struct R_binding **bindings;
    struct R_binding *hashbinding ;
    int key;

    bindings = window->bindings;
    if (bindings == NULL) {
        bindings = calloc(sizeof(struct R_binding *),
                          R_BINDING_HASH_ITEMS);
        if (bindings == NULL) {

            return FALSE;
        }
        window->bindings = bindings;
    }
    key = R_HASH_BINDING(binding->mask);
    hashbinding = window->bindings[key];
    if (hashbinding) {
        binding->next = hashbinding;
    }
    window->bindings[key] = binding;

    return TRUE;
}

struct R_binding *
R_find_binding(struct R_window *window,
               R_binding_t mask)
{
    struct R_binding *binding;
    int key;

    if (window->bindings) {
        key = R_HASH_BINDING(mask);
        binding = window->bindings[key];
        while ((binding) && (binding->mask != mask)) {
            binding = binding->next;
        }

        return binding;
    }

    return NULL;
}

struct R_binding *
R_create_binding(struct R_window *window,
                 R_binding_t mask,
                 R_bindingfunc_t *func,
                 void *arg)
{
    struct R_binding *binding;

    binding = calloc(sizeof(struct R_binding),
                     1);
    if (binding) {
        binding->mask = mask;
        binding->func = func;
        binding->arg = arg;
    }

    return binding;
}

