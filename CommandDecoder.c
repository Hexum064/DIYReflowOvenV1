/*
 * CommandDecoder.c
 *
 * Created: 2017-01-11 20:33:07
 *  Author: Branden
 */ 

#include <stdio.h>
#include <string.h>
#include <nvm.h>
#include "CommandDecoder.h"	 
#include "Oven.h"


volatile CmdStateEnum _cmdState = Waiting;
volatile char _cmdModifier;
volatile char _command[8];
volatile char _args[32];
volatile char _index = 0;

void PrintStatusRequired(StatusEnum requiredStatus)
{
	switch (requiredStatus)
	{
		case Stopped:
		printf("%s %s\r\n", STATUS_PRE_MUST, STOPPED_STATUS);
		break;
		case Running:
		printf("%s %s\r\n", STATUS_PRE_MUST, RUNNING_STATUS);
		break;
		case Test1:
		printf("%s %s\r\n", STATUS_PRE_MUST, TEST1_STATUS);
		break;
		case Test2:
		printf("%s %s\r\n", STATUS_PRE_MUST, Test2);
		break;
	}
	
}

void PrintStatusCannotBe(StatusEnum requiredStatus)
{
	switch (requiredStatus)
	{
		case Stopped:
		printf("%s %s\r\n", STATUS_PRE_NOT, STOPPED_STATUS);
		break;
		case Running:
		printf("%s %s\r\n", STATUS_PRE_NOT, RUNNING_STATUS);
		break;
		case Test1:
		printf("%s %s\r\n", STATUS_PRE_NOT, TEST1_STATUS);
		break;
		case Test2:
		printf("%s %s\r\n", STATUS_PRE_NOT, Test2);
		break;
	}
	
}

