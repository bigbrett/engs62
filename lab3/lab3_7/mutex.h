#pragma once


/*
 * Routine to try and lock mutex
 * returns r0=0 if successful lock, r0=1 for FAIL */
int lock_mutex(void);

/* unlocks mutex by restoring to 0 */
void unlock_mutex(void);




