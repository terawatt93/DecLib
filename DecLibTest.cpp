#include "DecLibTest.hh"
#include <future>  
#include <iostream>
#include <fstream>   

#include <condition_variable>
#include <mutex>
#pragma once

std::mutex read_mutex;

int HistManager::GetThisThreadNumber()
{
	lock.lock();
	int value=0;
	for(unsigned int i=0;i<Objects.size();i++)
	{
		if(id_values[i]==this_thread::get_id())
		{
			value=i;
			break;
		}
	}
	id_values.push_back(this_thread::get_id());
	value=id_values.size()-1;
	Objects.resize(id_values.size());
	cout<<"objects.size():"<<Objects.size()<<"\n";
	lock.unlock();
	return value;
}

TH1F* HistManager::BookTH1F(int thread_id,const char* Name,const char* Title,int NBins, double Min, double Max)
{
	lock.lock();
	if((int)Objects.size()<thread_id+1)
	{
		Objects.resize(thread_id+1);
	}
	int IDValue=thread_id;
	lock.unlock();
	//int IDValue=GetThisThreadNumber();
	
	TH1F* HistPointer=new TH1F(Name,Title,NBins,Min,Max);
	Objects[IDValue].push_back(HistPointer);
	return HistPointer;
}
TH1D* HistManager::BookTH1D(int thread_id,const char* Name,const char* Title,int NBins, double Min, double Max)
{
	lock.lock();
	if((int)Objects.size()<thread_id+1)
	{
		Objects.resize(thread_id+1);
	}
	int IDValue=thread_id;
	lock.unlock();
	TH1D* HistPointer=new TH1D(Name,Title,NBins,Min,Max);
	Objects[IDValue].push_back(HistPointer);
	return HistPointer;
}
TH2F* HistManager::BookTH2F(int thread_id,const char* Name,const char* Title,int NBinsX, double MinX, double MaxX,int NBinsY, double MinY, double MaxY)
{
	lock.lock();
	if((int)Objects.size()<thread_id+1)
	{
		Objects.resize(thread_id+1);
	}
	int IDValue=thread_id;
	lock.unlock();
	TH2F* HistPointer=new TH2F(Name,Title,NBinsX,MinX,MaxX,NBinsY,MinY,MaxY);
	Objects[IDValue].push_back(HistPointer);
	return HistPointer;
}

void HistManager::Merge()
{
	DecManager& dm=dm.Instance();
	cout<<"Merge()!\n";
	while(1)
	{
		if(!dm.IsAnalysisFinished())
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		else
		{
			break;
		}
	}
	cout<<"while passed\n";
	int NHistograms=0;
	if((Merged.size()==0)&&(Objects.size()>0))
	{
		NHistograms=Objects[0].size();
		lock.lock();
		Merged.resize(NHistograms);
		MergingNow.resize(NHistograms);
		for(int i=0;i<NHistograms;i++)
		{
			Merged[i]=false;
			MergingNow[i]=false;
		}
		lock.unlock();
	}
	if(Objects.size()>0)
	{
		NHistograms=Objects[0].size();
	}
	while(!MergingDone)
	{
		int CurrentNum=-1;
		lock.lock();
		for(unsigned int i=0;i<Merged.size();i++)
		{
			if(!Merged[i])
			{
				CurrentNum=i;
				Merged[i]=true;
				break;
			}
		}
		lock.unlock();
		if(CurrentNum>-1)
		{
			//cout<<"CurrentNum: "<<CurrentNum<<"\n";
		//	cout<<"Size:"<<Objects.size()<<"\n";
			for(unsigned int i=1;i<Objects.size();i++)
			{
		//		cout<<"Sum hist: "<<i<<"\n";
				Objects[0][CurrentNum]->Add(Objects[i][CurrentNum]);
				delete Objects[i][CurrentNum];
			}
		}
		else
		{
			MergingDone=true;
			return;
		}
	}
}
void HistManager::SaveToFile(TFile *f)
{
	if(!f)
	{
		cout<<"This is HistManager::SaveToFile(TFile *f): pointer to file is invalid\n";
	}
	if(!MergingDone)
	{
		Merge();
	}
	if(Objects.size()>0)
	{
		for(unsigned int i=0;i<Objects[0].size();i++)
		{
			f->WriteTObject(Objects[0][i]);
		}
	}
}

