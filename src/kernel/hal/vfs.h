#ifndef _VFS_H_
#define _VFS_H_

#include <stdint.h>
#include <stddef.h>

typedef int fd_t;

#define STDIN   0
#define STDOUT  1
#define STDERR  2
#define STDDBG  3

size_t fd_write(fd_t file, uint8_t* data, size_t size);

#endif