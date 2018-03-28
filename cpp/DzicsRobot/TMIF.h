/// TMIF.h

#pragma once

#include <winsock.h>

#ifdef TMIF_EXPORTS
#define EXP __declspec(dllexport)
#else
#define EXP __declspec(dllimport)
#endif


//Return Code List
typedef enum
{
	RC_OK = 0x0000,				// Operation completed successfully.

	// Command error	
	RC_CMD_FIRST		= 0x0000,
	RC_CMD_PARAMETER	= 0x0001,
	RC_CMD_NO_DATA		= 0x0002,
	RC_CMD_STATE		= 0x0003,
	RC_CMD_NO_STORAGE	= 0x0004,
	RC_CMD_LAST			= 0x00ff,

	// Communication error notice from controller
	RC_NAK_FIRST			= 0x1000,
	RC_NAK_COMMAND,				// Command error
	RC_NAK_COMMAND_LENGTH,		// Command length error
	RC_NAK_TIMEOUT,				// Timeout
	RC_NAK_CHECKSUM,			// Checksum error 
	RC_NAK_INVALID_STATE,		// Status error
	RC_NAK_OTHER,				// Other error
	RC_NAK_LAST				= 0x1fff,

	// Error notice from communication DLL 
	RC_LIB_ERR_FIRST		= 0x2000,
	RC_LIB_ERR_OPEN_DEVICE,		// Opening the device failed
	RC_LIB_ERR_NO_DEVICE,		// The device is not open
	RC_LIB_ERR_SEND,			// Command sending error
	RC_LIB_ERR_RECEIVE,			// Response receiving error
	RC_LIB_ERR_TIMEOUT,			// Timeout
	RC_LIB_ERR_CHECKSUM,		// Checksum error
	RC_LIB_ERR_COMMCODE,		// Command code error
	RC_LIB_ERR_PARAMETER,		// Parameter error
	RC_LIB_ERR_NOMEMORY,		// No free memory
	RC_LIB_LAST				= 0x2fff,
	//TMIFDLL error
	RC_ERR_OPEN_FILE		= 0x3000, // Opening the file failed
	RC_ERR_INVALID_FILE,			  // Incorrect file
	RC_ERR_READ_FILE,				  // Reading from the file failed
	RC_ERR_WRITE_FILE,				  // Writing to the file failed
	// Other error
	RC_OTHER_ERR_FIRST	= 0xF000,
	RC_OTHER_ERR_DISCONNECT,	// Cable disconnection suspected
	RC_OTHER_ERR_FAILED			// Undefined error
} RC;


/// Image 
#define TMIF_IMAGE_WIDTH				(1280)
#define TMIF_IMAGE_HEIGHT				(1024)

/// Time structure
typedef struct {
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
} TMIF_TIME;

/// The target head
typedef enum {
	TMIF_HEAD_A = 0x00,						// Head A
	TMIF_HEAD_B = 0x01,						// Head B
	TMIF_HEAD_BOTH = 0x02,					// Both heads
} TMIF_HEAD;
/// Structure for the master image
typedef struct {
	BYTE byImage[TMIF_IMAGE_WIDTH * TMIF_IMAGE_HEIGHT];
} TMIF_IMAGE;

/// OUT No
typedef enum {
	TMIF_OUTNO_1 = 0x0001,					// OUT1
	TMIF_OUTNO_2 = 0x0002,					// OUT2
	TMIF_OUTNO_3 = 0x0004,					// OUT3
	TMIF_OUTNO_4 = 0x0008,					// OUT4
	TMIF_OUTNO_5 = 0x0010,					// OUT5
	TMIF_OUTNO_6 = 0x0020,					// OUT6
	TMIF_OUTNO_7 = 0x0040,					// OUT7
	TMIF_OUTNO_8 = 0x0080,					// OUT8
	TMIF_OUTNO_9 = 0x0100,					// OUT9
	TMIF_OUTNO_10 = 0x0200,					// OUT10
	TMIF_OUTNO_11 = 0x0400,					// OUT11
	TMIF_OUTNO_12 = 0x0800,					// OUT12
	TMIF_OUTNO_13 = 0x1000,					// OUT13
	TMIF_OUTNO_14 = 0x2000,					// OUT14
	TMIF_OUTNO_15 = 0x4000,					// OUT15
	TMIF_OUTNO_16 = 0x8000,					// OUT16
	TMIF_OUTNO_ALL = 0xFFFF,				// All OUT
} TMIF_OUTNO;

