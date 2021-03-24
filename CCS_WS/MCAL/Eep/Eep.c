/*
 * Eep.c
 *
 *  Created on: Nov 4, 2020
 *      Author: Ahmed Yusri Mohammed Ezzat.
 */

#include "Eep.h"
#include "SchM_Eep.h"
#include "TM4C129E_HwEepInfo.h"

#define EEP_MEMMAP_START_SEC_INIT_ZERO_VAR
#include "Eep_MemMap.h"

typedef enum
{
   E_EEP_INTERNAL_IDLE_OPER = 0,
   E_EEP_INTERNAL_CANCEL_OPER,
   E_EEP_INTERNAL_WRITE_OPER_PENDING,
   E_EEP_INTERNAL_ERASE_OPER_PENDING,
   E_EEP_INTERNAL_READ_OPER_PENDING,
   E_EEP_INTERNAL_COMPARE_OPER_PENDING,
   E_EEP_INTERNAL_WRITE_OPER_DONE,
   E_EEP_INTERNAL_ERASE_OPER_DONE,
   E_EEP_INTERNAL_READ_OPER_DONE
}Eep_operationStateType;

struct Eep_ProcCtx
{
   Eep_AddressType address;      /*Current address.*/
   Eep_LengthType rem;           /*remaining length.*/
   uint32 *data;                 /*Src Des data pointer.*/
   Eep_operationStateType op;    /*Suboperation.. */
   MemIf_JobResultType result;   /*job result.*/
   uint32 cmpBuffer;
};

typedef boolean (*Eep_InternalProcType)(struct Eep_ProcCtx *);

typedef struct Eep_ProcCtx Eep_ProcCtxType;

static struct
{
   const Eep_ConfigType *ConfigPtr;
   Eep_ProcCtxType ctx;
   Eep_InternalProcType internalProcessing;
   MemIf_StatusType state;
}Eep_globalVar =\
{
   NULL_PTR,
   {0, 0, NULL_PTR, E_EEP_INTERNAL_IDLE_OPER , MEMIF_JOB_OK , 0},
   NULL_PTR,
   MEMIF_UNINIT
};

#define EEP_MEMMAP_STOP_SEC_INIT_ZERO_VAR
#include "Eep_MemMap.h"

#define EEP_MEMMAP_START_SEC_CODE
#include "Eep_MemMap.h"

/*****************************************************************************
 * Local Functions.
 ******************************************************************************/

static inline void Eep_setupBlockIdx(uint32 address)
{
   /*set the current block index..*/
   EEP_REG_INFO()->EEBLOCK = (address >> EEP_WORD_SIZE_P2_IN_BYTES) >> EEP_BLOCK_SIZE_P2_IN_WORDS;
   
   /*set the word offset to correct word in the current block.*/
   EEP_REG_INFO()->EEOFFSET = address & EEP_BLOCK_SIZE_MASK;
}/*Eep_setupBlockIdx*/

static inline void Eep_updateBlockIdx(void)
{
   if(0 == EEP_REG_INFO()->EEOFFSET) /*if the OFFSET wraps around to 0.*/
   {
      EEP_REG_INFO()->EEBLOCK += 1; /*Increment the current block index..*/
   }
}/*Eep_updateBlockIdx*/

static inline void Eep_internalWrite(uint32 data)
{
   /*Write WORD/PAGE to the current location..*/
   EEP_REG_INFO()->EERDWRINC = data;
}/*Eep_internalWrite*/

static inline uint32 Eep_internalRead(void)
{
   return EEP_REG_INFO()->EERDWRINC;
}/*Eep_internalWrite*/

static inline void Eep_internalU8Copy(uint8 *src, uint8 *des, uint8 len)
{
   while(len--)
   {
      des[len] = src[len];
   }
}/*Eep_internalU8Copy*/

static inline boolean Eep_internalU8Compare(uint8 *src, uint8 *des, uint8 len)
{
   boolean isEqual = TRUE;

   while(len--)
   {
      if(des[len] != src[len]) /*Check if there is a mismatch..*/
      {
         isEqual = FALSE;
         break;
      }
   }
   return isEqual;
}/*Eep_internalU8Compare*/

/******************************************************************
 * Function Name: <Eep_idleProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Idlel state handler.
 ****************************************************************/
static boolean Eep_idleProcessing(struct Eep_ProcCtx *ctx)
{
   return FALSE; /*We don't need to exit from this state. "The Exit action will be taken by the requester."*/
}/*Eep_idleProcessing*/

