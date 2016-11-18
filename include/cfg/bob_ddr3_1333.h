//
//DRAM Timing
//

//DDR3-1333 Micron Part : MT41J1G4-15E
//Clock Rate : 666MHz
#define NUM_RANKS  4
#define NUM_BANKS  8
#define NUM_ROWS  65536
#define NUM_COLS  2048

#define DEVICE_WIDTH  4
#define BL      8 //only used in power calculation

#define Vdd     1.5f

//CLOCK PERIOD
#define tCK     1.5f //ns

//in clock ticks
//ACT to READ or WRITE
#define tRCD    9
//PRE command period
#define tRP     9
//ACT to ACT
#define tRC     33
//ACT to PRE
#define tRAS    24

//CAS latency
#define tCL     9
//CAS Write latency
#define tCWL    7

//ACT to ACT (different banks)
#define tRRD    4
//4 ACT Window
#define tFAW    20
//WRITE recovery
#define tWR     10
//WRITE to READ
#define tWTR    5
//READ to PRE
#define tRTP    5
//CAS to CAS
#define tCCD    4
//REF to ACT
#define tRFC    107
//CMD time
#define tCMDS   1
//Rank to rank switch
#define tRTRS   2 //clk

//IDD Values
#define IDD0    75
#define IDD1    90
#define IDD2P0  12
#define IDD2P1  30
#define IDD2Q   35
#define IDD2N   40
#define IDD2NT  55
#define IDD3P   35
#define IDD3N   45
#define IDD4R   150
#define IDD4W   155
#define IDD5B   230
#define IDD6    12
#define IDD6ET  16
#define IDD7    290
#define IDD8    0
