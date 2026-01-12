
#ifndef BECOL_ERRORS
#define BECOL_ERRORS

#include "../memory/mem.h"
#include <stdbool.h>

typedef enum { BECOL_ERROR_GENERIC, BECOL_ERROR_PARSING } ErrorCode;

typedef struct {
    ErrorCode err;
    char *message;
} Error;

extern bool errorFlag;
extern Error *currentError;

void BecolReportError(MemoryArena *arena, ErrorCode err, char *message);
bool BecolIsError();
void BecolClearError();
void BecolPrintError();

#endif