/******************************************************************
 * Function Name: <Eep_writeProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Write state handler.
 ****************************************************************/
static boolean Eep_writeProcessing(struct Eep_ProcCtx *ctx)
{
   boolean isDone = FALSE;

   switch(ctx->op)
   {
      case E_EEP_INTERNAL_WRITE_OPER_PENDING:
      {
         if(!(EEP_REG_INFO()->EEDONE & 0X1)) /*The EEPROM is not working. "IDLE". */
         {
            if(!(EEP_REG_INFO()->EESUPP & 0X0C)) /*There is no Errors..*/
            {
               if(ctx->rem > 0)
               {
                  Eep_updateBlockIdx(); /*Update the block index if needed.*/
                  Eep_internalWrite(*ctx->data);
               
                  ++ctx->data; /*Update the internal variables*/
                  ctx->rem -= 4;
               }
               else
               {
                  ctx->op = E_EEP_INTERNAL_IDLE_OPER; /*Update internal supOperation state.*/
                  ctx->result = MEMIF_JOB_OK; /*job result is good..*/
                  isDone = TRUE; /*The current job is done ..*/
               }
            }
            else
            {
               ctx->op = E_EEP_INTERNAL_IDLE_OPER; /*Update internal supOperation state.*/
               ctx->result = MEMIF_JOB_FAILED; 
               isDone = TRUE; /*The current job is done ..*/
            }
         }
         break;
      }
      case E_EEP_INTERNAL_IDLE_OPER:
      default:
      {
         Eep_setupBlockIdx(ctx->address); /*Select the correct block..*/
         Eep_internalWrite(*ctx->data);
         
         ++ctx->data; /*Update the internal variables*/
         ctx->rem -= 4;

         ctx->op = E_EEP_INTERNAL_WRITE_OPER_PENDING;
         break;
      }
   }
   return isDone;
}/*Eep_writeProcessing*/

/******************************************************************
 * Function Name: <Eep_readProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Read state handler.
 ****************************************************************/
static boolean Eep_readProcessing(struct Eep_ProcCtx *ctx)
{
   uint32 tempData;
   boolean isDone = FALSE;
   
   switch(ctx->op)
   {
      case E_EEP_INTERNAL_READ_OPER_PENDING:
      {
         if(!(EEP_REG_INFO()->EEDONE & 0X1)) /*The EEPROM is not working. "IDLE". */
         {
            if(!(EEP_REG_INFO()->EESUPP & 0X0C)) /*There is no Errors..*/
            {
               tempData = Eep_internalRead(); /*Read the current word..*/

               if(ctx->rem >= EEP_WORD_SIZE_IN_BYTES)
               {
                  Eep_internalU8Copy((uint8*)&tempData, 
                                     (uint8*)ctx->data,
                                     EEP_WORD_SIZE_IN_BYTES);
                  
                  ++ctx->data; /*Update the internal variables*/
                  ctx->rem -= 4;
               }
               else
               {
                  Eep_internalU8Copy((uint8*)&tempData,
                                     (uint8*)ctx->data,
                                     ctx->rem);
                  ctx->rem = 0; /*This is the last sub-word..*/
               }
               
               if(ctx->rem > 0) /*If we have more data.*/
               {
                  Eep_updateBlockIdx(); /*Update the block index if needed.*/
               }
               else
               {
                  ctx->op = E_EEP_INTERNAL_IDLE_OPER; /*Update internal supOperation state.*/
                  ctx->result = MEMIF_JOB_OK; /*job result is good..*/
                  isDone = TRUE; /*The current job is done ..*/
               }
            }
            else
            {
               ctx->op = E_EEP_INTERNAL_IDLE_OPER; /*Update internal supOperation state.*/
               ctx->result = MEMIF_JOB_FAILED; 
               isDone = TRUE; /*The current job is done ..*/
            }
         }
         break;
      }
      case E_EEP_INTERNAL_IDLE_OPER:
      default:
      {
         Eep_setupBlockIdx(ctx->address); /*Select the correct block..*/

         ctx->op = E_EEP_INTERNAL_READ_OPER_PENDING;
         break;
      }
   }
   
   return isDone;
}/*Eep_readProcessing*/

/******************************************************************
 * Function Name: <Eep_eraseProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Erase state handler.
 ****************************************************************/
static boolean Eep_eraseProcessing(struct Eep_ProcCtx *ctx)
{
   return TRUE; /*Do nothing !..*/
}/*Eep_eraseProcessing*/

