#include "MemMap.h"
#include "Eep.h"
#include "Eep_Cbk.h"

static EepInitConfiguration EepInitConfigurationType={
	.EEP_BASE_ADDRESS =0,
	.EEP_DEFAULT_MODE=MEMIF_MODE_SLOW,
	.EEP_FAST_READ_BLOCK_SIZE=4,
	.EEP_FAST_WRITE_BLOCK_SIZE=4,
	.EEP_JOB_CALL_CYCLE=0,
	.EEP_NORMAL_READ_BLOCK_SIZE=4,
	.EEP_NORMAL_WRITE_BLOCK_SIZE=4,
	.EEP_SIZE=2048,
	.EEP_JOB_ERROR_NOTIFICATION=Eep_JobErrorNotification,
	.EEP_JOB_END_NOTIFICATION=Eep_JobEndNotification,
};
static Eep_ConfigType Eep_Config={
	.EepInitConfigurationRef=&EepInitConfigurationType,
};