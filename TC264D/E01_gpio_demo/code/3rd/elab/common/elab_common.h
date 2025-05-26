/*
 * eLab Project
 * Copyright (c) 2023, EventOS Team, <event-os@outlook.com>
 */

#ifndef ELAB_COMMON_H
#define ELAB_COMMON_H

/* includes ----------------------------------------------------------------- */
#include "../../../os/cmsis_os.h"
#include "elab_def.h"
#ifdef __cplusplus
extern "C" {
#endif

/* public typedef ----------------------------------------------------------- */
uint32_t elab_version(void);

/* time related. */

uint32_t elab_time_ms(void);

/* Memory management related. */
void *elab_malloc(uint32_t size);
void elab_free(void *memory);


#ifdef __cplusplus
}
#endif

#endif

/* ----------------------------- end of file -------------------------------- */
