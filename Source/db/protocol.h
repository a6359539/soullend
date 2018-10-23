#include <Windows.h>
#include <tchar.h>
#include <vector>

#define MAX_BUFF_SIZE   4000
#define MAX_PACKET_SIZE  255

#define BOARD_WIDTH   400
#define BOARD_HEIGHT  400

#define VIEW_RADIUS   7
#define MAX_USER 10

#define NPC_START  500
#define NUM_OF_NPC  200

#define MY_SERVER_PORT  4000

#define MAX_STR_SIZE  100




#define CS_DOWN  2
#define CS_LEFT  3
#define CS_RIGHT    4
#define CS_CHAT		5
#define CS_TRYLOGIN 1
#define SC_TRYLOGIN 1



#define CS_ATTACK		6
#define CS_IDSERCH  7
#define CS_RESTART	8	
#define SC_POS           1
#define SC_PUT_PLAYER    2
#define SC_REMOVE_PLAYER 3
#define SC_CHAT		4
#define SC_ATTACK		5
#define SC_HIT		6
#define SC_IDSERCH  7
#define CSSC_IPPORT 2
#define CSSC_LISTENSERVERIP 3
#define CS_SAVECHAR 4
#define SC_LOADCHAR 4
#define CS_SAVECHARsc 5
#define SC_LOADCHARsc 5
#define CS_TRYNICKNAME 6
#define SC_AFTERNICKNAME 6

using namespace std;
#pragma pack (push, 1)

struct inventorystruct {
	WORD itemnum;
	WORD amount;
	BYTE enchant;
};
struct weaponstruct {
	BYTE weponlevel;
	INT weponexp;
};


struct cs_packet_login {
	WORD size;
	BYTE type;
	WCHAR id[MAX_STR_SIZE];
	WCHAR password[MAX_STR_SIZE];
};
struct sc_packet_loginafter {
	BYTE size;
	BYTE type;
	WORD aftertype;
};

struct cssc_packet_saveloadchar {
	BYTE size;
	BYTE type;
	inventorystruct inventory[20];
	BYTE level;
	INT exp;
	INT x;
	INT y;
	INT z;
};

struct cssc_packet_saveloadcharsc {
	BYTE size;
	BYTE type;
	weaponstruct weapon[9];
	WCHAR nickname[20];
};

struct cs_packet_trycreatenickname{
	BYTE size;
	BYTE type;
	WCHAR nickname[20];
};

struct sc_packet_afternickname {
	BYTE size;
	BYTE type;
	BYTE after;
};


struct cs_packet_connetserver {
	BYTE size;
	BYTE type;
	WORD servertype;
};

struct sc_packet_connetserver {
	BYTE size;
	BYTE type;
	WORD portoffset;
};

struct sc_packet_testconnetserver {
	BYTE size;
	BYTE type;
	CHAR ip[MAX_STR_SIZE];
};

struct cs_simpletry
{
	BYTE size;
	BYTE type;
};

struct cs_packet_IDserch {
	BYTE size;
	BYTE type;
	BYTE id;
};
struct cs_packet_left {
	BYTE size;
	BYTE type;
};

struct cs_packet_right {
	BYTE size;
	BYTE type;
};

struct cs_packet_chat {
	BYTE size;
	BYTE type;
	WCHAR message[MAX_STR_SIZE];
};
struct sc_packet_hit {
	BYTE size;
	BYTE type;
	WORD id;
	INT target;
	BYTE hittype;
};

struct sc_packet_pos {
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
	BYTE direction;
	BYTE anim;
};

struct sc_packet_put_player {
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
	BYTE direction;
	BYTE anim;
	INT hp;
	BYTE level;
	INT exp;
	WCHAR message[MAX_STR_SIZE];
};
struct sc_packet_remove_player {
	BYTE size;
	BYTE type;
	WORD id;
};
struct sc_packet_arrow {
	BYTE size;
	BYTE type;
	WORD id;
	WORD x;
	WORD y;
	BYTE state;
	BYTE arrowdirection;
	INT hp;
};

struct sc_packet_chat {
	BYTE size;
	BYTE type;
	WORD id;
	WCHAR message[MAX_STR_SIZE];
};

#pragma pack (pop)