void Counter::AddCount(int Ch,Long64_t _Tstmp,Long64_t Count)
{
	if(int(HWCounts.size())<Ch+1)
	{
		HWCounts.resize(Ch+1);
		Tstmp.resize(Ch+1);
	}
	Tstmp[Ch].push_back(_Tstmp);
	HWCounts[Ch].push_back(Count);
}
vector<TGraph> Counter::ToTGraphs()
{
	vector<TGraph> result;
	for(unsigned int i=0;i<HWCounts.size();i++)
	{
		if(HWCounts[i].size()>0)
		{
			TGraph gr(Tstmp.size());
			auto iterTstmp=Tstmp[i].begin();
			gr.SetName(TString::Format("Count_%d",i));
			gr.SetTitle(TString::Format("Count_%d; Time, s; Count",i));
			int iterator=0;
			Long64_t prev=0;
			for (auto iter = HWCounts[i].begin(); iter != HWCounts[i].end(); iter++)
    	{
    		Long64_t value=(*iter);
    		Long64_t value_prev=(*iter);
      	gr.SetPoint(iterator,(*iterTstmp)/(1.0e8),value-prev);
      	prev=value_prev;
      	iterator++;
      	iterTstmp++;
    	}
    	result.push_back(gr);
		}
	}
	return result;
}
DataBuffer::DataBuffer(int _BufferLength)
{
	if(_BufferLength>0)
	{
		Buf=new ULong64_t[_BufferLength+MAXCH+1];//чтобы гарантированно влезло целое число событий
		BufferLength=_BufferLength+MAXCH+1;
		std::memset(Buf, 0, BufferLength*8);
	}
}


void DataBuffer::SetSize(int size)
{
	if(Buf)
	{
		delete[] Buf;
	}
	if(size>0)
	{
		Buf=new ULong64_t[size+MAXCH+1];//чтобы гарантированно влезло целое число событий
		BufferLength=size+MAXCH+1;
		std::memset(Buf, 0, BufferLength*8);
	}
}
void DataBuffer::Read(gzFile ff,ULong64_t &PrevWord)
{
	if(BufferLength==0)
	{
		cout<<"This is DataBuffer::Read(gzFile ff,ULong64_t &PrevWord): buffer size is 0. Size was chosen 1e6 by default\n";
		SetSize(1e6);
	}
	if(!ff)
	{
		cout<<"Error in DataBuffer::Read(gzFile *ff): ff pointer is invalid!\n";
	}
	if(PrevWord>0)
	{
		Buf[0]=PrevWord;
		FirstEventWord=0;
		LastWord=1;
		PrevWord=0;
	}
	while(LastWord<BufferLength-(MAXCH+1))
	{
		int res=gzread(ff,&Buf[LastWord],8);
		//if((!res)||(fDecManager->NEvents>5e7))
		if(!res)
		{
			FileEnded=true;
			ReadyForRead=true;
			return;
		}
		UChar_t* w8 = (UChar_t*) &Buf[LastWord];
		//UChar_t* w8 = (UChar_t*) &word;
		UChar_t frmt = w8[7] & 0x80; //event start bit
		//cout<<"frmt "<<(int)frmt<<"\n";
		if(frmt)
		{
			PrevState = UChar_t(Buf[LastWord]>>48);
			if(fFile->FirstTstmp!=0)
			{
				fFile->FirstTstmp=Buf[LastWord]&0xFFFFFFFFFFFF;
			}
			fFile->LastTstmp=Buf[LastWord]&0xFFFFFFFFFFFF;
			if(LastWord<BufferLength-MAXCH)
			{
				NEvents++;
				fFile->NEvents++;
				fDecManager->NEvents++;
				if(FirstEventWord==-1)
				{
					FirstEventWord=LastWord;
				}
				AverageSize=(float)NEvents/LastWord;
			}
			else
			{
				PrevWord=Buf[LastWord];
				Buf[LastWord]=0;
				ReadyForRead=true;
			//	cout<<"ReadyForRead!\n";
				return;
			}
		}
		else
		{
			
			if((PrevState & 128))
			{
				Short_t* buf2 = (Short_t*) &Buf[LastWord];
				UChar_t Chan = (UChar_t) buf2[3];
				Long64_t Counter=(Buf[LastWord])&0xFFFFFFFFFFFF;
				if(fFile->Counts.size()<Chan+1)
				{
					fFile->Counts.resize(Chan+1);
				}
				fFile->Counts[Chan]=Counter;
				fFile->CountObject.AddCount(Chan,fFile->LastTstmp,Counter);
			//	cout<<"Count:"<<(int)Chan<<" "<<Counter<<"\n";
				
			}
		}
		//cout<<Buf[LastWord]<<" "<<LastWord<<"\n";
		LastWord++;
	}
	ReadyForRead=true;
//	cout<<"ReadyForRead!\n";
//	cout<<"Buffer "<<ID<<" NEvents:"<<NEvents<<"\n";
}

