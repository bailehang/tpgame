#pragma  once

//other define 
#define  SAFE_DELETE(x)			{	if( (x)!= NULL) {	delete (x) ;	(x) = NULL ; } }	
#define	 SAFE_DELETE_ARRAY(x)	{	if( (x)!= NULL) {	delete [](x) ;  (x) = NULL ; } }

//Recv Buffer
#define BUFFER_SIZE				5012
#define SEND_BUFFER_SIZE		32768
#define MAX_SEND_BUFFER_SIZE	65536
#define MASKKEY					0x17    
#define STARTMARK				0x0e000

//Engine Event Packet Protocol

/// 信息反馈
enum	
{
	CLIENT_DISCONNECT	=			1,
	CLIENT_KEEPALIVE,
	CLIENT_GRACEFUL_DISCONNECT,
	COMMAND_CHECK_NETSTATE,
	TEST_MSG_SPEEP,
};

// 连接Uid
#define				CONNECTOR_UID				60000

// 	最大缓存数据包
#define				MAX_SAVECOUNT_OF_RECVPACKET	10000

enum SOCK_TYPE					{ GENERAL_TYPE , SPECIAL_TYPE };
enum OVER_TYPE					{ RECV_OVERLAPPED, SEND_OVERLAPPED, BOTH_OVERLAPPED };
enum OVER_STATE					{ READY,GOOD,BAD };
enum SEND_ERROR_TYPE			{ SUCCESS ,OVER_BUFFER_ERROR , NETWORK_ERROR };
enum							{ WAIT_CONNECTION=0 , ACTIVE_CONNECTION };
enum							{ NO_KEY , MAKE_KEY ,CHANGE_KEY ,OK_KEY };
enum PACKET_SAMPLING_ERROR_CODE {	PACKET_SAMPLING_OK = 0,
									PACKET_STARTMARK_ERROR,
									PACKET_SIZE_ERROR , 
									PACKET_CHECKSUM_ERROR ,
									PACKET_SEQUNCENUM_ERROR,
									PACKET_MASK_ERROR };

enum BUFFER_TYPE	{ B_T_32,B_T_64,B_T_128,B_T_256,B_T_512,B_T_1024,B_T_5120 };

typedef unsigned short WORD;
typedef unsigned long  DWORD;

#define  Min(A , B)	 ( ((A) >(B))?(B):(A) )
#define  Max(A , B)	 ( ((A) <(B))?(B):(A) )