/******************************************************************
 * Function Name: <Eep_compareProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Compare state handler.
 ****************************************************************/
static boolean Eep_compareProcessing(struct Eep_ProcCtx *ctx)
{
   static Eep_ProcCtxType localCtx = {0, 0, NULL_PTR, E_EEP_INTERNAL_IDLE_OPER , MEMIF_JOB_OK , 0};
   boolean isDone = FALSE;
   uint8 cmbSize = EEP_WORD_SIZE_IN_BYTES;

   switch(ctx->op)
   {
      case E_EEP_INTERNAL_COMPARE_OPER_PENDING:
      {
         if(TRUE == Eep_readProcessing(&localCtx))
         {
            if(ctx->rem >= EEP_WORD_SIZE_IN_BYTES)
            {  
               ctx->rem -= 4;
            }
            else
            {
               cmbSize = ctx->rem;
               ctx->rem = 0; /*This is the last sub-word..*/
            }
            if(TRUE == Eep_internalU8Compare((uint8*)&localCtx.cmpBuffer,
                                             (uint8*)ctx->data,
                                             cmbSize))
            {
               ++ctx->data; /*Increment the data pointer..*/
               localCtx.address += 4;
               
               if(0 < ctx->rem)
               {
                  localCtx.data = &localCtx.cmpBuffer;
                  localCtx.rem = ctx->rem >= EEP_WORD_SIZE_IN_BYTES ? EEP_WORD_SIZE_IN_BYTES : ctx->rem;

                  (void)Eep_readProcessing(&localCtx);   /*Excute the internal read..*/
               }
               else
               {
                  ctx->result = MEMIF_JOB_OK;
                  ctx->op = E_EEP_INTERNAL_IDLE_OPER;  
                  isDone = TRUE;
               }
            }
            else
            {
               ctx->result = MEMIF_JOB_FAILED;
               ctx->op = E_EEP_INTERNAL_IDLE_OPER;  
               isDone = TRUE;
            }
         }
         break;
      }
      case E_EEP_INTERNAL_IDLE_OPER:
      default:
      {
         localCtx = *ctx;
         localCtx.data = &localCtx.cmpBuffer;
         localCtx.rem = ctx->rem >= EEP_WORD_SIZE_IN_BYTES ? EEP_WORD_SIZE_IN_BYTES : ctx->rem;

         (void)Eep_readProcessing(&localCtx);   /*Excute the internal read..*/

         ctx->op = E_EEP_INTERNAL_COMPARE_OPER_PENDING;
         break;
      }
   }
   return isDone;
}/*Eep_compareProcessing*/

/******************************************************************
 * Function Name: <Eep_internalProcessing>
 * Param (IN) : struct Eep_ProcCtx *
 * Param (OUT): None
 * Return:      boolean
 * Description: Finalize the cancelled operation.
 ****************************************************************/
static boolean Eep_internalProcessing(struct Eep_ProcCtx *ctx)
{
   boolean isDone = FALSE;

   /**
    * @brief TODO: COMPELTE THE IMPLEMENTATION.
    */
   if(!(EEP_REG_INFO()->EEDONE & 0X1)) /*Wait ?*/
   {
      isDone = TRUE;
   }

   return isDone;
}/*Eep_internalProcessing*/

/*****************************************************************************
 * Global Functions.
 ******************************************************************************/

/******************************************************************
 * Function Name: <Eep_Init>
 * Param (IN) : const Eep_ConfigType* ConfigPtr
 * Param (OUT): None
 * Return:      None
 * Description: Service for EEPROM initialization. 
 ****************************************************************/
void Eep_Init(const Eep_ConfigType* ConfigPtr)
{
   SYS_NOOP_CYCLES();

   while(EEP_REG_INFO()->EEDONE & 1); /*Wait for the EEP to complete its power-on initialization.*/

   if(!(EEP_REG_INFO()->EESUPP & 0xC)) /*If either of the bits are set, return an error, else continue*/
   {
      SYS_EEP_HOLD_ON_RESET(); /*Reset the EEP module.*/

      SYS_EEP_RELEASE_RESET(); /*Reset the EEP module.*/

      SYS_NOOP_CYCLES();

      while(EEP_REG_INFO()->EEDONE & 1); /*Wait for the EEP to complete its power-on initialization.*/

      if((!(EEP_REG_INFO()->EESUPP & 0xC))&& /*The EEP initialization is complete and software may use the peripheral as normal.*/
         (NULL_PTR != ConfigPtr)) /*If we have a valid parameter..*/
      {
         Eep_globalVar.ConfigPtr = ConfigPtr; /*Hold the most updated configuration variant.*/
         Eep_globalVar.internalProcessing = &Eep_idleProcessing;
         Eep_globalVar.state = MEMIF_IDLE; /*Now we could access the EEP module.*/
      }
   }
   /*else*/
   /*If the PRETRY or ERETRY bits are set in the ESUPP register, the EEPROM was unable
     to recover its state. If power is stable when this occurs, this indicates a fatal error and
     is likely an indication that the EEPROM memory has exceeded its specified lifetime
     write/erase specification. If the supply voltage is unstable when this return code is
     observed, retrying the operation once the voltage is stabilized may clear the error.
   */
}/*Eep_Init*/