void DataBuffer::Reset()
{
	NEvents=0;
	NReadEvents=0;
	LastWord=0;
	LastRead=0;
	AverageSize=0;
	FileEnded=false;
	FirstEventWord=-1;
	Active=false;
	ReadyForRead=false;
	//cout<<"Reset buffer "<<ID<<"\n";
}

inline UChar_t GetFrmt(ULong64_t *word)
{
	UChar_t* w8 = (UChar_t*) word;
	return w8[7] & 0x80;
}

bool DataBuffer::GetNextEvent(Event *ev)
{
	if(FirstEventWord==-1)
	{
		cout<<"This is bool DataBuffer::GetNextEvent(Event *ev): there are no events! False returned";
		return false;
	}
	if(LastRead==0)
	{
		LastRead=FirstEventWord;
	}
	if(LastRead<LastWord)
	{
		if(GetFrmt(&Buf[LastRead]))
		{
			Long64_t Timestamp=Buf[LastRead] & sixbytes;
			if(!ev)
			{
				ev=new Event();
			}
			ev->NPulses=0;
			ev->Tstmp=Timestamp;
			NReadEvents++;
			LastRead++;
			fDecManager->NEventsPerThread[ThreadID]++;
			if(Timestamp>0)
			{
				while(LastRead<LastWord)
				{
					if(GetFrmt(&Buf[LastRead]))
					{
						return true;
					}
					Short_t* buf2 = (Short_t*) &Buf[LastRead];
					UShort_t* buf2u = (UShort_t*) &Buf[LastRead];
					LastRead++;
					UChar_t* buf1u = (UChar_t*) buf2;
					ev->fFile=fFile;
					if(int(ev->Pulses.size())<ev->NPulses+1)
					{
						ev->Pulses.push_back(Pulse());
					}
					ev->Pulses[ev->NPulses].Chan=(UChar_t) buf2[3];
					ev->Pulses[ev->NPulses].Area=(*buf2u+gRandom->Rndm()-1.5)*0.2;
					ev->Pulses[ev->NPulses].Time = (buf2[1]+gRandom->Rndm()-0.5)*0.01; 
					ev->Pulses[ev->NPulses].Width = (buf2[2]+gRandom->Rndm()-0.5)*0.001;
					ev->Pulses[ev->NPulses].Height = ((UInt_t) buf1u[7]) << 8;
					ev->Pulses[ev->NPulses].fEvent=ev;
					//if((ev->Pulses[ev->NPulses].Area>0)&&(ev->Pulses[ev->NPulses].Height<30000)&&(ev->Pulses[ev->NPulses].Height>0)&&(ev->Pulses[ev->NPulses].Width>0))
					ev->NPulses++;
				}
			}
		}
	}
	cout<<"Buf:"<<ID<<" Nevents NReadEvents:"<<NEvents<<" "<<NReadEvents<<"\n";
	return false;
}

