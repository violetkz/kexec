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
#include "prog_exec.h"
#include "shell_exec.h"
#include "dynamic_buffer.h"
#include "trace.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int args, char* argv[]){
    char *c;
    int i = 1;
    for (;i < args; ++i) {
        int rc = shell_exec(argv[i],  &c);
        if (rc != 0) {
            perror("shell_exec failed.");
        }
        printf("return:\n%s", c);
    }
    return 0;
}
