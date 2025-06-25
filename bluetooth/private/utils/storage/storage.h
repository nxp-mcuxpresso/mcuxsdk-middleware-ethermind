
/**
 *  \file storage.h
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_STORAGE_
#define _H_STORAGE_

/* --------------------------------------------- Header File Inclusion */
#include "BT_common.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Structures/Data Types */

/* --------------------------------------------- Macros */

#ifdef __cplusplus
extern "C"{
#endif
/* --------------------------------------------- Internal Functions */

/* --------------------------------------------- API Declarations */
void em_storage_init (void);
void storage_bt_init (void);
void storage_shutdown (void);
void storage_bt_shutdown (void);

#ifdef __cplusplus
};
#endif
#endif /* _H_STORAGE_ */
