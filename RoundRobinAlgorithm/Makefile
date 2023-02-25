# Compiler settings
CC=gcc
CFLAGS=-Wall -Wextra -std=c99

# Directories
SRC_DIR=./
RR_DIR=./RoundRobin
MODELS_DIR=./Models
UTIL_DIR=./Util

# Source files
SRCS=$(SRC_DIR)/main.c $(SRC_DIR)/process_generator.c $(UTIL_DIR)/data_saver.c $(UTIL_DIR)/color_print.c $(RR_DIR)/best_quantum_time_round_robin.c $(RR_DIR)/manhattan_round_robin.c $(RR_DIR)/round_robin.c

# Object files
OBJS=$(SRCS:.c=.o)

# Header files
HDRS=$(SRC_DIR)/process_generator.h $(UTIL_DIR)/data_saver.h $(UTIL_DIR)/color_print.h $(RR_DIR)/best_quantum_time_round_robin.h $(RR_DIR)/manhattan_round_robin.h $(RR_DIR)/round_robin.h $(MODELS_DIR)/process.h $(MODELS_DIR)/process_result.h $(MODELS_DIR)/round_robin_result.h

# Output executable name
OUTPUT=program

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUTPUT)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(RR_DIR)/%.o: $(RR_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(MODELS_DIR)/%.o: $(MODELS_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(UTIL_DIR)/%.o: $(UTIL_DIR)/%.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)
