#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_H2R "USAGE\n" \
                  "    hex2rgb <HEX[A]>           Convert hex to rgb\n" \
                  "    hex2rgb                    Read from stdin\n"
#define USAGE_R2H "USAGE\n" \
                  "    rgb2hex <R> <G> <B> [A]    Convert rgb to hex (separate args)\n" \
                  "    rgb2hex <R,G,B[,A]>        Convert rgb to hex (single arg)\n" \
                  "    rgb2hex                    Read from stdin\n"
#define THROW(e) do { fputs(e, stderr); exit(EXIT_FAILURE); } while (0)


const char *basename(const char *path) {
    const char *name = strrchr(path, '/');
    return name ? ++name : path;
}

int main(int argc, char **argv) {
    char buf[16];
    char *in = buf;
    unsigned int n, r, g, b, a;

    const char *name = basename(argv[0]);
    if (strcmp(name, "hex2rgb") == 0)
        goto hex2rgb;
    else if (strcmp(name, "rgb2hex") == 0)
        goto rgb2hex;
    else
        THROW("Unknown basename, valid values are 'hex2rgb' and 'rgb2hex'\n");
    return EXIT_FAILURE;

hex2rgb:
    // Setup input string
    if (argc == 1) {
        if (fgets(buf, sizeof buf, stdin))
            buf[strcspn(buf, "\r\n")] = '\0';
        else
            THROW(USAGE_H2R);
    } else if (argc == 2)
        in = argv[1];
    else
        THROW(USAGE_H2R);

    if (in[0] == '#')
        in++;

    // Read values
    n = sscanf(in, "%2x%2x%2x%2x", &r, &g, &b, &a);
    if (r > 255 || g > 255 || b > 255 || a > 255)
        THROW(USAGE_H2R);

    // Print values
    if (n == 3 && strlen(in) == 6)
        printf("%u,%u,%u\n", r, g, b);
    else if (n == 4 && strlen(in) == 8)
        printf("%u,%u,%u,%u\n", r, g, b, a);
    else
        THROW(USAGE_H2R);
    goto success;

rgb2hex:
    // Setup input string
    if (argc == 1) {
        if (fgets(buf, sizeof buf, stdin))
            buf[strcspn(buf, "\r\n")] = '\0';
        else
            THROW(USAGE_R2H);
    } else if (argc == 2)
        in = argv[1];
    else if (argc == 4 || argc == 5) {
        // Concatenate component args
        char *p = in;
        for (int i = 1; i < argc; ++i) {
            if (i != 1)
                *p++ = ',';
            for (int j = 0; argv[i][j] != '\0'; ++j) {
                if (j == 3)
                    THROW(USAGE_R2H);
                *p++ = argv[i][j];
            }
        }
        *p = '\0';
    } else
        THROW(USAGE_R2H);

    // Read values
    n = sscanf(in, "%u%*[, ]%u%*[, ]%u%*[, ]%u", &r, &g, &b, &a);
    if (r > 255 || g > 255 || b > 255 || a > 255)
        THROW(USAGE_R2H);
    
    // Print values
    if (n == 3 && argc < 5)
        printf("#%02X%02X%02X\n", r, g, b);
    else if (n == 4)
        printf("#%02X%02X%02X%02X\n", r, g, b, a);
    else
        THROW(USAGE_R2H);
    goto success;

success:
    return EXIT_SUCCESS;
}
