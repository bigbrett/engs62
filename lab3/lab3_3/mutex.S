/*
 * mutex.S - mutex support in assembly on the STM32F4-Discovery Board
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 * REFERENCES:
 *
 * All documentation references written as "[#]-<section>" where:
 *
 * [1] = RM0090 STM32F407 Reference Manual.pdf
 * [2] = STM32F4 Discovery Schematic.pdf
 * [3] = STM32F407xx Data Sheet.pdf
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Make these routines visible to other modules */
    .global lock_mutex
    .global unlock_mutex

    .text

	.lcomm MUTEX 4 // initialize a mutex in RAM

/*
 * Routine to try and lock mutex
 * returns r0=0 if successful lock, r0=1 for FAIL
 */
lock_mutex:
	push {r1,r2,lr}
try_lock:
	ldr r1, =MUTEX
	ldrex r2, [r1] // get mutex status
	cmp r2, #0 // check if locked
	bne try_lock_fail // unsucessful: device already ulocked
is_unlocked:
	mov r0, #1 // attempt to lock device
	strex r2, r0,[r1] // try to store 1 in Mutex
	cmp r2, #0 // check if successful
	bne try_lock_fail // unsucessful: could not unlock
try_lock_good:
	mov r0, #0 // return SUCCESS status
	b exit_lock // exit
try_lock_fail:
	mov r0, #1 // return FAIL status
exit_lock:
	pop {r1,r2,pc}


/*
 * Routine to unlock mutex
 */
unlock_mutex:
	push {r1,r2,lr}
	ldr r1, =MUTEX
	mov r2, #0
	str r2, [r1] // set mutex to 1
	pop {r1,r2,pc}


	.end
