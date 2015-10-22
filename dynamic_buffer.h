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
#ifndef __DYNAMIC_BUFFER___H
#define __DYNAMIC_BUFFER___H

#ifdef  __cplusplus
extern "C" {
#endif 

/**
 * the simple implementation of dynamic buffer.
 * 
 * Note:
 * the dynamic buffer is implemented by C standard function 'realloc' 
 * so don't use it for large data. The initial length of buffer is 4096 bytes,
 * and each time increase double current length.
 */
struct dynamic_buffer;

/** create a dynamic buffer 
 *  @return if successful, return the pointer of buffer. If there is an error
 *          return a NULL pointer.
 */
struct dynamic_buffer *dynbuffer_new();

/** create a dynamic buffer with unattached mode.
 *  the 'unattached mode' means the buffer spaces will be not free after calling
 *  func 'dynbuffer_free', the caller Should release the memory with c standard
 *  func 'free' by self.
 *  @return if successful, return the pointer of buffer. If there is an error
 *          return a NULL pointer.
 */
struct dynamic_buffer *dynbuffer_unattached_mode_new();

/** free the memory */
void dynbuffer_free(struct dynamic_buffer *dbuf);

/** return the pointer of free space of buffer */
char *dynbuffer_writable_pos(struct dynamic_buffer *dbuf);

/** return the pointer of written data */
char *dynbuffer_data(struct dynamic_buffer *dbuf);

/** return the length of written data */
unsigned int dynbuffer_data_len(struct dynamic_buffer *dbuf);

/** try to acquire the new space with length 'len' 
 * @return if successful, return 'len', otherwise return 0
 */
int   dynbuffer_acquire(struct dynamic_buffer *dbuf, unsigned int len);

/** specify that the length which has consumed in buffer 
 * @return if successful, return the consumed length 'len', otherwise return 0
 */
int dynbuffer_consume(struct dynamic_buffer *dbuf, unsigned int len);

/**
 * write data into dbuf.
 * @param s   the data pointer 
 * @param len the data length.
 * @return the current writable position pointer.
 */
char *dynbuffer_write(struct dynamic_buffer* dbuf, const char *s, size_t len);

#ifdef  __cplusplus
}
#endif 

#endif //~__DYNAMIC_BUFFER___H