void ProcessExcCommand(char command[])
{
	printf("                               \r");
	
	if (strcmp(command, LOG_ON_CMD) == 0)
	{
		if (_ovenStatus != Running)
		{
			PrintStatusRequired(Running);
			return;
		}	
		printf("%.8s\r\n", LOG_ON_CMD);
		_logEnabled = 1;
	}
	else if (strcmp(command, LOG_OFF_CMD) == 0)
	{
		printf("%.8s\r\n", LOG_OFF_CMD);
		_logEnabled = 0;		
	}
	else if (strcmp(command, HEAT_T_ON_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", HEAT_T_ON_CMD);
	}
	else if (strcmp(command, HEAT_T_OFF_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}			
		printf("%.8s\r\n", HEAT_T_OFF_CMD);
	}
	else if (strcmp(command, HEAT_B_ON_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", HEAT_B_ON_CMD);
	}
	else if (strcmp(command, HEAT_B_OFF_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", HEAT_B_OFF_CMD);
	}
	else if (strcmp(command, FAN_ON_CMD) == 0)
	{
		if (_ovenStatus == Running)
		{
			PrintStatusCannotBe(Running);
			return;
		}
		printf("%.8s\r\n", FAN_ON_CMD);
	}
	else if (strcmp(command, BACKLIGHT_ON_CMD) == 0)
		printf("%.8s\r\n", BACKLIGHT_ON_CMD);
	else if (strcmp(command, BACKLIGHT_OFF_CMD) == 0)
		printf("%.8s\r\n", BACKLIGHT_OFF_CMD);
	else if (strcmp(command, RUN_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", RUN_CMD);
	}
	else if (strcmp(command, STOP_CMD) == 0)
	{
		if (_ovenStatus != Running)
		{
			PrintStatusRequired(Running);
			return;
		}		
		printf("%.8s\r\n", STOP_CMD);
	}
	else if (strcmp(command, STOP_ALL_CMD) == 0)
		printf("%.8s\r\n", STOP_ALL_CMD);
	else
		printf("UNKNOWN COMMAND\r\n");

	if (_ovenStatus == Test2)
	printf("Test2\r\n");
}

void ProcessQueryCommand(char command[])
{
	uint8_t buff[3];

	if (strcmp(command, TEMP_CMD) == 0)
	{
		printf("%.8s\r\n", TEMP_CMD);
		nvm_flash_read_buffer(0, buff, 3);
		printf("%.3s\r\n", buff);
	}
	else if (strcmp(command, TIME_CMD) == 0)
		printf("%.8s\r\n", TIME_CMD);
	else if (strcmp(command, HEAT_T_CMD) == 0)
		printf("%.8s\r\n", HEAT_T_CMD);
	else if (strcmp(command, HEAT_B_CMD) == 0)
		printf("%.8s\r\n", HEAT_B_CMD);
	else if (strcmp(command, FAN_CMD) == 0)
		printf("%.8s\r\n", FAN_CMD);
	else if (strcmp(command, BACKLIGHT_CMD) == 0)
		printf("%.8s\r\n", BACKLIGHT_CMD);
	else if (strcmp(command, PROFILE_CMD) == 0)
		printf("%.8s\r\n", PROFILE_CMD);
	else if (strcmp(command, STATUS_CMD) == 0)
		printf("%.8s\r\n", STATUS_CMD);
	else if (strcmp(command, CAL_HIGH_CMD) == 0)
		printf("%.8s\r\n", CAL_HIGH_CMD);
	else if (strcmp(command, CAL_MID_CMD) == 0)
		printf("%.8s\r\n", CAL_MID_CMD);
	else if (strcmp(command, CAL_LOW_CMD) == 0)
		printf("%.8s\r\n", CAL_LOW_CMD);
	else if (strcmp(command, BL_ON_TIME_CMD) == 0)
		printf("%.8s\r\n", BL_ON_TIME_CMD);
	else if (strcmp(command, PROFILES_CMD) == 0)
		printf("%.8s\r\n", PROFILES_CMD);
	else if (strcmp(command, PROF_CFG_CMD) == 0)
		printf("%.8s\r\n", PROF_CFG_CMD);
	else
		printf("UNKNOWN QUERY COMMAND\r\n");													
}

void ProcessSetCommand(char command[], char args[])
{
	printf("                               \r");
	
	if (strcmp(command, TEMP_CMD) == 0)
	{
		//if (_ovenStatus != Stopped)
		//{
			//PrintStatusRequired(Stopped);
			//return;
		//}		
		printf("%.8s = %.32s\r\n", TEMP_CMD, args);
		nvm_flash_erase_and_write_buffer(0, args, 3, false);
	}
	else if (strcmp(command, PROFILE_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}
		printf("%.8s\r\n", PROFILE_CMD);
	}
	else if (strcmp(command, CAL_HIGH_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}
		printf("%.8s\r\n", CAL_HIGH_CMD);
	}
	else if (strcmp(command, CAL_MID_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", CAL_MID_CMD);
	}
	else if (strcmp(command, CAL_LOW_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", CAL_LOW_CMD);
	}
	else if (strcmp(command, BL_ON_TIME_CMD) == 0)
		printf("%.8s\r\n", BL_ON_TIME_CMD);
	else if (strcmp(command, PROF_CFG_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", PROF_CFG_CMD);
	}
	else if (strcmp(command, PROF_DEL_CMD) == 0)
	{
		if (_ovenStatus != Stopped)
		{
			PrintStatusRequired(Stopped);
			return;
		}		
		printf("%.8s\r\n", PROF_DEL_CMD);
	}
	else
		printf("UNKNOWN SET COMMAND\r\n");

}



char ToUpper(char c)
{
	if (c > 96 && c < 123)
	c -= 32;
	
	return c;
}	 

void ProcessInput(char c)	
{
	c = ToUpper(c);
	
	switch (_cmdState)
	{
		case Waiting:
		
			if (c == 'A');
				_cmdState = A_Received;
			break;
		case A_Received:
			if (c == 'T')
			{
				_cmdState = T_Received;
				memset(&_command[0], 0, sizeof(_command));
				memset(&_args[0], 0, sizeof(_args));;				
			}
			else
				_cmdState = Waiting;
			break;
		case T_Received:
			if (c == '?')
			{
				_cmdModifier = '?';
				_cmdState = Query_Received;
			}
			else if (c == '=')
			{
				_index = 0;
				_cmdModifier = '=';
				_cmdState = Set_Received;
			}
			else if (c > 46 && c < 91 && _index < 8)
			{
				_command[_index++] = c;
			}
			else if (c == '\r' && _index > 0)
			{
				_index = 0;
				ProcessExcCommand(_command);
				_cmdState = Waiting;
			}
			
			break;
		case Query_Received:
			if (c == '\r')
			{
				_index = 0;
				ProcessQueryCommand(_command);
				_cmdState = Waiting;
			}
			break;
		case Set_Received:
			if (c != '\r' && _index < 32)
			{
				_args[_index++] = c;
			}
			else if (c == '\r')
			{
				_index = 0;
				ProcessSetCommand(_command, _args);
			
				_cmdState = Waiting;
			}	
			break;	
	}
	
	
}



