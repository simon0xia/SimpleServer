#ifndef _MESSAGEDEFINE_H
#define _MESSAGEDEFINE_H

enum MessageType
{

// Client -> LoginServer
CL_MsgStart = 10000,
	CL_Heart = CL_MsgStart,
	CL_Login,
	CL_User,
	CL_Password,
CL_MsgEnd,

// Login -> Client
LC_MsgStart = 10500,
	LC_Heart,
	LC_Login,
	LC_User,
	LC_Password,
	LC_Fail,
LC_MsgEnd,

// Client -> Public Server
CP_MsgStart = 11000,
	CP_Heart = CP_MsgStart,
	CP_New,
	CP_Delete,
	CP_Recover,
	CP_Buy,
	CP_Sale,
	CP_Rank,
	CP_Mall,
	CP_Shop,
	CP_Auctions,
CP_MsgEnd,

// Public Server -> Client
PC_MsgStart = 11500,
	PC_Heart,
	PC_OK,
	PC_Fail,
PC_MsgEnd,

// Client -> Game Server
CG_MsgStart = 12000,
CG_MsgEnd,

// Game Server -> Client
GC_MsgStart = 12500,
GC_MsgEnd,

// Login Server -> Public Server
LP_MsgStart = 13000,
	LP_Heart = LP_MsgStart,
	LP_Key,
LP_MsgEnd,

// Public Server -> Login Server
PL_MsgStart = 13500,
	PL_Heart = PL_MsgStart,
PL_MsgEnd,

// Game Server -> Public Server
GP_MsgStart = 14000,
GP_MsgEnd,

// Public Server -> Game Server
PG_MsgStart = 14500,
PG_MsgEnd

};

#endif //#ifndef _MESSAGEDEFINE_H
