#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct xillyfifo {
    unsigned long read_total;
    unsigned long write_total;
    unsigned int bytes_in_fifo;
    unsigned int read_position;
    unsigned int write_position;
    unsigned int size;
    unsigned int done;
    unsigned char *baseaddr;
    sem_t write_sem;
    sem_t read_sem;
};

struct xillyinfo {
    int slept;
    int bytes;
    int position;
    void *addr;
};

// IMPORTANT:
// =========
//
// NONE of the fifo_* functions is reentrant. Only one thread should have
// access to any set of them. This is pretty straightforward when one thread
// writes and one thread reads from the FIFO.
//
// Also make sure that fifo_drained() and fifo_wrote() are NEVER called with
// req_bytes larger than what their request-counterparts RETURNED, or
// things will go crazy pretty soon.

int fifo_init(struct xillyfifo *fifo, unsigned int size);
void fifo_done(struct xillyfifo *fifo);
void fifo_destroy(struct xillyfifo *fifo);
int fifo_request_drain(struct xillyfifo *fifo, struct xillyinfo *info);
void fifo_drained(struct xillyfifo *fifo, unsigned int req_bytes);
int fifo_request_write(struct xillyfifo *fifo, struct xillyinfo *info);
void fifo_wrote(struct xillyfifo *fifo, unsigned int req_bytes);
