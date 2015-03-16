
#include <stdlib.h>
#include <string.h>

#include "dynamic_buffer.h"

enum {
    DYNAMIC_BUF_ATTACHED_MOD,
    DYNAMIC_BUF_UNATTACHED_MOD
};

struct dynamic_buffer {
    char        *start_pos;
    char        *curr_pos;
    unsigned int   buf_len;
    unsigned int   sum_len; 
    int            flg;
};

struct dynamic_buffer *dynbuffer_new() {
    struct dynamic_buffer * newbuf = 
        (struct dynamic_buffer *)  malloc (sizeof (struct dynamic_buffer));
    if (newbuf) {
        /* init a space which length is 4096 as default */
        newbuf->buf_len  = 4096;
        newbuf->curr_pos = newbuf->start_pos = (char *) malloc (newbuf->buf_len);
        newbuf->flg      = DYNAMIC_BUF_ATTACHED_MOD;
    }
    return newbuf;
}


struct dynamic_buffer *dynbuffer_unattached_mode_new() {
    struct dynamic_buffer *newbuf = dynbuffer_new();
    if (newbuf) {
        newbuf->flg = DYNAMIC_BUF_UNATTACHED_MOD;
    }
    return newbuf;
}

void dynbuffer_free(struct dynamic_buffer *dbuf) {
    if (dbuf) {
        if (dbuf->flg == DYNAMIC_BUF_ATTACHED_MOD) {
            free (dbuf->start_pos);
        }
        free (dbuf);
    }
}

char *dynbuffer_write(struct dynamic_buffer* dbuf, const char *s, size_t len) {
    if (dbuf) {
        if (dynbuffer_acquire(dbuf, len) == len) {
            memcpy(dbuf->curr_pos, s, len);
            dynbuffer_consume(dbuf, len);
        }
        return dbuf->curr_pos;
    }
    return NULL; 
}

char *dynbuffer_writable_pos(struct dynamic_buffer *dbuf) {
    if (dbuf) {
        return dbuf->curr_pos;
    }
    return NULL;
}

char *dynbuffer_data(struct dynamic_buffer *dbuf) {
    if (dbuf) return dbuf->start_pos;
    else      return NULL;
}

unsigned int dynbuffer_data_len(struct dynamic_buffer *dbuf) {
    if (dbuf) return dbuf->sum_len;
    else      return 0;
}

int  dynbuffer_acquire(struct dynamic_buffer *dbuf, unsigned int len) {
    int ret = 0;
    if (dbuf && len > 0) {
        /* if the left buffer is not enough, increase buffer len */
        if (dbuf->curr_pos + len > dbuf->start_pos + dbuf->buf_len) {
            int new_len = dbuf->buf_len * 2;
            char *new_buff = (char *)realloc(dbuf->start_pos, new_len); 
            if (new_buff) {
                dbuf->start_pos = new_buff;
                dbuf->buf_len = new_len; 
                /* re-calc the writable position in new buffer */
                dbuf->curr_pos  = dbuf->start_pos + dbuf->sum_len;
                ret = len;
            }
            else {
                /* relloc failed. but we don't need free the previous memory */
            }
        }
        else { /* it mean there is enough space to use, just return length */
            ret = len; 
        }
    }
    return ret;
}

int dynbuffer_consume(struct dynamic_buffer *dbuf, unsigned int len) {
    if (dbuf && len > 0) {
        dbuf->curr_pos += len;
        dbuf->sum_len += len;
    }
    return dbuf->sum_len;
}

