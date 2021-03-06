#include "stdint.h"
#include "LED.h"
#include "mutex.h"
#include "servo.h"
#include "timer.h"
#include "USART2.h"

typedef enum {
	STATE_RESET = 0,
	STATE_TRAFFIC_INIT,
	STATE_TRAIN_INIT,
	STATE_TRAIN,
	STATE_PED_INIT,
	STATE_PED_XING,
	STATE_MAINTENANCE_INIT,
	STATE_MAINTENANCE
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

void close_gate(void);
void open_gate(void);


/* Getter/Setters for FSM control signals */
uint8_t get_PED(void);
uint8_t get_ARRIVING(void);
uint8_t get_CLOSED(void);
uint8_t get_CLEAR(void);
uint8_t get_HOLD(void);
void set_PED(uint8_t val);
void set_ARRIVING(uint8_t val);
void set_CLOSED(uint8_t val);
void set_CLEAR(uint8_t val);
void set_HOLD(uint8_t val);

