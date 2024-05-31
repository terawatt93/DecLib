#include <sstream>
#include <iostream>
#include <TObject.h>
#include <TRandom.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TF1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TObject.h>
#include <thread>  
#include <zlib.h>
#include <TGraph.h>
#include <list>
#include <TClass.h> 
#include <TSystem.h>   
#include <map>  
#include <mutex>  
#include <TROOT.h>
#include <string>
#include <vector>
#include <list>
#pragma once
using namespace std;

#define MAXCH 128

class Event;
class DecFile;
class DecManager;

class HistManager
{
	private: 
		HistManager () {}
		HistManager(const HistManager&);
		HistManager& operator = (HistManager&);
	public:
	static HistManager& Instance()
	{
		static HistManager  instance;
		return instance;
	}
	public:
	mutex lock;
	vector<vector<TH1*> > Objects;
	vector<TObject*> MasterObjects;
	vector<bool> Merged;
	vector<bool> MergingNow;
	bool MergingDone=false;
	vector<thread::id> id_values;
	TH1F *BookTH1F(int thread_id,const char* Name,const char* Title,int NBins, double Min, double Max);
	TH1D *BookTH1D(int thread_id,const char* Name,const char* Title,int NBins, double Min, double Max);
	TH2F *BookTH2F(int thread_id,const char* Name,const char* Title,int NBinsX, double MinX, double MaxX,int NBinsY, double MinY, double MaxY);
	int GetThisThreadNumber();
	void Merge();
	void SaveToFile(TFile *f);
};


class Counter
{
	public:
	vector<list<Long64_t> > Tstmp;
	vector<list<Long64_t> > HWCounts;
	void AddCount(int Ch,Long64_t _Tstmp,Long64_t Count);
	vector<TGraph> ToTGraphs();
};

class DataBuffer
{
	public:
	int BufferLength=0;//длина буфера
	ULong64_t* Buf=0;
	ULong64_t NEvents=0;
	ULong64_t NReadEvents=0;
	int LastWord=0;
	int LastRead=0;
	int FirstEventWord=-1;
	int ThreadID=0;
	float AverageSize=0;
	bool FileEnded=false;
	const Long64_t sixbytes=0xFFFFFFFFFFFF;
	DataBuffer(int _BufferLength=0);
	void SetSize(int size);
	void Reset();
	void Read(gzFile ff,ULong64_t &PrevWord);
	bool GetNextEvent(Event *ev);
	UChar_t PrevState=0;
	bool Active=false;
	bool ReadyForRead=false;
	DecFile *fFile;//!
	DecManager *fDecManager;//!
	int ID=0;
};

class Channel
{
	public:
	string Name;
	bool StartFlag, DSPFlag;
	int Number;
	Channel() { }
	Channel(int _Number,string _Name, bool _Start, bool _Dsp) 
	{
		Number=_Number;
		Name=_Name;
		StartFlag=_Start;
		DSPFlag=_Dsp;
	}
	string GetString(string option="full");
};

/*class RomanaOptions
{
	public:
	TDatime StartTime;
	TDatime StopTime;
	float MeasurementDuration, PeriodOfDigitizing;
	vector<Channel> Channels;
	vector<string> PossibleChNames;
	unsigned int NChannelsInSystem;
	int ReadFromDec(char* buf, char* buf2);
	string GetString();
};*/

class RomanaOptions
{
	public:
	void ReadFromBuffer(char *buf,int size);
};

class Pulse:public TObject
{
	public:
	double Area;
	double Time;
	double Width;
	double Height;
	unsigned int Chan;
	Event *fEvent;//!
	ClassDef(Pulse,1);
};

class Event:public TObject
{
	public:
	vector<Pulse> Pulses;
	vector<Pulse*> SortedPulses;
	Long64_t Tstmp;
	int NPulses=0;
	bool Erase=false;
	void Sort();
	Pulse* GetPulse(unsigned char Chan);
	vector<Pulse*> GetPulses();
	DecFile *fFile;//!
	bool Good=true;
	UChar_t State;
	ClassDef(Event,1);
};

class DecFile:public TObject
{
	public:
	UShort_t fmt; 
	UShort_t mod;
	string Name;
	gzFile ff;
	string ParString;
	bool Opened=false;
	bool IsOpened();
	bool Finished=false;
	bool IsFinished();
	int NThreads=1;
	void GetNextEvent(Event *event, int ThreadID=1);
	RomanaOptions ROptions;
	UShort_t FileType;
	int AssignedThreadNumber=0;
	Long64_t FirstTstmp=0, LastTstmp=0;
	ULong64_t NEvents=0;
	bool Open(string _Name="");
	vector<Long64_t> Counts;
	Counter CountObject;
	ClassDef(DecFile,1);
};


class DecManager:public TObject
{
	private: 
		DecManager () {}
		DecManager(const DecManager&);
		DecManager& operator = (DecManager&);
	public:
	static DecManager& Instance()
	{
		static DecManager  instance;
		return instance;
	}
	//как это должно работать: есть вектор из буферов, который больше, чем количество потоков. Когда запускаются потоки, каждому из них присваивается номер, соответствующий номеру элемента в векторе активных буферов. При чтении каждого события методом GetNextEvent или GetVectorOfEvents проверяется, не является ли событие в буфере последним. Если является, то происходит поиск нового полного незанятого буфера, и на него заменяется указатель. Прочитанный буфер "сбрасывается" и заполняется заново, для чего создается поток, проверяющий наличие прочитанных буферов и читающий их.
	vector<DataBuffer> Buffers;
	vector<DataBuffer*> ActiveBuffers;//!
	vector<DecFile*> Files;//!
	DecFile *ActiveFile=0;
	bool AssignBuffers(int ThreadID);//функция, следящая за актуальностью буферов
	int BufSize=1e6;
	int NThreads=1;
	int NBuffers=1;
	bool AddFile(DecFile *f);
	bool AddFile(string filename);
	void SetNThreads(int _NThreads);
	void SetNBuffers(int _NBuffers);
	void SetBufferSize(int _BufSize);
	void Reopen();
	void Reset();
	bool ThereAreNotFinishedFiles=false;
	bool FileReadingFinished=false;
	ULong64_t PrevWord=0;
	vector<Event> GetVectorOfEvents(int size,int ThreadID=0);
	bool FillBuffers();
	bool Initialized=false;
	vector<bool> InitializedForThread;
	bool GetNextEvent(Event *ev, int ThreadID=0);
	Event GetCopyOfNextEvent(int ThreadID=0);
	bool FillingNow=false;
	void FillBuffersInCycle();
	void Start();
	ULong64_t NEvents=0;
	bool ContinueReading=false;
	bool IsAnalysisFinished();
	vector<ULong64_t> NEventsPerThread;
	ClassDef(DecManager,1);
};