Pulse* Event::GetPulse(unsigned char Chan)
{
	for(int i=0;i<NPulses;i++)
	{
		if(Pulses[i].Chan==Chan)
		{
			return &Pulses[i];
		}
	}
	return 0;
}
vector<Pulse*> Event::GetPulses()
{
	vector<Pulse*> result(NPulses);
	for(int i=0;i<NPulses;i++)
	{
		result[i]=&Pulses[i];
	}
	return result;
}
/*int RomanaOptions::ReadFromDec(char* buf, char* buf2)
{
  // Ищет нужные параметры в буфере buf
  // прекращает поиск в конце буфера (buf2 должно быть = buf+sz),
  // Возвращает 1 - если копирование успешно
  //            0 - если произошла ошибка
	TString classname;
	TString varname;
	TString memname;
	UShort_t len=0;
	const UShort_t mx=50000;
	char data[mx];
	
	vector<int> ChannelTypes;//будем пока складывать в эти векторы параметры каналов. Да, не лучшее решение, но такова структура файла
	vector<Bool_t> DspFlags, StartFlags;
	while (buf<buf2) 
	{
		memcpy(&len,buf,sizeof(len)); //считываем длину записи имени в буфере
		//могут быть нули в конце буфера из-за кратности 8
		if (len==0 && buf2-buf<10) 
		{
		//cout << "len==0: " << len << " " << buf2-buf << endl;
		break ;
		//return 1;
		}
		else if (len==0 || len>=mx || buf+len>buf2)
		{
			cout << "len1 error: " << len << " " << buf2-buf << endl;
			return 0;
		}
		buf+=sizeof(len);
		memname = TString(buf,len-1); //считываем имя в буфере
		buf+=len;
		// data
		memcpy(&len,buf,sizeof(len)); //считываем длину данных в буфере
		if (len==0 || len>=mx || buf+len>buf2) 
		{
			cout << "len2 error: " << len << " " << buf2-buf << endl;
			return 0;
		}
		buf+=sizeof(len);
		memcpy(data,buf,len); //считываем данные в буфере
		buf+=len;
		if (memname.EqualTo("class")) 
		{
			classname = TString(data,len-1); //считываем имя класса в буфере
			continue;
		}
		else if (memname.EqualTo("var")) 
		{
			varname = TString(data,len-1); //считываем имя переменной в буфере
			continue;
		}
		
		if(classname=="Toptions")
		{
			//cout<<memname<<" "<<varname<<" Data:"<<TString(data,len-1)<<"\n";
			if(memname=="ch_name")
			{
				
				TString DetNames(data,len-1);
				DetNames.ReplaceAll(" ","_");
				for(int i=0;i<DetNames.Length();i++)
				{
					if((DetNames[i]<'0')||(DetNames[i]>'z'))
					{
						DetNames[i]=' ';
					}
				}
				stringstream s(DetNames.Data());
				while(s)
				{
					string str;
					s>>str;
					if(str.size()>0)
					{
						PossibleChNames.push_back(str);
					}
					
				}
			}
			if(memname=="Nchan")
			{
				UChar_t* ChPart;
				ChPart=(UChar_t*)data;
				NChannelsInSystem=ChPart[0];
			}
			if(memname=="chtype")
			{
				Int_t* ChTypePart;
				ChTypePart=(Int_t*)data;
				for(int i=0;i<len/4;i++)
				{
					ChannelTypes.push_back(ChTypePart[i]);
				}
			}
			if(memname=="St")
			{
				Bool_t* ChType;
				ChType=(Bool_t*)data;
				for(int i=0;i<len;i++)
				{
					StartFlags.push_back(ChType[i]);
				}
			}
			
			if(memname=="dsp")
			{
				Bool_t* ChType;
				ChType=(Bool_t*)data;
				for(int i=0;i<len;i++)
				{
					DspFlags.push_back(ChType[i]);
				}
			}
			
			if(memname=="F_start")
			{
				Long64_t* F_start=(Long64_t*)data;
				StartTime.Set((F_start[0]+788907600000)*0.001);
				
			}
			if(memname=="F_stop")
			{
				Long64_t* F_stop=(Long64_t*)data;
				StopTime.Set((F_stop[0]+788907600000)*0.001);
			}
			if(memname=="T_acq")
			{
				Float_t* T_acq_tmp=(Float_t*)data;
				MeasurementDuration=T_acq_tmp[0];
			}
			if(memname=="Period")
			{
				Float_t* Period=(Float_t*)data;
				PeriodOfDigitizing=Period[0];
			}
			
		}
	}
	ChannelTypes.resize(NChannelsInSystem);
	DspFlags.resize(NChannelsInSystem);
	StartFlags.resize(NChannelsInSystem);
	for(unsigned int i=0;i<NChannelsInSystem;i++)
	{
		Channels.emplace_back(i,PossibleChNames[ChannelTypes[i]-1],StartFlags[i],DspFlags[i]);
	}
	return 0;
}
*/

