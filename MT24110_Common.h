/*
 * MT24110_Common.h
 * Common header for socket communication implementations
 * Myself: Akash Singh (MT24110)
 * Location: Bulandshahr, UP, INDIA
 * Education: MTech at IIITD, CSE
 */

#ifndef MT24110_COMMON_H
#define MT24110_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <stdatomic.h>

/* Message structure with 8 dynamically allocated string fields */
typedef struct {
    char *field1;
    char *field2;
    char *field3;
    char *field4;
    char *field5;
    char *field6;
    char *field7;
    char *field8;
} MT24110_Message;

/* Server configuration */
typedef struct {
    int port;
    int message_size;
    int num_threads;
    volatile int running;
} MT24110_ServerConfig;

/* Client configuration */
typedef struct {
    char *server_ip;
    int port;
    int message_size;
    int num_threads;
    int duration_sec;
    volatile int running;
} MT24110_ClientConfig;

/* Statistics structure */
typedef struct {
    atomic_long bytes_sent;
    atomic_long bytes_received;
    atomic_long messages_sent;
    atomic_long messages_received;
    atomic_long total_latency_us;
} MT24110_Stats;

/* Function prototypes */
MT24110_Message *mt24110_create_message(int field_size);
void mt24110_destroy_message(MT24110_Message *msg);
void mt24110_serialize_message(MT24110_Message *msg, char *buffer, int buffer_size);
MT24110_Message *mt24110_deserialize_message(char *buffer, int buffer_size);
void mt24110_init_stats(MT24110_Stats *stats);
void mt24110_print_stats(MT24110_Stats *stats);

/* Utility macros */
#define MT24110_CHECK_NULL(ptr, msg) if ((ptr) == NULL) { perror(msg); exit(EXIT_FAILURE); }
#define MT24110_MIN(a, b) ((a) < (b) ? (a) : (b))

/* Default values */
#define MT24110_DEFAULT_PORT 8080
#define MT24110_DEFAULT_MESSAGE_SIZE 1024
#define MT24110_DEFAULT_NUM_THREADS 4
#define MT24110_DEFAULT_DURATION 5

#endif /* MT24110_COMMON_H */