// The screen to display
typedef enum
{
	TMIF_DISPLAY_MEASURE_ALL = 0x00,		// Numeric value/multiple screens
	TMIF_DISPLAY_MEASURE_OUTNO_1, 			// OUT1 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_2, 			// OUT2 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_3, 			// OUT3 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_4, 			// OUT4 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_5, 			// OUT5 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_6, 			// OUT6 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_7, 			// OUT7 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_8, 			// OUT8 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_9, 			// OUT9 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_10, 			// OUT10 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_11, 			// OUT11 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_12, 			// OUT12 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_13, 			// OUT13 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_14, 			// OUT14 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_15, 			// OUT15 in the numeric value/single screen
	TMIF_DISPLAY_MEASURE_OUTNO_16, 			// OUT16 in the numeric value/single screen
	TMIF_DISPLAY_IMAGE_HEAD_A,  			// Image display - Head A  
	TMIF_DISPLAY_IMAGE_HEAD_B,  			// Image display - Head B
	TMIF_DISPLAY_IMAGE_BOTH,				// Image display - Both heads
	TMIF_DISPLAY_TARGET_NUM
} TMIF_DISPLAYTARGET;
/// Measurement value structure
typedef enum {
	TMIF_FLOATRESULT_VALID,					// Valid data
	TMIF_FLOATRESULT_RANGEOVER_P,			// Over range on positive (+) side
	TMIF_FLOATRESULT_RANGEOVER_N,			// Over range on negative (-) side
	TMIF_FLOATRESULT_WAITING,				// Wait for comparator result
} TMIF_FLOATRESULT;

typedef enum {
	TMIF_WAITINGRESULT = 0x0,				// Wait for comparator result
	TMIF_HI = 0x1,							// Comparator result: HI
	TMIF_GO = 0x2,							// Comparator result: GO
	TMIF_LO = 0x4,							// Comparator result: LO
	TMIF_ALARMRESULT = 0x5,					// Alarm value
} TMIF_MEASURERESULT;

typedef struct {
	TMIF_FLOATRESULT FloatResult;			// Whether the data is valid or not
	TMIF_MEASURERESULT MeasureResult;		// GO/HI/LO/Wait for comparator result
	float fValue;							// Measured value for the case of TMIF_FLOATRESULT_VALID; Invalid value for other cases
} TMIF_MEASUREDATA;
/// The area for the obtained image.
typedef struct {
	short nXStart;							// Image area: Start of the X coordinate
	short nXEnd;							// Image area: End of the X coordinate
	short nYStart;							// Image area: Start of the Y coordinate
	short nYEnd;							// Image area: End of the Y coordinate
} TMIF_REQAREA;
/// The image area obtained with the controller
typedef struct {
	short nXStart;							// Image area: Start of the X coordinate
	short nXEnd;							// Image area: End of the X coordinate
	short nYStart;							// Image area: Start of the Y coordinate
	short nYEnd;							// Image area: End of the Y coordinate
	DWORD dwSize;							// No. of obtained pixels
} TMIF_GET_AREA_INFO;
/// The storage status structure
typedef struct {
	BYTE byTarget;							// Storage mode
	BYTE byStat;							// Storage status
	WORD wTargetDtl;						// Detailed storage information
	DWORD dwCnt;
} TMIF_STORAGESTAT;
/// Data storage structure
typedef struct {
	TMIF_FLOATRESULT FloatResult;			// Whether the data is valid or not
	FLOAT fValue;							// Measured value for the case of TMIF_FLOATRESULT_VALID; Invalid value for other cases
} TMIF_DATA_STORAGE;
/// The storage structure
typedef struct {
	BYTE byImageCount;						// No. of images being stored
	BYTE byHead;							// Data being stored
	BYTE byUnit;							// Display unit
	BYTE byRangeX_A;						// Head A: Measurement range (X)
	BYTE byRangeY_A;						// Head A: Measurement range (Y)
	BYTE byRangeX_B;						// Head B: Measurement range (X)
	BYTE byRangeY_B;						// Head B: Measurement range (Y)
	BYTE reserve;
} TMIF_STORAGE_INFO;

/// Parameters when opening the Ethernet device
typedef struct {
	WORD wPort;								// Port number
	WORD wReserve;							// Reserved
	IN_ADDR IPAddress;						// IP address
} TMIF_OPENPARAM_ETHERNET;


