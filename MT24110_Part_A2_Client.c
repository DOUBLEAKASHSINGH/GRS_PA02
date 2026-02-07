/*
 * MT24110_Part_A2_Client.c
 * One-copy optimized client using sendmsg()
 * Myself: Akash Singh (MT24110)
 * Location: Bulandshahr, UP, INDIA
 * Education: MTech at IIITD, CSE
 *
 * EXPLAINATION OF ONE-COPY ELIMINATION:
 * Traditional send() does:
 *   Copy 1: User buffer -> Kernel socket buffer
 *
 * Using sendmsg() with pre-registered iovec:
 *   Eliminates the intermediate copy by passing pointer to user buffer
 *   Kernel reads directly from user memory
 *
 * Copy still occurs on recv() side (kernel -> user)
 * Total: 1 copy (reduced from 2 in baseline)
 */

#include "MT24110_Common.h"

MT24110_ClientConfig config;
MT24110_Stats client_stats;

/* Thread-specific data */
typedef struct {
    int thread_id;
    int sock_fd;
    long bytes_sent;
    long bytes_received;
    long messages_sent;
    long messages_received;
    long total_latency_us;
} MT24110_ThreadData;

/* Worker thread using sendmsg() for one-copy send */
void *mt24110_worker_thread(void *arg) {
    MT24110_ThreadData *data = (MT24110_ThreadData *)arg;

    char *buffer = malloc(config.message_size);
    MT24110_CHECK_NULL(buffer, "malloc buffer");

    /* Create message and serialize once */
    MT24110_Message *msg = mt24110_create_message(config.message_size);
    mt24110_serialize_message(msg, buffer, config.message_size);
    mt24110_destroy_message(msg);

    struct iovec iov[1];
    struct msghdr msg_header;
    struct timespec start, end;

    while (config.running) {
        clock_gettime(CLOCK_MONOTONIC, &start);

        /* Prepare msghdr for sendmsg - ONE COPY */
        memset(&msg_header, 0, sizeof(msg_header));
        iov[0].iov_base = buffer;
        iov[0].iov_len = config.message_size;
        msg_header.msg_iov = iov;
        msg_header.msg_iovlen = 1;

        /* sendmsg sends directly from user buffer - NO COPY to kernel buffer */
        int sent = sendmsg(data->sock_fd, &msg_header, 0);
        if (sent < 0) {
            perror("sendmsg failed");
            break;
        }
        data->bytes_sent += sent;
        data->messages_sent++;

        /* Receive still involves one copy (kernel -> user) */
        memset(&msg_header, 0, sizeof(msg_header));
        iov[0].iov_base = buffer;
        iov[0].iov_len = config.message_size;
        msg_header.msg_iov = iov;
        msg_header.msg_iovlen = 1;

        int received = recvmsg(data->sock_fd, &msg_header, 0);
        if (received <= 0) {
            if (received == 0) {
                printf("Server closed connection\n");
            } else {
                perror("recvmsg failed");
            }
            break;
        }
        data->bytes_received += received;
        data->messages_received++;

        clock_gettime(CLOCK_MONOTONIC, &end);
        long latency = (end.tv_sec - start.tv_sec) * 1000000L +
                      (end.tv_nsec - start.tv_nsec) / 1000L;
        data->total_latency_us += latency;
    }

    free(buffer);

    /* Aggregate to global stats */
    atomic_fetch_add(&client_stats.bytes_sent, data->bytes_sent);
    atomic_fetch_add(&client_stats.bytes_received, data->bytes_received);
    atomic_fetch_add(&client_stats.messages_sent, data->messages_sent);
    atomic_fetch_add(&client_stats.messages_received, data->messages_received);
    atomic_fetch_add(&client_stats.total_latency_us, data->total_latency_us);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <server_ip> <port> <message_size> <num_threads> <duration_sec>\n", argv[0]);
        return EXIT_FAILURE;
    }

    config.server_ip = argv[1];
    config.port = atoi(argv[2]);
    config.message_size = atoi(argv[3]);
    config.num_threads = atoi(argv[4]);
    config.duration_sec = atoi(argv[5]);
    config.running = 1;

    printf("One-copy client connecting to %s:%d\n", config.server_ip, config.port);
    printf("Message size: %d bytes, Threads: %d, Duration: %d sec\n",
           config.message_size, config.num_threads, config.duration_sec);
    printf("Using sendmsg() - one copy eliminated on send path\n");

    mt24110_init_stats(&client_stats);

    pthread_t threads[config.num_threads];
    MT24110_ThreadData thread_data[config.num_threads];
    int sock_fds[config.num_threads];

    for (int i = 0; i < config.num_threads; i++) {
        sock_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        MT24110_CHECK_NULL((void *)(intptr_t)sock_fds[i], "socket failed");

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(config.port);

        if (inet_pton(AF_INET, config.server_ip, &server_addr.sin_addr) <= 0) {
            perror("inet_pton failed");
            return EXIT_FAILURE;
        }

        if (connect(sock_fds[i], (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("connect failed");
            return EXIT_FAILURE;
        }

        /* Disable Nagle for lower latency */
        int flag = 1;
        setsockopt(sock_fds[i], IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(flag));

        thread_data[i].thread_id = i;
        thread_data[i].sock_fd = sock_fds[i];
        thread_data[i].bytes_sent = 0;
        thread_data[i].bytes_received = 0;
        thread_data[i].messages_sent = 0;
        thread_data[i].messages_received = 0;
        thread_data[i].total_latency_us = 0;
    }

    /* Start worker threads */
    for (int i = 0; i < config.num_threads; i++) {
        if (pthread_create(&threads[i], NULL, mt24110_worker_thread, &thread_data[i]) != 0) {
            perror("pthread_create failed");
            return EXIT_FAILURE;
        }
    }

    /* Run for specified duration */
    sleep(config.duration_sec);
    config.running = 0;

    /* Wait for threads to finish */
    for (int i = 0; i < config.num_threads; i++) {
        pthread_join(threads[i], NULL);
        close(sock_fds[i]);
    }

    /* Print results */
    long bs = atomic_load(&client_stats.bytes_sent);
    long br = atomic_load(&client_stats.bytes_received);
    long ms = atomic_load(&client_stats.messages_sent);
    long mr = atomic_load(&client_stats.messages_received);
    long tl = atomic_load(&client_stats.total_latency_us);

    double duration = (double)config.duration_sec;
    double throughput_gbps = (bs * 8.0) / (duration * 1e9);
    double avg_latency_us = (mr > 0) ? (double)tl / mr : 0;

    printf("\n=== One-Copy Client Results ===\n");
    printf("Duration: %.2f seconds\n", duration);
    printf("Total bytes sent: %ld (%.2f GB)\n", bs, bs / 1e9);
    printf("Total bytes received: %ld (%.2f GB)\n", br, br / 1e9);
    printf("Messages sent: %ld\n", ms);
    printf("Messages received: %ld\n", mr);
    printf("Throughput: %.4f Gbps\n", throughput_gbps);
    printf("Average latency: %.2f us\n", avg_latency_us);

    return EXIT_SUCCESS;
}

