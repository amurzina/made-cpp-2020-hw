#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

// constants from wiki
#define PLUS_ZERO   0x0000000000000000
#define MINUS_ZERO  0x8000000000000000
#define PLUS_INF    0x7FF0000000000000
#define MINUS_INF   0xFFF0000000000000

#define SIGN        63
#define FRCT_STRT   51

#define EXP_MSK     0x7ff0000000000000
#define FRCT_MSK    0x000fffffffffffff

#define POSITIVE    0
#define NEGATIVE    1

/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1ull << index;
    return number & mask;
}

bool isNormal (const uint64_t number) {
    return ((number & EXP_MSK) != 0) && ((number & FRCT_MSK) != 0);
}

bool isDenormal(const uint64_t number) {
    return ((number & EXP_MSK) == 0) && getBit(number, 0);
}

/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == PLUS_ZERO;
}

bool checkForMinusZero (uint64_t number) {
    return number == MINUS_ZERO;
}

bool checkForPlusInf (uint64_t number) {
    return number == PLUS_INF;
}

bool checkForMinusInf (uint64_t number) {
    return number == MINUS_INF;
}

bool checkForPlusNormal (uint64_t number) {
    return getBit(number, SIGN) == POSITIVE && isNormal(number);
}

bool checkForMinusNormal (uint64_t number) {
    return getBit(number, SIGN) == NEGATIVE && isNormal(number);
}

bool checkForPlusDenormal (uint64_t number) {
return getBit(number, SIGN) == POSITIVE && isDenormal(number);
}

bool checkForMinusDenormal (uint64_t number) {
return getBit(number, SIGN) == NEGATIVE && isDenormal(number);
}

bool checkForSignalingNan (uint64_t number) {
    return (number & FRCT_MSK) == FRCT_MSK && (number & FRCT_MSK) == 1 && getBit(number, FRCT_STRT) == NEGATIVE;
}

bool checkForQuietNan (uint64_t number) {
    return (number & FRCT_MSK) == FRCT_MSK && getBit(number, FRCT_STRT);
}


void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}
