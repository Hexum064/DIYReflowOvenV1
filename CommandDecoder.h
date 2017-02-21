/*
 * CommandDecoder.h
 *
 * Created: 2017-01-11 22:22:59
 *  Author: Branden
 */ 


#ifndef COMMANDDECODER_H_
#define COMMANDDECODER_H_

extern	unsigned char SPMCODE_asm (void);

//AT COMMANDS
#define LOG_ON_CMD "LOGON"
#define LOG_OFF_CMD "LOGOFF"
#define TEMP_CMD "TEMP"
#define TIME_CMD "TIME"
#define HEAT_T_ON_CMD "HTON"
#define HEAT_T_OFF_CMD "HTOFF"
#define HEAT_B_ON_CMD "HBON"
#define HEAT_B_OFF_CMD "HBOFF"
#define HEAT_T_CMD "HT"
#define HEAT_B_CMD "HB"
#define FAN_CMD "FAN"
#define FAN_ON_CMD "FANON"
#define FAN_OFF_CMD "FANOFF"
#define BACKLIGHT_CMD "BL"
#define BACKLIGHT_ON_CMD "BLON"
#define BACKLIGHT_OFF_CMD "BLOFF"
#define PROFILE_CMD "PROF"
#define RUN_CMD "RUN"
#define STOP_CMD "STOP"
#define STATUS_CMD "STAT"
#define CAL_HIGH_CMD "CALH"
#define CAL_MID_CMD "CALM"
#define CAL_LOW_CMD "CALL"
#define BL_ON_TIME_CMD "BLONT"
#define PROFILES_CMD "PROFS"
#define PROF_CFG_CMD "PCFG"
#define PROF_DEL_CMD "PDEL"
#define STOP_ALL_CMD "STOPALL"

//TEMP STRINGS
#define STATUS_PRE_MUST "STATUS MUST BE:"
#define STATUS_PRE_NOT "STATUS CANNOT BE:"
#define STOPPED_STATUS "STOPPED"
#define RUNNING_STATUS "RUNNING"
#define TEST1_STATUS "TEST1"
#define TEST2_STATUS "TEST2"

typedef enum 
{
	Waiting,
	A_Received,
	T_Received,
	Query_Received,
	Set_Received
} CmdStateEnum;

void ProcessInput(char c);


#endif /* COMMANDDECODER_H_ */