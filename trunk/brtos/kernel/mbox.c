
/*********************************************************************************************************
*                                               BRTOS
*                                Brazilian Real-Time Operating System
*                            Acronymous of Basic Real-Time Operating System
*
*                              
*                                  Open Source RTOS under MIT License
*
*
*
*                                           OS MailBox functions
*
*
*   Author:   Gustavo Weber Denardin
*   Revision: 1.0
*   Date:     20/03/2009
*
*********************************************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "OS_types.h"
#include "event.h"
#include "BRTOS.h"


#if (BRTOS_MBOX_EN == 1)
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Create MailBox Function                     /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

INT8U OSMboxCreate (void *message, BRTOS_Mbox **event)
{
  BRTOS_Mbox *pont_event;
  INT16S i = 0;

  if (iNesting > 0) {                                // See if caller is an interrupt
      return(IRQ_PEND_ERR);                          // Can't be create by interrupt
  }
    
  // Enter critical
  // Verifica se ainda h� blocos de controle de eventos dispon�veis
  if (iBRTOS_Mbox < BRTOS_MAX_MBOX)
  {
      iBRTOS_Mbox++;
      pont_event = OSMboxFreeList;
      
      // Posiciona a lista de blocos de controle de eventos livre no pr�ximo bloco dispon�vel
      OSMboxFreeList = &BRTOS_Mbox_Table[iBRTOS_Mbox];
  } else
  {
    // Caso n�o haja mais blocos dispon�veis, retorna exce��o
    return(NO_AVAILABLE_EVENT);
  }    
    

    // Exit Critical
  pont_event->OSEventPointer   = message;                // Link EventPtr to message
  pont_event->OSEventWait      = 0;
  
  for(i=0;i<WAIT_LIST_SIZE;i++)
  {
    pont_event->OSEventList[i]=0;  
  }
  
  *event = pont_event;
  
  return(ALLOC_EVENT_OK);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Mailbox Pend Function                       /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

void *OSMboxPend (BRTOS_Mbox *pont_event, INT16U timeout)
{
  INT16U time_wait = 0;
  INT16S i = 0;  
  
  // Enter Critical Section
  OSEnterCritical;  
  
  if(iNesting > 0)
  {
    OSExitCritical;
    return (void *)0;
  }
  
  ContextTask[currentTask].Suspended = TRUE;
  ContextTask[currentTask].SuspendedType = MAILBOX;
  
  if (timeout)
  {  
    time_wait = counter + timeout;
    if (time_wait >= TickCountOverFlow)
      time_wait = timeout - (TickCountOverFlow - counter);
    ContextTask[currentTask].TimeToWait = time_wait;
  } else
    ContextTask[currentTask].TimeToWait = 65000;  
  
  // Aloca a tarefa com semaforo pendente na lista do evento
  for(i=0;i<WAIT_LIST_SIZE;i++)
  {
    if(pont_event->OSEventList[i] == currentTask)
    {
      break;
    }
      
    if(pont_event->OSEventList[i] == 0)
    {
      pont_event->OSEventList[i] = currentTask;
      pont_event->OSEventWait++;
      break;
    }
  }
  
  // Troca contexto e s� retorna quando estourar Delay ou quando ocorrer um Post do Semaforo
  ChangeContext(); 
  return (pont_event->OSEventPointer);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
/////      Mailbox Post Function                       /////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

INT8U OSMboxPost(BRTOS_Mbox *pont_event, void *message)
{
  int i=0;
  
  // Critical Section
  #if (Coldfire == 1)
  if (!iNesting)
  #endif
     OSEnterCritical;
       
  if (pont_event->OSEventWait != 0)                                  // See if any task is waiting for Message
  {
    for(i=0;i<WAIT_LIST_SIZE;i++)
    {
	    if (pont_event->OSEventList[i] != 0)
	    {    
        ContextTask[pont_event->OSEventList[i]].Suspended = READY;   // Coloca a tarefa na Ready List
      }
    }
    pont_event->OSEventPointer = message;
    
    
    ChangeContext();
    return OK;
  }
  
  return ERR_EVENT_NO_CREATED;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
#endif
