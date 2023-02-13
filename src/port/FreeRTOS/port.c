#include "port.h"
#include "FreeRTOS.h"
#include "semphr.h"

static SemaphoreHandle_t sema = NULL;

void allocatorPortInit(void)
{
    if (sema == NULL) {
        sema = xSemaphoreCreateBinary();
    }
}

void allocatorPortEnterCriticalSection(void)
{
    xSemaphoreTake(sema, portMAX_DELAY);
}

void allocatorPortExitCriticalSection(void)
{
    xSemaphoreGive(sema);
}