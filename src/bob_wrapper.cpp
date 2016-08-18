/*********************************************************************************
*  Copyright (c) 2010-2011, Elliott Cooper-Balis
*                             Paul Rosenfeld
*                             Bruce Jacob
*                             University of Maryland 
*                             dramninjas [at] gmail [dot] com
*  All rights reserved.
*  
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  
*     * Redistributions of source code must retain the above copyright notice,
*        this list of conditions and the following disclaimer.
*  
*     * Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
*  
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*********************************************************************************/

#include <cstring>
#include "bob_wrapper.h"
#include "bob_logicoperation.h"

using namespace std;

namespace BOBSim
{
BOBWrapper::BOBWrapper(void) :
    returnedReads(0),
    returnedReadSize(0),
    totalReturnedReads(0),
    issuedWrites(0),
    issuedWritesSize(0),
    totalIssuedWrites(0),
    bob(new BOB(this)), //Create BOB object and register callbacks
	readDoneCallback(NULL),
	writeDoneCallback(NULL),
    logicDoneCallback(NULL),
    committedWrites(0),
    issuedLogicOperations(0),
    totalLogicResponses(0),
	totalTransactionsServiced(0),
#if 0
	portRoundRobin(0),
#endif
    currentClockCycle(0)
{
#define TMP_STR_LEN 80

	//Initialize output files
	char *sim_desc;
	char tmp_str[TMP_STR_LEN];
	if ((sim_desc = getenv("SIM_DESC")) != NULL)
	{
        snprintf(tmp_str, TMP_STR_LEN, "BOBstats%s.txt", sim_desc);
		statsOut.open(tmp_str);
        snprintf(tmp_str, TMP_STR_LEN, "BOBpower%s.txt", sim_desc);
		powerOut.open(tmp_str);
	}
	else
	{
        strcpy(tmp_str, "BOBstats.txt");
		statsOut.open(tmp_str);
        strcpy(tmp_str, "BOBpower.txt");
		powerOut.open(tmp_str);
    }

	//Incoming request packet fields (to be added to ports)
    inFlightRequest.Cache = new Transaction*[NUM_PORTS];
    for(unsigned i=0; i<NUM_PORTS; i++) {
        inFlightRequest.Cache[i] = NULL;
    }
    inFlightRequest.Counter = new unsigned[NUM_PORTS];
    memset(inFlightRequest.Counter, 0, sizeof(unsigned)*NUM_PORTS);
    inFlightRequest.HeaderCounter = new unsigned[NUM_PORTS];
    memset(inFlightRequest.HeaderCounter, 0, sizeof(unsigned)*NUM_PORTS);

	//Outgoing response packet fields (being sent back to cache)
    inFlightResponse.Cache = new Transaction*[NUM_PORTS];
    for(unsigned i=0; i<NUM_PORTS; i++) {
        inFlightResponse.Cache[i] = NULL;
    }
    inFlightResponse.Counter = new unsigned[NUM_PORTS];
    memset(inFlightResponse.Counter, 0, sizeof(unsigned)*NUM_PORTS);

	//For statistics & bookkeeping
    requestPortEmptyCount = new unsigned[NUM_PORTS];
    responsePortEmptyCount = new unsigned[NUM_PORTS];
    requestCounterPerPort = new unsigned[NUM_PORTS];
    readsPerPort = new unsigned[NUM_PORTS];
    writesPerPort = new unsigned[NUM_PORTS];
    returnsPerPort = new unsigned[NUM_PORTS];
    memset(requestPortEmptyCount, 0, sizeof(unsigned)*NUM_PORTS);
    memset(responsePortEmptyCount, 0, sizeof(unsigned)*NUM_PORTS);
    memset(requestCounterPerPort, 0, sizeof(unsigned)*NUM_PORTS);
    memset(readsPerPort, 0, sizeof(unsigned)*NUM_PORTS);
    memset(writesPerPort, 0, sizeof(unsigned)*NUM_PORTS);
    memset(returnsPerPort, 0, sizeof(unsigned)*NUM_PORTS);

    memset(perChanReqPort, 0, sizeof(perChanReqPort));
    memset(perChanReqLink, 0, sizeof(perChanReqLink));
    memset(perChanRspPort, 0, sizeof(perChanRspPort));
    memset(perChanRspLink, 0, sizeof(perChanRspLink));
    memset(perChanAccess, 0, sizeof(perChanAccess));
    memset(perChanRRQ, 0, sizeof(perChanRRQ));
    memset(perChanWorkQTimes, 0, sizeof(perChanWorkQTimes));

	//Write configuration stuff to output file
	//
	//System Parameters
    //
	statsOut<<"!!SYSTEM_INI"<<endl;
	statsOut<<"NUM_RANKS="<<NUM_RANKS<<endl;
	statsOut<<"NUM_CHANS="<<NUM_CHANNELS<<endl;
	statsOut<<"BUS_ALIGNMENT_SIZE="<<BUS_ALIGNMENT_SIZE<<endl;
	statsOut<<"JEDEC_DATA_BUS_WIDTH="<<DRAM_BUS_WIDTH<<endl;
	statsOut<<"TRANS_QUEUE_DEPTH="<<PORT_QUEUE_DEPTH<<endl;
	statsOut<<"CMD_QUEUE_DEPTH="<<CHANNEL_WORK_Q_MAX<<endl;
	statsOut<<"USE_LOW_POWER=false"<<endl;
	statsOut<<"EPOCH_COUNT="<<EPOCH_LENGTH<<endl;
	statsOut<<"ROW_BUFFER_POLICY=close_page"<<endl;
	statsOut<<"SCHEDULING_POLICY=N/A"<<endl;
    statsOut<<"ADDRESS_MAPPING_SCHEME=scheme"<<MAPPINGSCHEME<<endl;
	statsOut<<"QUEUING_STRUCTURE=N/A"<<endl;
	statsOut<<"DEBUG_TRANS_Q=false"<<endl;
	statsOut<<"DEBUG_CMD_Q=false"<<endl;
	statsOut<<"DEBUG_ADDR_MAP=false"<<endl;
	statsOut<<"DEBUG_BANKSTATE=false"<<endl;
	statsOut<<"DEBUG_BUS=false"<<endl;
	statsOut<<"DEBUG_BANKS=false"<<endl;
    statsOut<<"DEBUG_POWER=false"<<endl;

	//
	//Device Parameters
	//
	statsOut<<"!!DEVICE_INI"<<endl;
	statsOut<<"NUM_BANKS="<<NUM_BANKS<<endl;
	statsOut<<"NUM_ROWS="<<NUM_ROWS<<endl;
	statsOut<<"NUM_COLS="<<NUM_COLS<<endl;
	statsOut<<"DEVICE_WIDTH="<<DEVICE_WIDTH<<endl;
	statsOut<<"REFRESH_PERIOD=7800"<<endl;
	statsOut<<"tCK="<<tCK<<endl;
	statsOut<<"CL="<<tCL<<endl;
	statsOut<<"AL=0"<<endl;
	statsOut<<"BL="<<BL<<endl;
	statsOut<<"tRAS="<<tRAS<<endl;
	statsOut<<"tRCD="<<tRCD<<endl;
	statsOut<<"tRRD="<<tRRD<<endl;
	statsOut<<"tRC="<<tRC<<endl;
	statsOut<<"tRP="<<tRP<<endl;
	statsOut<<"tCCD="<<tCCD<<endl;
	statsOut<<"tRTP="<<tRTP<<endl;
	statsOut<<"tWTR="<<tWTR<<endl;
	statsOut<<"tWR="<<tWR<<endl;
	statsOut<<"tRTRS=1"<<endl;
	statsOut<<"tRFC="<<tRFC<<endl;
	statsOut<<"tFAW="<<tFAW<<endl;
	statsOut<<"tCKE=0"<<endl;
	statsOut<<"tXP=0"<<endl;
	statsOut<<"tCMD=1"<<endl;
	statsOut<<"IDD0="<<IDD0<<endl;
	statsOut<<"IDD1="<<IDD1<<endl;
	statsOut<<"IDD2P="<<IDD2P1<<endl;
	statsOut<<"IDD2Q="<<IDD2Q<<endl;
	statsOut<<"IDD2N="<<IDD2N<<endl;
	statsOut<<"IDD3Pf="<<IDD3P<<endl;
	statsOut<<"IDD3Ps=0"<<endl;
	statsOut<<"IDD3N="<<IDD3N<<endl;
	statsOut<<"IDD4W="<<IDD4W<<endl;
	statsOut<<"IDD4R="<<IDD4R<<endl;
	statsOut<<"IDD5="<<IDD5B<<endl;
	statsOut<<"IDD6="<<IDD6<<endl;
	statsOut<<"IDD6L="<<IDD6ET<<endl;
	statsOut<<"IDD7="<<IDD7<<endl;

	statsOut<<"!!EPOCH_DATA"<<endl;
}

BOBWrapper::~BOBWrapper(void)
{
  delete[] requestPortEmptyCount;
  delete[] responsePortEmptyCount;
  delete[] requestCounterPerPort;
  delete[] readsPerPort;
  delete[] writesPerPort;
  delete[] returnsPerPort;
  delete[] inFlightRequest.Cache;
  delete[] inFlightRequest.Counter;
  delete[] inFlightRequest.HeaderCounter;
  delete[] inFlightResponse.Cache;
  delete[] inFlightResponse.Counter;
  powerOut.close();
  statsOut.close();
  delete bob;
}

#if 0
inline bool BOBWrapper::isPortAvailable(unsigned port)
{
    return inFlightRequest.Counter[port] == 0 &&
	       bob->ports[port].inputBuffer.size()<PORT_QUEUE_DEPTH;
}


//Uses the port heuristic to determine which port should be used to receive a request
inline int BOBWrapper::FindOpenPort(uint coreID)
{
    switch(PORT_HEURISTIC)
	{
	case FIRST_AVAILABLE:
		for (unsigned i=0; i<NUM_PORTS; i++)
		{
			if (isPortAvailable(i))
			{
				return i;
			}
		}
		break;
	case PER_CORE:
		if(isPortAvailable(coreID%NUM_PORTS))
		{
			return coreID%NUM_PORTS;
		}
		break;
	case ROUND_ROBIN:
		int startIndex = portRoundRobin;
		do
		{
			if(isPortAvailable(portRoundRobin))
			{
				int i=portRoundRobin;
				portRoundRobin=(portRoundRobin+1)%NUM_PORTS;
				return i;
			}
			else
			{
				portRoundRobin=(portRoundRobin+1)%NUM_PORTS;
			}
		}
		while(startIndex!=portRoundRobin);

		break;
	};
	return -1;
}

//MARSS uses this function 
//
//Creates Transaction object and determines which port should receive request
//
bool BOBWrapper::AddTransaction(uint64_t addr, bool isWrite, int coreID, void *logicOperation)
{
	int openPort;
	bool isLogicOp = logicOperation != NULL;
	TransactionType type = isLogicOp ? LOGIC_OPERATION : (isWrite ? DATA_WRITE : DATA_READ) ;
	Transaction *trans;

	if ((openPort = FindOpenPort(coreID)) > -1)
	{
		trans = new Transaction(type, TRANSACTION_SIZE, addr);
		trans->portID = openPort;
		trans->coreID=coreID;
		if (isLogicOp)
		{
			trans->logicOpContents = logicOperation;
		}
		if(currentClockCycle<5000)
		{
            DEBUG("!! to port "<<openPort);
        }

		AddTransaction(trans, openPort);
	}
	else 
	{
		return false;
	}

	return true;
}
#endif

void BOBWrapper::RegisterCallbacks(
    void (*_readDone)(unsigned, uint64_t),
    void (*_writeDone)(unsigned, uint64_t),
    void (*_logicDone)(unsigned, uint64_t))
{
	readDoneCallback = _readDone;
	writeDoneCallback = _writeDone;
	logicDoneCallback = _logicDone;
}

bool BOBWrapper::AddTransaction(Transaction* trans, unsigned port)
{
    if(inFlightRequest.Counter[port]==0 &&
       bob->ports[port].inputBuffer.size()<PORT_QUEUE_DEPTH)
	{
		trans->fullStartTime = currentClockCycle;
		requestCounterPerPort[port]++;

        inFlightRequest.Cache[port] = trans;
        inFlightRequest.Cache[port]->portID = port;
        inFlightRequest.Cache[port]->cyclesReqPort = currentClockCycle;

        inFlightRequest.HeaderCounter[port] = 1;

		switch(trans->transactionType)
		{
		case DATA_READ:
			readsPerPort[port]++;
            inFlightRequest.Counter[port] = 1;
			break;
		case DATA_WRITE:
			issuedWrites++;
            issuedWritesSize += trans->transactionSize;
			writesPerPort[port]++;
            inFlightRequest.Counter[port] = trans->transactionSize / PORT_WIDTH; // trans->transactionSize == TRANSACTION_SIZE
			break;
		case LOGIC_OPERATION:
			issuedLogicOperations++;
            inFlightRequest.Counter[port] = trans->transactionSize / PORT_WIDTH; // trans->transactionSize == TRANSACTION_SIZE
			break;
		default:
			ERROR(" = Error - wrong type");
			ERROR(trans);
			break;
		}

        if(DEBUG_PORTS) DEBUG(" = Putting transaction on port "<<port<<" - for "<<inFlightRequest.Counter[port]<<" CPU cycles");

		return true;
	}
	else
	{
		if(DEBUG_PORTS) DEBUG(" = Port Busy or Full");

		return false; //port busy
	}
}

//
//Updates the state of the memory system
//
void BOBWrapper::Update(void)
{
    bob->Update();

	//BOOK-KEEPING
	for(unsigned i=0; i<NUM_PORTS; i++)
    {
		//
		//Requests
		//
        if(inFlightRequest.HeaderCounter[i]>0 && !--inFlightRequest.HeaderCounter[i]) //done
        {
            if(DEBUG_PORTS)DEBUG("== Header done - Adding to port "<<i);
            bob->ports[i].inputBuffer.push_back(inFlightRequest.Cache[i]);
        }

        if(inFlightRequest.Counter[i]>0)
        {
            if(!--inFlightRequest.Counter[i])
            {
                inFlightRequest.Cache[i]=NULL;
            }
        }
        else //STATS
        {
            requestPortEmptyCount[i]++;
        }

        if(inFlightResponse.Counter[i]>0)
        {
            if(!--inFlightResponse.Counter[i])
            {
                switch(inFlightResponse.Cache[i]->transactionType) {
                case RETURN_DATA:
#ifdef HMCSIM_SUPPORT
                    if (callback)
                    {
                      (*callback)(vault, inFlightResponse.Cache[i]->payload);
                    }
#endif
                    if (readDoneCallback)
                    {
                        (*readDoneCallback)(i, inFlightResponse.Cache[i]->address);
                    }

                    UpdateLatencyStats(inFlightResponse.Cache[i]);

                    returnsPerPort[i]++;
                    break;

                case LOGIC_RESPONSE:
#ifdef HMCSIM_SUPPORT
                    if (callback)
                    {
                      (*callback)(vault, inFlightResponse.Cache[i]->payload);
                    }
#endif
                    if(logicDoneCallback)
                    {
                        (*logicDoneCallback)(inFlightResponse.Cache[i]->mappedChannel, inFlightResponse.Cache[i]->address);
                    }

                    UpdateLatencyStats(inFlightResponse.Cache[i]);

                    totalLogicResponses++;
                    break;

                default:
                    ERROR("== ERROR - unknown packet type coming down ");
                    exit(0);
                }

                bob->ports[i].outputBuffer.erase(bob->ports[i].outputBuffer.begin());
                delete inFlightResponse.Cache[i];
            }
        }
        else //STATS
        {
            responsePortEmptyCount[i]++;
        }
	}

	//NEW STUFF
	for(unsigned i=0; i<NUM_PORTS; i++)
	{
		//look for new stuff to return from bob controller
		if(bob->ports[i].outputBuffer.size()>0 &&
                inFlightResponse.Counter[i]==0)
		{
            inFlightResponse.Cache[i] = bob->ports[i].outputBuffer[0];

            switch(inFlightResponse.Cache[i]->transactionType) {
            case RETURN_DATA:
                inFlightResponse.Counter[i] = inFlightResponse.Cache[i]->transactionSize / PORT_WIDTH; // inFlightResponse.Cache[i]->transactionSize == TRANSACTION_SIZE
                break;
            case LOGIC_RESPONSE:
                inFlightResponse.Counter[i] = 1;
                break;
            default:
                ERROR("== Error - wrong type of transaction out of");
                exit(0);
            }
		}
    }

       if(currentClockCycle%EPOCH_LENGTH==0 && currentClockCycle>0)
       {
               PrintStats(false);
       }


	currentClockCycle++;
}


void BOBWrapper::UpdateLatencyStats(Transaction *returnedRead)
{
    switch(returnedRead->transactionType)
    {
    case LOGIC_RESPONSE:
        DEBUG("  == Got Logic Response");
		DEBUG("  == Logic All Done!");
		DEBUG("  == Took : "<<CPU_CLK_PERIOD*(currentClockCycle - returnedRead->fullStartTime)<<"ns");
        break;
    case RETURN_DATA:
      {
		returnedRead->fullTimeTotal = currentClockCycle - returnedRead->fullStartTime;
		returnedRead->cyclesRspPort = currentClockCycle - returnedRead->cyclesRspPort;

		unsigned chanID = returnedRead->mappedChannel;

		//add latencies to lists
		perChanFullLatencies[chanID].push_back(returnedRead->fullTimeTotal);
        perChanReqPort[chanID] += returnedRead->cyclesReqPort;
        perChanRspPort[chanID] += returnedRead->cyclesRspPort;
        perChanReqLink[chanID] += returnedRead->cyclesReqLink;
        perChanRspLink[chanID] += returnedRead->cyclesRspLink;
        perChanAccess[chanID] += returnedRead->dramTimeTotal;
        perChanRRQ[chanID] += returnedRead->cyclesInReadReturnQ;
        perChanWorkQTimes[chanID] += returnedRead->cyclesInWorkQueue;
		/*
			DEBUGN("== Read Returned");
			DEBUGN(" full: "<< returnedRead->fullTimeTotal * CPU_CLK_PERIOD<<"ns");
			DEBUGN(" dram: "<< returnedRead->dramTimeTotal * CPU_CLK_PERIOD<<"ns");
			DEBUG(" chan: "<< returnedRead->channelTimeTotal * CPU_CLK_PERIOD<<"ns");
		*/
        fullLatencies.push_back(returnedRead->fullTimeTotal);
        dramLatencies.push_back(returnedRead->dramTimeTotal);
        chanLatencies.push_back(returnedRead->channelTimeTotal);

		//latencies[((returnedRead->fullTimeTotal/10)*10)*CPU_CLK_PERIOD]++;

		returnedReads++;
        returnedReadSize += returnedRead->transactionSize;
      }
        break;
    default:
        break;
    }
}

//
//Prints all statistics on epoch boundaries
//
void BOBWrapper::PrintStats(bool finalPrint)
{
#ifndef NO_OUTPUT
    unsigned fullSum = 0;
    unsigned dramSum = 0;
    unsigned chanSum = 0;
    for(unsigned i=0; i<fullLatencies.size(); i++)
    {
      fullSum += fullLatencies[i];
      dramSum += dramLatencies[i];
      chanSum += chanLatencies[i];
    }
	float fullMean = (float)fullSum / returnedReads;
	float dramMean = (float)dramSum / returnedReads;
	float chanMean = (float)chanSum / returnedReads;

    //calculate standard deviation
	double fullstdsum = 0;
	double dramstdsum = 0;
    double chanstdsum = 0;
	unsigned fullLatMax = 0;
	unsigned fullLatMin = -1;//max unsigned value
	unsigned dramLatMax = 0;
	unsigned dramLatMin = -1;
	unsigned chanLatMax = 0;
	unsigned chanLatMin = -1;
	for(unsigned i=0; i<fullLatencies.size(); i++)
	{
		fullLatMax = max(fullLatencies[i],fullLatMax);
        fullLatMin = min(fullLatencies[i], fullLatMin);
        fullstdsum+=pow(fullLatencies[i]-fullMean,2);

		if(dramLatencies[i]>dramLatMax) dramLatMax = dramLatencies[i];
        if(dramLatencies[i]<dramLatMin) dramLatMin = dramLatencies[i];
        dramstdsum+=pow(dramLatencies[i]-dramMean,2);

		if(chanLatencies[i]>chanLatMax) chanLatMax = chanLatencies[i];
		if(chanLatencies[i]<chanLatMin) chanLatMin = chanLatencies[i];
        chanstdsum+=pow(chanLatencies[i]-chanMean,2);
    }
	double fullstddev = sqrt(fullstdsum/returnedReads);
	double dramstddev = sqrt(dramstdsum/returnedReads);
	double chanstddev = sqrt(chanstdsum/returnedReads);
#endif

    unsigned elapsedCycles = currentClockCycle%EPOCH_LENGTH;
    if(elapsedCycles==0)
    {
        elapsedCycles = EPOCH_LENGTH;
    }

    double clockperiod_ns = CPU_CLK_PERIOD * 1E-9 /* ns */;
//    double frequency_GHz = 1/(clockperiod_ns / 1E-9) /* GHz */;
    double transferamount_Bytes = issuedWritesSize + returnedReadSize; //(issuedWrites+returnedReads)*TRANSACTION_SIZE;
    double one_GB = (1<<30);

    double bandwidth = transferamount_Bytes/(double)(elapsedCycles*clockperiod_ns)/one_GB;

	double rw_ratio = ((double)returnedReads/(double)committedWrites)*100.0f;

	statsOut  <<currentClockCycle*CPU_CLK_PERIOD/1000000<< ","<< setprecision(4) << bandwidth << "," << fullMean*CPU_CLK_PERIOD<<","<<rw_ratio<<",;";

	totalTransactionsServiced += (issuedWrites + returnedReads);
	totalReturnedReads += returnedReads;
	totalIssuedWrites += issuedWrites;

	PRINT("==================Epoch ["<<currentClockCycle/EPOCH_LENGTH<<"]=======================");
	PRINT("per epoch] reads   : "<<returnedReads<<" writes : "<<issuedWrites<<" ("<<(float)returnedReads/(float)(returnedReads+issuedWrites)*100<<"% Reads) : "<<returnedReads+issuedWrites);
	PRINT("lifetime ] reads   : "<<totalReturnedReads<<" writes : "<<totalIssuedWrites<<" total : "<<totalTransactionsServiced);
	PRINT("issued logic ops   : "<<issuedLogicOperations<< " returned logic responses : "<<totalLogicResponses);
	PRINT("bandwidth          : "<<bandwidth<<" GB/sec");
	PRINT("current cycle      : "<<currentClockCycle);
	PRINT("--------------------------");
	PRINT("full time mean  : "<<fullMean*CPU_CLK_PERIOD<<" ns");
	PRINT("          std   : "<<fullstddev*CPU_CLK_PERIOD<<" ns");
	PRINT("          min   : "<<fullLatMin*CPU_CLK_PERIOD<<" ns");
	PRINT("          max   : "<<fullLatMax*CPU_CLK_PERIOD<<" ns");
	PRINT("chan time mean  : "<<chanMean*CPU_CLK_PERIOD<<" ns");
	PRINT("           std  : "<<chanstddev*CPU_CLK_PERIOD);
	PRINT("           min  : "<<chanLatMin*CPU_CLK_PERIOD<<" ns");
	PRINT("           max  : "<<chanLatMax*CPU_CLK_PERIOD<<" ns");
	PRINT("dram time mean  : "<<dramMean*CPU_CLK_PERIOD<<" ns");
	PRINT("          std   : "<<dramstddev*CPU_CLK_PERIOD<<" ns");
	PRINT("          min   : "<<dramLatMin*CPU_CLK_PERIOD<<" ns");
	PRINT("          max   : "<<dramLatMax*CPU_CLK_PERIOD<<" ns");
    PRINT("-- Per Channel Latency Components in nanoseconds (All from READs) : ");
	PRINT("      reqPort    reqLink     workQ    access     rrq    rspLink   rspPort  total");
	for(unsigned i=0; i<NUM_CHANNELS; i++)
	{
		float tempSum=0;
		unsigned tempMax=0;
		unsigned tempMin=-1;
		double stdsum=0;
        double mean=0;

		for(unsigned j=0; j<perChanFullLatencies[i].size(); j++)
		{
			if(perChanFullLatencies[i][j]>tempMax) tempMax = perChanFullLatencies[i][j];
			if(perChanFullLatencies[i][j]<tempMin) tempMin = perChanFullLatencies[i][j];

            tempSum+=perChanFullLatencies[i][j];
		}

		mean=tempSum/(float)perChanFullLatencies[i].size();

		//write per channel latency to stats file
		statsOut<<CPU_CLK_PERIOD*mean<<",";

		for(unsigned j=0; j<perChanFullLatencies[i].size(); j++)
		{
			stdsum+=pow(perChanFullLatencies[i][j]-mean,2);
		}

#define MAX_STR 512
		char tmp_str[MAX_STR];

        snprintf(tmp_str,MAX_STR,"%u]%10.4f%10.4f%10.4f%10.4f%10.4f%10.4f%10.4f%10.4f\n",
		         i,
                 CPU_CLK_PERIOD*(perChanReqPort[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanReqLink[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanWorkQTimes[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanAccess[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanRRQ[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanRspLink[i]/perChanFullLatencies[i].size()),
                 CPU_CLK_PERIOD*(perChanRspPort[i]/perChanFullLatencies[i].size()),
		         CPU_CLK_PERIOD*(mean));

		PRINTN(tmp_str);
		//clear
        perChanWorkQTimes[i] = 0;
        perChanFullLatencies[i].clear();
        perChanReqLink[i] = 0;
        perChanRspLink[i] = 0;
        perChanReqPort[i] = 0;
        perChanRspPort[i] = 0;
        perChanAccess[i] = 0;
        perChanRRQ[i] = 0;
	}

	PRINT(" ---  Port stats (per epoch) : ");
	for(unsigned i=0; i<NUM_PORTS; i++)
	{
		PRINTN(" "<<i<<"] ");
		PRINTN(" request: "<<(float)requestPortEmptyCount[i]/(elapsedCycles)*100.0<<"\% idle ");
		PRINTN(" response: "<<(float)responsePortEmptyCount[i]/(elapsedCycles)*100.0<<"\% idle ");
		PRINTN(" rds:"<<readsPerPort[i]);
		PRINTN(" wrts:"<<writesPerPort[i]);
		PRINTN(" rtn:"<<returnsPerPort[i]);
		PRINT(" tot:"<<requestCounterPerPort[i]);

		//clear
		returnsPerPort[i]=0;
		writesPerPort[i]=0;
		readsPerPort[i]=0;
		responsePortEmptyCount[i]=0;
		requestPortEmptyCount[i]=0;
		requestCounterPerPort[i]=0;
	}

	issuedWrites = 0;
    issuedWritesSize = 0;
    returnedReads = 0;
    returnedReadSize = 0;

	fullLatencies.clear();
	dramLatencies.clear();
	chanLatencies.clear();
	bob->PrintStats(statsOut, powerOut, finalPrint, elapsedCycles);
}

void BOBWrapper::WriteIssuedCallback(unsigned port, uint64_t address)
{
	committedWrites++;
	if (writeDoneCallback)
	{
		(*writeDoneCallback)(port,address);
	}
}


} //namespace BOBSim
