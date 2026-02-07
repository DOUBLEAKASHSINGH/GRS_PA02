/*
 * MT24110_Part_A1_Server.c
 * Two-copy baseline implementation using send()/recv()
 * Myself: Akash Singh (MT24110)
 * Location: Bulandshahr, UP, INDIA
 * Education: MTech at IIITD, CSE
 *
 * EXPLAINATION OF TWO COPIES:
 * Copy 1: User buffer -> Kernel socket buffer (via send())
 * Copy 2: Kernel socket buffer -> User buffer (via recv())
 * Total: 2 copies between user and kernel space
 */

#include "MT24110_Common.h"

MT24110_ServerConfig config;
volatile int server_running = 1;

/* Signal handler for graceful shutdown */
void mt24110_signal_handler(int sig) {
    (void)sig;
    server_running = 0;
}

/* Handle client connection - one thread per client */
void *mt24110_client_handler(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    char *buffer = malloc(config.message_size);
    MT24110_CHECK_NULL(buffer, "malloc handler buffer");

    MT24110_Stats local_stats;
    mt24110_init_stats(&local_stats);

    /* Receive messages continuously */
    while (server_running) {
        int received = recv(client_fd, buffer, config.message_size, 0);

        if (received <= 0) {
            if (received == 0) {
                printf("Client disconnected\n");
            } else if (errno != EINTR) {
                perror("recv failed");
            }
            break;
        }

        atomic_fetch_add(&local_stats.bytes_received, received);
        atomic_fetch_add(&local_stats.messages_received, 1);

        /* Echo back to client (second copy for response) */
        int sent = send(client_fd, buffer, received, 0);
        if (sent < 0) {
            perror("send failed");
            break;
        }

        atomic_fetch_add(&local_stats.bytes_sent, sent);
        atomic_fetch_add(&local_stats.messages_sent, 1);
    }

    free(buffer);
    close(client_fd);

    return NULL;
}

int main(int argc, char *argv[]) {
    /* Parse command line arguments */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <port> <message_size>\n", argv[0]);
        fprintf(stderr, "Example: %s 8080 1024\n", argv[0]);
        return EXIT_FAILURE;
    }

    config.port = atoi(argv[1]);
    config.message_size = atoi(argv[2]);
    config.running = 1;

    /* Setup signal handler for Ctrl+C */
    signal(SIGINT, mt24110_signal_handler);
    signal(SIGTERM, mt24110_signal_handler);

    /* Create server socket */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    MT24110_CHECK_NULL((void *)(intptr_t)server_fd, "socket creation failed");

    /* Allow port reuse */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /* Bind to port */
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(config.port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    /* Listen for connections */
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return EXIT_FAILURE;
    }

    printf("Two-copy server listening on port %d\n", config.port);
    printf("Message size: %d bytes\n", config.message_size);

    /* Accept concurrent clients */
    while (server_running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int *client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

        if (*client_fd < 0) {
            if (errno == EINTR) continue;
            perror("accept failed");
            free(client_fd);
            continue;
        }

        printf("Client connected from %s:%d\n",
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        /* Create one thread per client */
        pthread_t tid;
        if (pthread_create(&tid, NULL, mt24110_client_handler, client_fd) != 0) {
            perror("pthread_create failed");
            close(*client_fd);
            free(client_fd);
        } else {
            pthread_detach(tid);
        }
    }

    close(server_fd);
    printf("Server shutdown complete\n");

    return EXIT_SUCCESS;
}