void RomanaOptions::ReadFromBuffer(char *buf,int size)
{
	//формат: сначала идет длина имени, потом имя, потом длина блока данных, потом блок данных
	//UShort_t len=0;
	//char *bufEnd=buf+size;
	//while (buf+sizeof(len)<bufEnd)
	//{
		//if (len==0 && bufEnd-buf<10)
		//{
			////cout << "len==0: " << len << " " << buf2-buf << endl;
			//return;
		//}
		//memcpy(&len,buf,sizeof(len));
		//buf+=sizeof(len);
		//string Name=string(buf,len-1);
		//UShort_t DataLen=0;
		//memcpy(&DataLen,buf,sizeof(DataLen));
		//buf+=DataLen;
		//cout<<"par: "<<Name<<" data size: "<<DataLen<<" "<<size<<"\n";
	//}
	char *buf2=buf+size;
	TString classname;
  TString varname;
  TString memname;
  UShort_t len=0;
  const UShort_t mx=50000;
  char data[mx];

  while (buf+sizeof(len)<buf2) {

    memcpy(&len,buf,sizeof(len)); //считываем длину записи имени в буфере
    //prnt("ssd d ls;",BYEL,"bf2: ",len,mx,buf+len-buf2,RST);

    //могут быть нули в конце буфера из-за кратности 8
    if (len==0 && buf2-buf<10) {
      //cout << "len==0: " << len << " " << buf2-buf << endl;
      return ;
    }
    else if (len==0 || len>=mx || buf+len>buf2) {
      //prnt("ssd d ls;",BRED,"errlen1: ",len,mx,buf+len-buf2,RST);
      //cout << "len1 error: " << len << " " << buf2-buf << endl;
      return ;
    }
    buf+=sizeof(len);
    memname = TString(buf,len-1); //считываем имя в буфере
    buf+=len;

    // data
    memcpy(&len,buf,sizeof(len)); //считываем длину данных в буфере
    if (len==0 || len>=mx || buf+len>buf2) {
      cout << "len2 error: " << len << " " << buf2-buf << endl;
      return ;
    }
    buf+=sizeof(len);
    memcpy(data,buf,len); //считываем данные в буфере
    buf+=len;

    if (memname.EqualTo("class")) {
      classname = TString(data,len-1); //считываем имя класса в буфере
      continue;
    }
    else if (memname.EqualTo("var")) {
      varname = TString(data,len-1); //считываем имя переменной в буфере
      continue;
    }

    if (classname.EqualTo("Hdef")) {
      memname.Prepend('.');
      memname.Prepend(varname);
    }
    cout<<"par: m:"<<memname<<" c:"<<classname<<" v:"<<varname<<"\n";
}
}