/// Command	/////////////////////////////////////////////////////////////////////////
/// Change to measurement mode
EXP RC WINAPI TMIF_StartMeasure(BOOL bNotSaveSetting);
/// Change to communication mode
EXP RC WINAPI TMIF_StopMeasure();
/// Change to storage mode
EXP RC WINAPI TMIF_SetStorageMode();
/// Load program file/master data
EXP RC WINAPI TMIF_LoadProgramFile(IN LPCTSTR pszFilePath);
/// Save program file/master data
EXP RC WINAPI TMIF_SaveProgramFile(IN LPCTSTR pszFileFolder, IN LPCTSTR pszProgramName);
/// Load master image
EXP RC WINAPI TMIF_SetMasterImage(BYTE byPrgNo, TMIF_HEAD Head, TMIF_IMAGE* pMaster);
/// Get master image
EXP RC WINAPI TMIF_GetMasterImage(BYTE byPrgNo, TMIF_HEAD Head, TMIF_IMAGE* pMaster);
/// Load environment setting file
EXP RC WINAPI TMIF_LoadEnvironmentFile(IN LPCTSTR pszFilePath);
/// Save environment setting file
EXP RC WINAPI TMIF_SaveEnvironmentFile(IN LPCTSTR pszFilePath);
/// Get active program No.
EXP RC WINAPI TMIF_GetActivePrgNo(BYTE* pbyPrgNo);
/// Change program
EXP RC WINAPI TMIF_SetActivePrgNo(BYTE byPrgNo);
/// Change to program saved in memory card
EXP RC WINAPI TMIF_ChangeProgramInMemoryCard(BYTE byFileNo, BYTE byPrgNo);
/// Trigger
EXP RC WINAPI TMIF_SetTrigger(TMIF_HEAD Head);
/// Auto-zero
EXP RC WINAPI TMIF_SetAutoZero(BOOL bIsOn, TMIF_OUTNO OutNo);
/// Timing
EXP RC WINAPI TMIF_SetTiming(BOOL bIsOn, BYTE byTimNo, TMIF_OUTNO OutNo);
/// Reset
EXP RC WINAPI TMIF_SetReset();
/// Get measured value
EXP RC WINAPI TMIF_GetMeasureValue(IN TMIF_OUTNO OutNo, OUT TMIF_MEASUREDATA* pMeasureData);
/// Get latest image
EXP RC WINAPI TMIF_GetImage(TMIF_HEAD Head, BYTE byThin, TMIF_REQAREA* pReqArea, TMIF_GET_AREA_INFO* pGetAreaInfo,
	BYTE* pGetImageBuffer, IN TMIF_OUTNO OutNo, TMIF_MEASUREDATA* pMeasureData);
/// Start storage
EXP RC WINAPI TMIF_StartStorage();
/// Stop storage
EXP RC WINAPI TMIF_StopStorage();
/// Get storage status
EXP RC WINAPI TMIF_GetStorageStatus(TMIF_STORAGESTAT* pStat);
/// Clear data storage data
EXP RC WINAPI TMIF_ClearDataStorage();
/// Clear image storage data
EXP RC WINAPI TMIF_ClearImageStorage();
/// Get data storage data
EXP RC WINAPI TMIF_GetDataStorage(IN TMIF_OUTNO OutNo, IN INT nCount, OUT INT *pDataCount,
	OUT TMIF_TIME* pTime, OUT TMIF_DATA_STORAGE* pData);
/// Get image storage data
EXP RC WINAPI TMIF_GetImageStorage(TMIF_HEAD Head, BYTE byNum, BYTE* pImage);
/// Get image storage administration information
EXP RC WINAPI TMIF_GetImageStorageAdmin(TMIF_STORAGE_INFO* pStorageInfo, IN TMIF_OUTNO OutNo,
												IN INT nCount, OUT TMIF_TIME* pTime, OUT TMIF_MEASUREDATA* pData);
/// Save storage data
EXP RC WINAPI TMIF_SaveStorage(IN LPCTSTR pszFilePath);
/// Set key-lock
EXP RC WINAPI TMIF_SetKeyLock(BOOL bIsOn);
/// Change measurement screen
EXP RC WINAPI TMIF_ChangeDisplay(TMIF_DISPLAYTARGET byDispType);
/// Open USB device 
EXP RC WINAPI TMIF_UsbOpen();
/// Open Ether device 
EXP RC WINAPI TMIF_EtherOpen(TMIF_OPENPARAM_ETHERNET* pParam);
/// Close device 
EXP RC WINAPI TMIF_Close();
/// Get DLL version
EXP WORD WINAPI TMIF_GetDllVersion();
