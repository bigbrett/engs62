#include "stdint.h"
#include "LED.h"
#include "mutex.h"
#include "USART2.h"
#include "WiFi.h"

typedef enum {
	STATE_RESET = 0,
	STATE_ECHO_BYTES,
	STATE_PING,
	STATE_PING_RECV,
	STATE_UPDATE,
	STATE_UPDATE_RECV,
	STATE_GET_ID,
	STATE_GOT_ID,
	STATE_HAVE_ID,
	STATE_ID_RECV
} state_t;

/* Initialize the finite state machine */
void fsm_init(void);

/* Acquire a lock on the fsm before accessing/altering state */
#define FSM_LOCK_FAILED   (0)
#define FSM_LOCK_ACQUIRED (1)
uint8_t fsm_lock(void);
void fsm_unlock(void);

/* Query FSM state - only do this when you have acquired the lock! */
state_t fsm_get_state(void);

/* Alter FSM state - only do this when you have acquired the lock! */
void fsm_set_state(state_t new_state);
