/*typedef struct DeviceDesp{
	UINT16  nwkaddr;
	UINT8  	macaddr[8];
	UINT8  	depth;
	UINT8  	devtype;
	UINT16  parentnwkaddr;
}DeviceDesp,*pDeviceDesp;
typedef struct SensorDesp{
	UINT16  nwkaddr;
	UINT8  	sensortype;
	UINT16  sensorvalue;
} SensorDesp,*pSensorDesp;
*/typedef struct NwkDesp{
unsigned int  	panid; 
unsigned long int  	channel;  
unsigned char  	maxchild; 
unsigned char  	maxdepth;
unsigned char  	maxrouter;
 } NwkDesp,*pNwkDesp;
typedef struct{
    unsigned int  nwkaddr;
    unsigned char  	sensortype;
    unsigned long int  sensorvalue;
}SensorDesp,*pSensorDesp;

typedef struct DeviceInfo{
unsigned int  nwkaddr; 
unsigned char  	macaddr[8]; 
unsigned char  	depth; 
unsigned char   devtype; 
unsigned int  parentnwkaddr; 
unsigned char  	sensortype; 
unsigned long int  sensorvalue; 
unsigned char  	status; 
} DeviceInfo,*pDeviceInfo;
typedef struct NodeInfo{
DeviceInfo *devinfo;
struct NodeInfo *next;
unsigned char   row;
unsigned char   num;
}NodeInfo,*pNodeInfo;
typedef struct NodeNwkTopoInfo_message{
        DeviceInfo     devinfo;
        unsigned char   row;
        unsigned char   num;
	unsigned int    flag;
}NodeNwkTopoInfo_message;
//#define FALSE 0
//#define TRUE 1
//typedef enum {FALSE=0, TRUE=1} bool;


typedef struct head{
unsigned char protrol;
unsigned int size;
unsigned char opcode;
unsigned int ack;
}head;
#define MYPROTROL 111
#define ONLINE  0
#define NETINFO 1
#define DEVINFO 2
#define TOPOINFO 3
#define DEVCONTROL 4
#define SENSOR_WORK 5
#define TEMPHUM 6
#define RFIDGET 7
#define GPRSMESG 8
#define INTLOCK 9

#define GET_STATUS 10
#define SET_STATUS 11
#define GPRSSIG 12
