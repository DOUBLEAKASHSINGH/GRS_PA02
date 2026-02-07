/*
 * MT24110_Common.c
 * Common utilities for socket communication implementations
 * Myself: Akash Singh (MT24110)
 * Location: Bulandshahr, UP, INDIA
 * Education: MTech at IIITD, CSE
 */

#include "MT24110_Common.h"

/*
 * Create a message with 8 dynamically allocated string fields
 * Each field is filled with test data of specified size
 */
MT24110_Message *mt24110_create_message(int field_size) {
    MT24110_Message *msg = malloc(sizeof(MT24110_Message));
    MT24110_CHECK_NULL(msg, "malloc message");

    int total_size = field_size / 8;
    int remainder = field_size % 8;
    int alloc_size = (total_size > 0) ? total_size : 1;

    /* Allocate and fill each field with test data */
    msg->field1 = malloc(alloc_size);
    msg->field2 = malloc(alloc_size);
    msg->field3 = malloc(alloc_size);
    msg->field4 = malloc(alloc_size);
    msg->field5 = malloc(alloc_size);
    msg->field6 = malloc(alloc_size);
    msg->field7 = malloc(alloc_size);
    msg->field8 = malloc(alloc_size + remainder);

    MT24110_CHECK_NULL(msg->field1, "malloc field1");
    MT24110_CHECK_NULL(msg->field2, "malloc field2");
    MT24110_CHECK_NULL(msg->field3, "malloc field3");
    MT24110_CHECK_NULL(msg->field4, "malloc field4");
    MT24110_CHECK_NULL(msg->field5, "malloc field5");
    MT24110_CHECK_NULL(msg->field6, "malloc field6");
    MT24110_CHECK_NULL(msg->field7, "malloc field7");
    MT24110_CHECK_NULL(msg->field8, "malloc field8");

    /* Fill with simple test pattern - my initials AS */
    memset(msg->field1, 'A', alloc_size - 1);
    memset(msg->field2, 'S', alloc_size - 1);
    memset(msg->field3, 'M', alloc_size - 1);
    memset(msg->field4, 'T', alloc_size - 1);
    memset(msg->field5, '2', alloc_size - 1);
    memset(msg->field6, '4', alloc_size - 1);
    memset(msg->field7, '1', alloc_size - 1);
    memset(msg->field8, '0', alloc_size + remainder - 1);

    /* Null terminate each field */
    msg->field1[alloc_size - 1] = '\0';
    msg->field2[alloc_size - 1] = '\0';
    msg->field3[alloc_size - 1] = '\0';
    msg->field4[alloc_size - 1] = '\0';
    msg->field5[alloc_size - 1] = '\0';
    msg->field6[alloc_size - 1] = '\0';
    msg->field7[alloc_size - 1] = '\0';
    msg->field8[alloc_size + remainder - 1] = '\0';

    return msg;
}

/*
 * Destroy a message and free all dynamically allocated fields
 */
void mt24110_destroy_message(MT24110_Message *msg) {
    if (msg == NULL) return;

    free(msg->field1);
    free(msg->field2);
    free(msg->field3);
    free(msg->field4);
    free(msg->field5);
    free(msg->field6);
    free(msg->field7);
    free(msg->field8);
    free(msg);
}

/*
 * Serialize message into a flat buffer for transmission
 * Buffer layout: field1|field2|field3|field4|field5|field6|field7|field8
 */
void mt24110_serialize_message(MT24110_Message *msg, char *buffer, int buffer_size) {
    int offset = 0;

    /* Copy each field sequentially into buffer */
    int len1 = strlen(msg->field1) + 1;
    int len2 = strlen(msg->field2) + 1;
    int len3 = strlen(msg->field3) + 1;
    int len4 = strlen(msg->field4) + 1;
    int len5 = strlen(msg->field5) + 1;
    int len6 = strlen(msg->field6) + 1;
    int len7 = strlen(msg->field7) + 1;
    int len8 = strlen(msg->field8) + 1;

    memcpy(buffer + offset, msg->field1, len1);
    offset += len1;
    memcpy(buffer + offset, msg->field2, len2);
    offset += len2;
    memcpy(buffer + offset, msg->field3, len3);
    offset += len3;
    memcpy(buffer + offset, msg->field4, len4);
    offset += len4;
    memcpy(buffer + offset, msg->field5, len5);
    offset += len5;
    memcpy(buffer + offset, msg->field6, len6);
    offset += len6;
    memcpy(buffer + offset, msg->field7, len7);
    offset += len7;
    memcpy(buffer + offset, msg->field8, len8);
    offset += len8;
}

/*
 * Deserialize buffer back into message structure
 */
MT24110_Message *mt24110_deserialize_message(char *buffer, int buffer_size) {
    MT24110_Message *msg = malloc(sizeof(MT24110_Message));
    MT24110_CHECK_NULL(msg, "malloc deserialize");

    int offset = 0;

    /* Extract each field from buffer */
    msg->field1 = strdup(buffer + offset);
    offset += strlen(msg->field1) + 1;
    msg->field2 = strdup(buffer + offset);
    offset += strlen(msg->field2) + 1;
    msg->field3 = strdup(buffer + offset);
    offset += strlen(msg->field3) + 1;
    msg->field4 = strdup(buffer + offset);
    offset += strlen(msg->field4) + 1;
    msg->field5 = strdup(buffer + offset);
    offset += strlen(msg->field5) + 1;
    msg->field6 = strdup(buffer + offset);
    offset += strlen(msg->field6) + 1;
    msg->field7 = strdup(buffer + offset);
    offset += strlen(msg->field7) + 1;
    msg->field8 = strdup(buffer + offset);

    return msg;
}

/*
 * Initialize statistics structure
 */
void mt24110_init_stats(MT24110_Stats *stats) {
    atomic_store(&stats->bytes_sent, 0);
    atomic_store(&stats->bytes_received, 0);
    atomic_store(&stats->messages_sent, 0);
    atomic_store(&stats->messages_received, 0);
    atomic_store(&stats->total_latency_us, 0);
}

/*
 * Print statistics in human-readable format
 */
void mt24110_print_stats(MT24110_Stats *stats) {
    long bs = atomic_load(&stats->bytes_sent);
    long br = atomic_load(&stats->bytes_received);
    long ms = atomic_load(&stats->messages_sent);
    long mr = atomic_load(&stats->messages_received);
    long tl = atomic_load(&stats->total_latency_us);

    printf("\n=== Statistics ===\n");
    printf("Bytes Sent: %ld\n", bs);
    printf("Bytes Received: %ld\n", br);
    printf("Messages Sent: %ld\n", ms);
    printf("Messages Received: %ld\n", mr);
    if (mr > 0) {
        printf("Avg Latency: %.2f us\n", (double)tl / mr);
    }
}