/******************************************************************
 * Function Name: <Eep_Read>
 * Param (IN) : Eep_AddressType EepromAddress, 
 *              uint8* DataBufferPtr, 
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Reads from EEPROM. 
 ****************************************************************/
Std_ReturnType Eep_Read(Eep_AddressType EepromAddress, 
                        uint8* DataBufferPtr, 
                        Eep_LengthType Length)
{
   Std_ReturnType result = E_OK;

   if(MEMIF_IDLE == Eep_globalVar.state)
   {
#ifdef EEP_DEV_ERROR
      if((EepromAddress % Eep_globalVar.ConfigPtr->pageSize) || /*Check if the passed arguemnts are good.*/
         (Length % Eep_globalVar.ConfigPtr->pageSize) ||
         (NULL_PTR == DataBufferPtr) ||
         ((Length + EepromAddress) > EEP_SIZE_IN_BYTES)
        )
      {
         result = E_NOT_OK;
      }
      else
#endif
      {
         Eep_globalVar.state = MEMIF_BUSY;
         Eep_globalVar.ctx.result = MEMIF_JOB_PENDING;

         /*Marshal arguments.*/
         Eep_globalVar.ctx.address = EepromAddress;
         Eep_globalVar.ctx.rem = Length;
         Eep_globalVar.ctx.data = (uint32*)DataBufferPtr;

         Eep_globalVar.internalProcessing = &Eep_readProcessing; /*To be processed in the main function.*/
      }
   }

   return result;
}/*Eep_Read*/

/******************************************************************
 * Function Name: <Eep_Write>
 * Param (IN) : Eep_AddressType EepromAddress,  
 *              const uint8* DataBufferPtr
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Writes to EEPROM.. 
 ****************************************************************/
Std_ReturnType Eep_Write(Eep_AddressType EepromAddress, 
                         const uint8* DataBufferPtr, 
                         Eep_LengthType Length)
{
   Std_ReturnType result = E_OK;
   
   if(MEMIF_IDLE == Eep_globalVar.state) /*Check if we are Idle.*/
   {
#ifdef EEP_DEV_ERROR
      if((EepromAddress % Eep_globalVar.ConfigPtr->pageSize) || /*Check if the passed arguemnts are good.*/
         (Length % Eep_globalVar.ConfigPtr->pageSize) ||
         (NULL_PTR == DataBufferPtr)||
         ((Length + EepromAddress) > EEP_SIZE_IN_BYTES)
        )
      {
         result = E_NOT_OK;
      }
      else
#endif
      {
         Eep_globalVar.state = MEMIF_BUSY;
         Eep_globalVar.ctx.result = MEMIF_JOB_PENDING;

         /*Marshal arguments.*/
         Eep_globalVar.ctx.address = EepromAddress;
         Eep_globalVar.ctx.rem = Length;
         Eep_globalVar.ctx.data = (uint32*)DataBufferPtr;

         Eep_globalVar.internalProcessing = &Eep_writeProcessing; /*To be processed in the main function.*/
      }
   }

   return result;
}/*Eep_Write*/

/******************************************************************
 * Function Name: <Eep_Erase>
 * Param (IN) : Eep_AddressType EepromAddress,
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Service for erasing EEPROM sections.
 ****************************************************************/
