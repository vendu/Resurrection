/*
 * option.c - command-line option handling for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#if 0
Roption_t *
option_alloc(void)
{
    return (calloc(1, sizeof(Roption_t)));
}

Roption_t *
option_alloc_table(void)
{
    return (calloc(OPTION_HASH_SIZE, sizeof(Roption_t)));
}

void
option_destroy(Roption_t *option)
{
    if (option == NULL) {

	return;
    }

    option_destroy_data(option);

    free(option);

    return;
}

void
option_destroy_data(Roption_t *option)
{
    if (option == NULL) {

	return;
    }

    if (option->str) {
	free(option->str);
    }

    if (option->value.str) {
	free(option->value.str);
    }

    if (option->helpstr) {
	free(option->helpstr);
    }

    if (option->commandstr) {
	free(option->commandstr);
    }

    if (option->optionstr) {
	free(option->optionstr);
    }

    return;
}

int
option_copy(Roption_t *src, Roption_t *dest)
{
    size_t len;

    if (src == NULL
	|| dest == NULL) {

	return -1;
    }

    dest->type = src->type;

    if (src->str) {
	len = strlen(src->str);

	dest->str = calloc(1, len + 1);
	if (dest->str == NULL) {

	    return -1;
	}

	strncpy(dest->str, src->str, len);
    }

    dest->flag = src->flag;
    dest->valueptr = src->valueptr;

    if (src->value.str) {
	len = strlen(src->value.str);

	dest->value.str = calloc(1, len + 1);
	if (dest->value.str == NULL) {

	    return -1;
	}

	strncpy(dest->value.str, src->value.str, len);
    }

    if (src->helpstr) {
	len = strlen(src->helpstr);

	dest->helpstr = calloc(1, len + 1);
	if (dest->helpstr == NULL) {

	    return -1;
	}

	strncpy(dest->helpstr, src->helpstr, len);
    }

    if (src->commandstr) {
	len = strlen(src->commandstr);

	dest->commandstr = calloc(1, len + 1);
	if (dest->commandstr == NULL) {

	    return -1;
	}

	strncpy(dest->commandstr, src->commandstr, len);
    }

    if (src->optionstr) {
	len = strlen(src->optionstr);

	dest->optionstr = calloc(1, len + 1);
	if (dest->optionstr == NULL) {

	    return -1;
	}

	strncpy(dest->optionstr, src->optionstr, len);
    }

    switch (src->type) {
        case OPTION_BOOLEAN:
	    dest->value.boolean = src->value.boolean;

	    break;
        case OPTION_INTEGER:
	    dest->value.i = src->value.i;

	    break;
        case OPTION_UNSIGNED_INTEGER:
	    dest->value.ui = src->value.ui;

	    break;
        case OPTION_LONG:
	    dest->value.l = src->value.l;

	    break;
        case OPTION_UNSIGNED_LONG:
	    dest->value.ul = src->value.ul;

	    break;
        default:
	    break;
    }

    dest->window = src->window;
    dest->handler = src->handler;

    return 0;
}

int
option_register(Roption_t *optiontable, unsigned long type,
		const char *str,
		unsigned long flag,
		void *valueptr,
		const char *helpstr,
		const char *commandstr,
		const char *optionstr,
		Roptionhandler_t *handler)
{
    size_t len;
    Roption_t *newoption;
    
    if (optiontable == NULL
	|| type < 0
	|| type > OPTION_TYPES
	|| str == NULL) {
	
	return -1;
    }
    
    newoption = option_alloc();
    if (newoption == NULL) {
	
	return -1;
    }

    newoption->type = type;

    len = strlen(str);

    newoption->str = calloc(1, len + 1);
    if (newoption->str == NULL) {
	option_destroy(newoption);

	return -1;
    }

    strncpy(newoption->str, str, len);

    newoption->flag = flag;
    newoption->valueptr = valueptr;

    if (helpstr) {
	len = strlen(helpstr);

	newoption->helpstr = calloc(1, len + 1);
	if (newoption->helpstr == NULL) {
	    option_destroy(newoption);

	    return -1;
	}

	strncpy(newoption->helpstr, helpstr, len);
    }

    if (commandstr) {
	len = strlen(commandstr);

	newoption->commandstr = calloc(1, len + 1);
	if (newoption->commandstr == NULL) {
	    option_destroy(newoption);

	    return -1;
	}

	strncpy(newoption->commandstr, commandstr, len);
    }

    if (optionstr) {
	len = strlen(optionstr);

	newoption->optionstr = calloc(1, len + 1);
	if (newoption->optionstr == NULL) {
	    option_destroy(newoption);

	    return -1;
	}

	strncpy(newoption->optionstr, optionstr, len);
    }

    newoption->handler = handler;
    
    if (option_add(optiontable, newoption) < 0) {
	option_destroy(newoption);
	
	return -1;
    }
    
    return 0;
}

int
option_add(Roption_t *optiontable, Roption_t *option)
{
    unsigned long key = 0;
    Roption_t *tableoption = NULL;
    
    key = hash_compute_key(option->str, OPTION_HASH_PRIME);
    if (key < 0) {

	return -1;
    }

    if (option_find(optiontable, option->str) == 0) {

	return 0;
    }

    tableoption = &(optiontable[key]);

    if (tableoption->str == NULL) {
	option_copy(option, tableoption);
    } else {
	while (tableoption->next) {
	    tableoption = tableoption->next;
	}
	
	tableoption->next = option_alloc();
	if (tableoption->next == NULL) {
	    
	    return -1;
	}
	
	*(tableoption->next) = *option;
    }
    
    return 0;
}

Roption_t *
option_find(Roption_t *optiontable, const char *str)
{
    unsigned long key;
    Roption_t *tableoption;
    
    key = hash_compute_key(str, OPTION_HASH_PRIME);
    if (key < 0) {
	
	return NULL;
    }

    tableoption = &(optiontable[key]);
    
    if (tableoption->str == NULL) {
	
	return NULL;
    }
    
    if (strcasecmp(str, tableoption->str) == 0) {
	
	return tableoption;
    } else {
	tableoption = tableoption->next;
	while (tableoption) {
	    if (strcasecmp(str, tableoption->str) == 0) {
		
		return tableoption;
	    }

	    tableoption = tableoption->next;
	}
    }
    
    return NULL;
}

int
option_boolean_true(const char *str)
{
    if (str == NULL) {

	return FALSE;
    }

    if (!strcasecmp(str, "1")
	|| !strcasecmp(str, "on")
	|| !strcasecmp(str, "true")
	|| !strcasecmp(str, "yes")
	|| !strcasecmp(str, "enabled")) {

	return TRUE;
    }

    return FALSE;
}

int
option_boolean_false(const char *str)
{
    if (str == NULL) {

	return FALSE;
    }

    if (!strcasecmp(str, "0")
	|| !strcasecmp(str, "off")
	|| !strcasecmp(str, "false")
	|| !strcasecmp(str, "no")
	|| !strcasecmp(str, "disabled")) {

	return TRUE;
    }

    return FALSE;
}
#endif /* 0 */
