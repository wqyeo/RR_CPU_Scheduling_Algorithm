#ifndef COLOR_PRINT_H
#define COLOR_PRINT_H

#define COLOR_WHITE   "\x1b[37m"
#define COLOR_GRAY    "\x1b[90m"
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define PRINT_WHITE(msg)   printf("%s%s%s", COLOR_WHITE, msg, COLOR_RESET)
#define PRINT_GRAY(msg)    printf("%s%s%s", COLOR_GRAY, msg, COLOR_RESET)
#define PRINT_RED(msg)     printf("%s%s%s", COLOR_RED, msg, COLOR_RESET)
#define PRINT_GREEN(msg)   printf("%s%s%s", COLOR_GREEN, msg, COLOR_RESET)
#define PRINT_YELLOW(msg)  printf("%s%s%s", COLOR_YELLOW, msg, COLOR_RESET)
#define PRINT_BLUE(msg)    printf("%s%s%s", COLOR_BLUE, msg, COLOR_RESET)
#define PRINT_MAGENTA(msg) printf("%s%s%s", COLOR_MAGENTA, msg, COLOR_RESET)
#define PRINT_CYAN(msg)    printf("%s%s%s", COLOR_CYAN, msg, COLOR_RESET)

#endif
