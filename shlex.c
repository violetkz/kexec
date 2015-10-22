/*
 * This code is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 */
/*                      
 *                                              Ken <proxy.vk@gmail.com>
 *                                                            2015-10-22
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamic_buffer.h"

static char * parse_token_string(const char **pp) {
    const char *p = *pp;
    char  c = *p++;   /* skip quota */
    struct dynamic_buffer *token_buff = dynbuffer_unattached_mode_new();

    while (*p != '\0')  {
        if ( *p != c) {
            dynbuffer_write(token_buff, p, 1);
            p++;  /* move to next */
        }
        else {
            p++;  /* skip the quota in end of string */
            break;
        }
    }

    *pp = p;

    char *data = dynbuffer_data(token_buff);
    //printf("string token: ^%s^\n", data);
    dynbuffer_free(token_buff);
    return data;
}

static char *parse_token_cmdarg(const char **pp) {

    const char *p = *pp;
    struct dynamic_buffer *token_buff = dynbuffer_unattached_mode_new();

    while (*p != '\0' && *p != ' ' && *p != '\t') {
        if (*p == '\\') {   /* handle escape char. */
            p++;    /* skip escape char */
            if (*p != '\0') { 
                dynbuffer_write(token_buff, p, 1);
                p++;
            }
            else  break;
        }
        else {     /* common char */
            dynbuffer_write(token_buff, p, 1);
            p++;
        }
    }
    *pp = p;

    char *data = dynbuffer_data(token_buff);
    //printf("cmdarg token: ^%s^\n", data);
    dynbuffer_free(token_buff);
    return data;
}


int shlex_split(const char *p, char * argv[], int *argv_len) {
    char *token_buff = NULL;
    int i = 0;
    int pos = 0;
    while (p != NULL && *p != '\0') {
        char c = *p;
        switch (c) {
        case ' ':
        case '\t':
            p++;        /*  skip space; */
            break;
        case '\'':
        case '"':
            if (*argv_len - 1 > pos ) {
                argv[pos++] = parse_token_string(&p);
            }
            else return 1;
            break;
        default:
            if (*argv_len - 1> pos) {
                argv[pos++] = parse_token_cmdarg(&p);
            }
            else return 1;

            break;
        }
    }
    argv[pos] = NULL;  /* append a NULL as terminal flag */
    *argv_len = pos;   /* update used len to outside */
    return 0;
}

void free_shlex_argv(char* argv[], int argv_len) {
    int i = 0;
    for(; i < argv_len; ++i) {
        free(argv[i]);
        argv[i] = NULL;
    }
}

#ifdef _shlex_main_
int main(int argc, const char *argv[]) {
    if (argc > 0) {
        char * arr[32]; 
        int   arrlen = sizeof(arr);
        memset(arr, 0x00, sizeof (arr));
        shlex_split(argv[1], arr, &arrlen);
        
        printf("==:Token Num: %d\n", arrlen);
        char **p = &arr[0];
        while (p && *p != NULL) {
            printf("   :%s\n", *p);
            p++;
        }
    }
    
    return 0;
}
#endif