bool DecFile::Open(string _Name)
{
	if(_Name.size()>0)
	{
		Name=_Name;
	}
	ff = gzopen(Name.c_str(),"rb");
	if(!ff)
	{
		cout<<"Cannot open "<<Name<<"\n";
		return false;
	}
	Int_t sz;
	gzread(ff,&fmt,sizeof(Short_t));
	if (fmt==129)
	{
		gzread(ff,&mod,sizeof(Short_t));
		gzread(ff,&sz,sizeof(Int_t));
	}
	else
	{
		mod=fmt;
		fmt=129;
		gzread(ff,&sz,sizeof(UShort_t));
	}

	if (fmt!=129 || mod>100 || sz>5e5)
	{//возможно, это текстовый файл
		//или старый файл без параметров
		cout << "Header not found: "<< endl;
		return false;
	}

	char *buf = new char[sz];
	gzread(ff,buf,sz);
	
	ROptions.ReadFromBuffer(buf,sz);
	
	
	cout<<"sz="<<sz<<"\n";
	cout<<"buf:"<<buf<<"\n";
	
	ofstream ofs("par.txt",std::ios::binary);
	ofs.write(buf,sz);
	ofs.close();
	/*Int_t sz;
	gzread(ff,&fmt,sizeof(Short_t)); 
	if(fmt>128)
	{
		gzread(ff,&mod,sizeof(Short_t));
		gzread(ff,&sz,sizeof(Int_t));
	}
	else 
	{
		mod=fmt;
		gzread(ff,&sz,sizeof(UShort_t));
	} */
	//gzread(ff,&mod,sizeof(mod));
	//gzread(ff,&sz,sizeof(sz));
	/*char* buf = new char[sz];
	gzread(ff,buf,sz);//тут хранятся параметры
	//BufToClass(buf,buf+sz); 
	ROptions.ReadFromDec(buf,buf+sz);*/
	FileType=mod;
	return true;
}
bool DecFile::IsFinished()
{
	return Finished;
}
bool DecManager::AddFile(DecFile *f)
{
	if(!f)
	{
		cout<<"This is DecManager::AddFile(DecFile *f): pointer to file is invalid! False returned\n";
		return false;
	}
	for(unsigned int i=0;i<Files.size();i++)
	{
		if(Files[i])
		{
			if(Files[i]->Name==f->Name)
			{
				cout<<"This is DecManager::AddFile(DecFile *f): File with name "<<f->Name<<" already existed in buffer. False returned.\n";
				return false;
			}
		}
	}
	Files.push_back(f);
	return true;
}
bool DecManager::AddFile(string filename)
{
	for(unsigned int i=0;i<Files.size();i++)
	{
		if(Files[i])
		{
			if(Files[i]->Name==filename)
			{
				cout<<"This is DecManager::AddFile(string filename): File with name "<<filename<<" already existed in buffer. False returned.\n";
				return false;
			}
		}
	}
	DecFile *f=new DecFile();
	f->Open(filename);
	if(!AddFile(f))
	{
		delete f;
		cout<<"This is DecManager::AddFile(string filename): problem with file "<<filename<<" opening. False returned.\n";
		return false;
	}
	return true;
}
void DecManager::SetNThreads(int _NThreads)
{
	NThreads=_NThreads;
	NEventsPerThread.resize(NThreads);
	ActiveBuffers.resize(0);
	ActiveBuffers.resize(NThreads);
	InitializedForThread.resize(NThreads);
	for(int i=0;i<NThreads;i++)
	{
		InitializedForThread[i]=false;
	}
}
void DecManager::SetNBuffers(int _NBuffers)
{
	NBuffers=_NBuffers;
	Buffers.resize(0);
	Buffers.resize(NBuffers);
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		Buffers[i].ID=i;
		Buffers[i].fDecManager=this;
	}
}
void DecManager::SetBufferSize(int _BufSize)
{
	BufSize=_BufSize;
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		Buffers[i].SetSize(BufSize);
	}
}
bool AreThreeFreeBuffers(vector<DataBuffer> *Buffers)
{
	for(unsigned int i=0;i<Buffers->size();i++)
	{
		//cout<<" "<<i<<" "<<Buffers->at(i).LastWord<<"\n";
	//	cout<<"AreThreeFreeBuffers: buf:"<<Buffers->at(i).ID<<" "<<Buffers->at(i).LastWord<<"\n";
		if(Buffers->at(i).LastWord==0)
		{
			return true;
		}
	}
	return false;
}
void CheckAndFillBuffers(vector<DataBuffer> *Buffers,DecFile *f,ULong64_t &PrevWord)//остановился тут
{
	//cout<<"CheckAndFillBuffers\n";
	if(DecManager::Instance().FillingNow)
	{
		return;
	}
	DecManager::Instance().FillingNow=true;
	if(f->Finished)
	{
		cout<<"File "<<f->Name<<" is finished\n";
	}
	
	while(1)
	{
	//	bool ThereAreFreeBuffers=true;
		for(unsigned int i=0;i<Buffers->size();i++)
		{
			//cout<<" "<<i<<" "<<Buffers->at(i).LastWord<<"\n";
			if(Buffers->at(i).LastWord==0)
			{
				Buffers->at(i).fFile=f;
				Buffers->at(i).Read(f->ff,PrevWord);
			}
			if((!Buffers->at(i).fDecManager->InitializedForThread[i])&&(i<Buffers->at(i).fDecManager->InitializedForThread.size()))
			{
				Buffers->at(i).fDecManager->InitializedForThread[i]=true;
				Buffers->at(i).fDecManager->ActiveBuffers[i]=&(Buffers->at(i));
				Buffers->at(i).fDecManager->ActiveBuffers[i]->Active=true;
				Buffers->at(i).fDecManager->ActiveBuffers[i]->ThreadID=i;
			}
			if(Buffers->at(i).FileEnded)
			{
				f->Finished=true;
				DecManager::Instance().FillingNow=false;
				return;
			}
		}
		if(!AreThreeFreeBuffers(Buffers))
		{
			break;
		}
	}
	DecManager::Instance().FillingNow=false;
}

