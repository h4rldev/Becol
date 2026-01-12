#include "errors.h"
#include "../memory/mem.h"
#include <stdbool.h>
#include <stdio.h>

bool errorFlag = false;
Error *currentError;

void BecolReportError(MemoryArena *arena, ErrorCode err, char *message) {
    if (errorFlag) {
        printf("Error reported while error already existed\n");
        BecolPrintError();
        BecolClearError();
    }
    errorFlag = true;
    currentError = BecolArenaPushStruct(arena, Error, false);
    currentError->err = err;
    // currentError->message = BecolStrMalloc(message); // we cannot guarantee
    // that the string
    //  will be avaliable forever so we just
    //  copy it
}

bool BecolIsError() { return errorFlag; }

void BecolClearError() { errorFlag = false; }

void BecolPrintError() {
    if (errorFlag)
        printf("err: code: %d msg: \"%s\"\n", currentError->err,
               currentError->message); // TODO: actually good printing
}
