/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

/* include ------------------------------------------------------------------ */

#include "elab_common.h"
#include "elab_def.h"
#include "bsp_pit.h"
/* public function ---------------------------------------------------------- */
uint32_t elab_version(void)
{
    return 000100;
}



uint32_t elab_time_ms(void)
{
    return (uint32_t)get_TC264D_ms();
}



#if !defined(__linux__) && !defined(_WIN32)
ELAB_WEAK
#endif
void *elab_malloc(uint32_t size)
{
    return malloc(size);
}

#if !defined(__linux__) && !defined(_WIN32)
ELAB_WEAK
#endif
void elab_free(void *memory)
{
    if (memory != NULL)
    {
        free(memory);
    }
}

/* ----------------------------- end of file -------------------------------- */
