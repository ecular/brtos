/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __SYS_RTXC_H__
#define __SYS_RTXC_H__

#include "BRTOS.h"

#define SYS_MBOX_NULL (BRTOS_Queue*)0
#define SYS_SEM_NULL  (BRTOS_Sem*)0
#define SYS_DEFAULT_THREAD_STACK_DEPTH	1024
#define SYS_THREAD_NULL         NULL

typedef BRTOS_Sem* sys_sem_t;
typedef BRTOS_Queue* sys_mbox_t;
typedef ContextType* sys_thread_t;

typedef struct _sys_arch_state_t
{
	// Task creation data.
	char cTaskName[configMAX_TASK_NAME_LEN];
	unsigned short nStackDepth;
	unsigned short nTaskCount;
} sys_arch_state_t;

//extern sys_arch_state_t s_sys_arch_state;

void
sys_init(void);

sys_thread_t
sys_thread_new(char *name, void ( *thread ) ( void *arg ), void *arg, int stacksize, int prio );

/*FSL:workaround from lwIP port (1.1.1 to 1.3.1)*/
#define sys_arch_mbox_tryfetch(mbox,msg) \
    sys_arch_mbox_fetch(mbox,msg,1)

void  sys_assert( const char *const msg );
void  sys_debug( const char *const fmt, ... );

#endif /* __SYS_RTXC_H__ */

