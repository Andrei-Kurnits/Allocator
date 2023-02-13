#ifndef PORT_H
#define PORT_H

void allocatorPortInit(void);
void allocatorPortEnterCriticalSection(void);
void allocatorPortExitCriticalSection(void);

#endif // ifndef PORT_H