void DecManager::Start()
{
	thread(&DecManager::FillBuffersInCycle,this).detach();
}

void DecManager::FillBuffersInCycle()
{
	cout<<"startup\n";
	while(1)
	{
		if(AreThreeFreeBuffers(&Buffers))
		{
			ThereAreNotFinishedFiles=false;
			if(ActiveFile)
			{
				if(ActiveFile->IsFinished())
				{
					ActiveFile=0;
				}
			}
			if(!ActiveFile)
			{
				for(unsigned int i=0;i<Files.size();i++)
				{
					if(!Files[i])
					{
						cout<<"This is DecManager::FillBuffers(): pointer to file "<<i<<" is invalid! Check next files...\n";
					}
					else
					{
						if(!(Files[i]->IsFinished()))
						{
							ActiveFile=Files[i];
							ThereAreNotFinishedFiles=true;
						}
					}
				}
			}
			if(!ActiveFile)
			{
				cout<<"This is DecManager::FillBuffers(): there are no files for reading.\n";
				FileReadingFinished=true;
				return ;
			}
			ContinueReading=false;
			CheckAndFillBuffers(&Buffers,ActiveFile,PrevWord);
			Initialized=true;
			ContinueReading=true;
		}
		else
		{
		//	cout<<"sleep\n";
			std::this_thread::sleep_for(std::chrono::microseconds(1));
			if(FileReadingFinished)
				return;
		}
	}
}

bool DecManager::FillBuffers()
{
	ThereAreNotFinishedFiles=false;
	if(ActiveFile)
	{
		if(ActiveFile->IsFinished())
		{
			ActiveFile=0;
		}
	}
	if(!ActiveFile)
	{
		for(unsigned int i=0;i<Files.size();i++)
		{
			if(!Files[i])
			{
				cout<<"This is DecManager::FillBuffers(): pointer to file "<<i<<" is invalid! Check next files...\n";
			}
			else
			{
				if(!(Files[i]->IsFinished()))
				{
					ActiveFile=Files[i];
					ThereAreNotFinishedFiles=true;
				}
			}
		}
	}
	if(!ActiveFile)
	{
		cout<<"This is DecManager::FillBuffers(): there are no files for reading.\n";
		FileReadingFinished=true;
		return false;
	}
	if(!DecManager::Instance().FillingNow)
	{
		auto r=async(CheckAndFillBuffers,&Buffers,ActiveFile,ref(PrevWord));
	}
	
	//CheckAndFillBuffers(&Buffers,ActiveFile,ref(PrevWord));
	return true;
}
bool DecManager::GetNextEvent(Event *ev, int ThreadID)
{
	//cout<<"GetNextEvent\n";
	if(ThreadID>NThreads-1)
	{
		cout<<"This is DecManager::GetNextEvent(Event *ev, int ThreadID): Invalid ThreadID, NThreads="<<NThreads<<", ThreadID="<<ThreadID<<". False returned!\n";
		return false;
	}
	while(!InitializedForThread[ThreadID])
	{
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		if((FileReadingFinished)&&(!InitializedForThread[ThreadID]))
		{
			return false;
		}
		//cout<<"Init for thread "<<ThreadID<<"\n";
		//FillBuffers();
		/*for(unsigned int i=0;i<ActiveBuffers.size();i++)
		{
			ActiveBuffers[i]=&Buffers[i];
			ActiveBuffers[i]->ThreadID=ThreadID;
		}*/
		
	}
	if(ActiveBuffers[ThreadID]->GetNextEvent(ev))
	{
		return true;
	}
	else
	{
	/*	ofstream ofs(TString::Format("lod_g%d.txt",ThreadID),ios::app);
		ofs<<ThreadID<<" "<<ActiveBuffers[ThreadID]->NReadEvents<<" "<<ActiveBuffers[ThreadID]->NEvents<<"\n";
		cout<<"ActiveBuffers["<<ThreadID<<"]->NReadEvents "<<ActiveBuffers[ThreadID]->NReadEvents<<" "<<ActiveBuffers[ThreadID]->NEvents<<"\n";
		ofs.close();
		ActiveBuffers[ThreadID]->Reset();*/
		ActiveBuffers[ThreadID]->Reset();

		while(!AssignBuffers(ThreadID))
		{
			if(!FileReadingFinished)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
			else
			{
				return false;
			}
		}
		return ActiveBuffers[ThreadID]->GetNextEvent(ev);
	}
		/*if(!AssignBuffers(ThreadID))
		{
			while(1)
			{
				//cout<<"Check buffers\n";
				if(AssignBuffers(ThreadID))
				{
				//	cout<<"return next event\n";
					return ActiveBuffers[ThreadID]->GetNextEvent(ev);
				}
				else if(!FileReadingFinished)
				{
				//	cout<<"waiting\n";
					std::this_thread::sleep_for(std::chrono::microseconds(1));
				}
				else
				{
				//	cout<<"return false\n";
					return false;
				}
			}
		}
	}
	cout<<"Call FillBuffers for thread "<<ThreadID<<"\n";

	

	//return FillBuffers();*/
	
}

