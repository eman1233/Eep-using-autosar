#ifndef __DEM_H__
#define __DEM_H__
typedef enum{
	DEM_EVENT_STATUS_PASSED=0x00,
	DEM_EVENT_STATUS_FAILED,
	DEM_EVENT_STATUS_PREPASSED,
	DEM_EVENT_STATUS_PREFAILED,
	DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED,
}Dem_EventStatusType;
#endif