Std_ReturnType Eep_Erase(Eep_AddressType EepromAddress, 
                         Eep_LengthType Length)
{
   Std_ReturnType result = E_OK;
   
   if(MEMIF_IDLE == Eep_globalVar.state) /*Check if we are Idle.*/
   {
#ifdef EEP_DEV_ERROR
      if((EepromAddress % Eep_globalVar.ConfigPtr->sectorSize) || /*Check if the passed arguemnts are good.*/
         (Length % Eep_globalVar.ConfigPtr->sectorSize) ||
         ((Length + EepromAddress) > EEP_SIZE_IN_BYTES)
        )
      {
         result = E_NOT_OK;
      }
      else
#endif
      {
         Eep_globalVar.state = MEMIF_BUSY;
         Eep_globalVar.ctx.result = MEMIF_JOB_PENDING;

         /*Marshal arguments.*/
         Eep_globalVar.ctx.address = EepromAddress;
         Eep_globalVar.ctx.rem = Length;

         Eep_globalVar.internalProcessing = &Eep_eraseProcessing; /*To be processed in the main function.*/
      }
   }

   return result;
}/*Eep_Erase*/

/******************************************************************
 * Function Name: <Eep_Compare>
 * Param (IN) : Eep_AddressType EepromAddress,  
 *              const uint8* DataBufferPtr
 *              Eep_LengthType Length
 * Param (OUT): None
 * Return:      Std_ReturnType
 * Description: Compares a data block in EEPROM with an 
 *              EEPROM block in the memory.
 ****************************************************************/
Std_ReturnType Eep_Compare(Eep_AddressType EepromAddress, 
                           const uint8* DataBufferPtr, 
                           Eep_LengthType Length)
{
   Std_ReturnType result = E_OK;

   if(MEMIF_IDLE == Eep_globalVar.state)
   {
#ifdef EEP_DEV_ERROR
      if((EepromAddress % Eep_globalVar.ConfigPtr->pageSize) || /*Check if the passed arguemnts are good.*/
         (Length % Eep_globalVar.ConfigPtr->pageSize) ||
         (NULL_PTR == DataBufferPtr) ||
         ((Length + EepromAddress) > EEP_SIZE_IN_BYTES)
        )
      {
         result = E_NOT_OK;
      }
      else
#endif
      {
         Eep_globalVar.state = MEMIF_BUSY;
         Eep_globalVar.ctx.result = MEMIF_JOB_PENDING;

         /*Marshal arguments.*/
         Eep_globalVar.ctx.address = EepromAddress;
         Eep_globalVar.ctx.rem = Length;
         Eep_globalVar.ctx.data = (uint32*)DataBufferPtr;

         Eep_globalVar.internalProcessing = &Eep_compareProcessing; /*To be processed in the main function.*/
      }
   }

   return result;
}/*Eep_Compare*/

/******************************************************************
 * Function Name: <Eep_Cancel>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Cancels a running job.
 ****************************************************************/
void Eep_Cancel(void)
{
   Eep_globalVar.state = MEMIF_BUSY_INTERNAL; /*The job cancellation may take some time. "So may be we need to ignore the incomming request during this time".*/
   Eep_globalVar.ctx.result = MEMIF_JOB_CANCELLED; /*Mark the job result as cancelled..*/

   Eep_globalVar.internalProcessing = &Eep_internalProcessing; /*To be processed in the main function.*/
}/*Eep_Cancel*/

/******************************************************************
 * Function Name: <Eep_GetStatus>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      MemIf_StatusType
 * Description: Returns the EEPROM state.
 ****************************************************************/
MemIf_StatusType Eep_GetStatus(void)
{
   return Eep_globalVar.state;
}/*Eep_GetStatus*/

/******************************************************************
 * Function Name: <Eep_GetJobResult>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      MemIf_JobResultType
 * Description: This service returns the result of the last job.
 ****************************************************************/
MemIf_JobResultType Eep_GetJobResult(void)
{
   return Eep_globalVar.ctx.result;
}/*Eep_GetJobResult*/

/******************************************************************
 * Function Name: <Eep_MainFunction>
 * Param (IN) : None
 * Param (OUT): None
 * Return:      None
 * Description: Service to perform the processing of the EEPROM jobs 
 *              (read/write/erase/compare).
 * 
 * Remark:   After a job cancelling, main function waits 
 *           until the EEPROM hardware becomes available 
 *           to perform the new job request).
 ****************************************************************/
void Eep_MainFunction(void)
{
   if(TRUE == Eep_globalVar.internalProcessing(&Eep_globalVar.ctx)) /*Execute the pending operation..*/
   {
      Eep_globalVar.internalProcessing = &Eep_idleProcessing; /*Go to the Idle state..*/
      Eep_globalVar.state = MEMIF_IDLE;
   }
}/*Eep_MainFunction*/

#define EEP_MEMMAP_STOP_SEC_CODE
#include "Eep_MemMap.h"