bool DecManager::AssignBuffers(int ThreadID)
{
	
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		if((Buffers[i].LastWord>0)&&(Buffers[i].FirstEventWord!=-1)&&(Buffers[i].ReadyForRead)&&(!Buffers[i].Active))
		{
			ActiveBuffers[ThreadID]=&Buffers[i];
			ActiveBuffers[ThreadID]->Active=true;
			ActiveBuffers[ThreadID]->ThreadID=ThreadID;
			return true;
		}/*
			if()

			bool IsAvalible=true;
			for(unsigned int j=0;j<ActiveBuffers.size();j++)
			{
				cout<<"Buf id:"<<Buffers[i].ID<<" ActiveID:"<<ActiveBuffers[j]->ID<<" is Active: "<<ActiveBuffers[j]->Active<<"\n";
				if((Buffers[i].ID==ActiveBuffers[j]->ID)&&(ActiveBuffers[j]->Active))
				{
					IsAvalible=false;
				}
			}
			cout<<"IsAvalible:"<<IsAvalible<<"\n";
			if(IsAvalible)
			{
				cout<<"Avalible\n";
				if(Buffers[i].ReadyForRead)
				{
					ActiveBuffers[ThreadID]=&Buffers[i];
					ActiveBuffers[ThreadID]->Active=true;
					ActiveBuffers[ThreadID]->ThreadID=ThreadID;
					cout<<"Assign buffer "<<ActiveBuffers[ThreadID]->ID<<" to thread "<<ThreadID<<"\n";
					return true;
				}
			}
		}*/
	}
	return false;
}

Event DecManager::GetCopyOfNextEvent(int ThreadID)
{
	Event ev;
	bool good=GetNextEvent(&ev,ThreadID);
	ev.Good=good;
	return ev;
}
void DecManager::Reopen()
{
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		Buffers[i].Reset();
	}
	for(unsigned int i=0;i<Files.size();i++)
	{
		Files[i]->Open(Files[i]->Name);
	}
}
void DecManager::Reset()
{
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		Buffers[i].Reset();
	}
	Files.resize(0);
	FileReadingFinished=false;
}
bool DecManager::IsAnalysisFinished()
{
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		if(Buffers[i].Active)
		{
			return false;
		}
	}
	if(FileReadingFinished)
	{
		return true;
	}
	return false;
}
vector<Event> DecManager::GetVectorOfEvents(int size,int ThreadID)
{
	vector<Event> result;
	Event ev;
	while((GetNextEvent(&ev,ThreadID))&&(int(result.size())<=size))
	{
		result.push_back(ev);
	}
	return result;
}
