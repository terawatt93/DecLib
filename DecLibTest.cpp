#include "DecLibTest.hh"
#include <future>  
#include <iostream>
#include <fstream>   

#include <condition_variable>
#include <mutex>
#include "romana_headers/BinRead.cpp"
#include <new>

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
			cout<<"sleep "<<this_thread::get_id()<<"\n";
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
      	gr.SetPoint(iterator,(*iterTstmp)*fFile->Period/(1.0e9),value-prev);
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
        cout<<"Error in DataBuffer::Read: invalid gzFile\n";
        FileEnded=true;
        ReadyForRead=true;
        return;
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
		if((res!=8)||((fDecManager->NEventsPerThread[ThreadID]>=fDecManager->MaxNEvents)&&(fDecManager->MaxNEvents>0)))
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
			if(fFile->FirstTstmp==0)
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
		if(fDecManager->verbose>1)
		{
			cout<<"This is bool DataBuffer::GetNextEvent(Event *ev): there are no events! False returned";
		}
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
			LastTstmp=Timestamp;
			if(!ev) return false;
			ev->NPulses=0;
			ev->fFile=fFile;
			ev->Tstmp=Timestamp;
			NReadEvents++;
			LastRead++;
			{
				std::lock_guard<std::mutex> guard(fDecManager->StateMutex);
				fDecManager->NEventsPerThread[ThreadID]++;
			}
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
					//ev->Pulses[ev->NPulses].Time = (buf2[1]+gRandom->Rndm()-0.5)*0.010; в нс при периоде 10 нс
					ev->Pulses[ev->NPulses].Time = (buf2[1]+gRandom->Rndm()-0.5)*ev->fFile->Period; 
					ev->Pulses[ev->NPulses].Width = (buf2[2]+gRandom->Rndm()-0.5)*0.001;
					ev->Pulses[ev->NPulses].Height = ((UInt_t) buf1u[7]) << 8;
					ev->Pulses[ev->NPulses].fEvent=ev;
					//if((ev->Pulses[ev->NPulses].Area>0)&&(ev->Pulses[ev->NPulses].Height<30000)&&(ev->Pulses[ev->NPulses].Height>0)&&(ev->Pulses[ev->NPulses].Width>0))
					ev->NPulses++;
				}
			}
			return true; // Last event in the buffer (including EOF).
		}
	}
	if(fDecManager->verbose>2)
	{
		int Seconds=int((double)LastTstmp*(ev->fFile->Period/1e9))%60;
		int Minutes=(int((double)LastTstmp*(ev->fFile->Period/1e9))%3600)/60;
		int Hours=int((double)LastTstmp*(ev->fFile->Period/1e9))/3600;
		cout<<"Buf:"<<ID<<" Nevents NReadEvents:"<<NEvents<<" "<<NReadEvents<<" ("<<Hours<<":"<<Minutes<<":"<<Seconds<<") "<<"\n";
	}
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
    Channels.clear();
    PresentCOptions.clear();
    PresentTOptions.clear();
    jsTOptions = nlohmann::json::object();
    jsCOptions = nlohmann::json::object();
    // Reset previous-file values, so an absent property never leaks from a
    // previously opened DEC file. ROOT TObject-derived classes are assignable.
	COptions.~Coptions();
	new (&COptions) Coptions();

	TOptions.~Toptions();
	new (&TOptions) Toptions();
    ReadFromBin(COptions, buf, size, &PresentCOptions);
    ReadFromBin(TOptions, buf, size, &PresentTOptions);
    ToJSON(COptions, jsCOptions);
    ToJSON(TOptions, jsTOptions);
    // JSON keys indicate actual presence, not merely members of the schema.
    for (auto it=jsCOptions.begin(); it!=jsCOptions.end(); ) {
        if (it.key() != "Class" && !HasCOption(it.key())) it=jsCOptions.erase(it);
        else ++it;
    }
    for (auto it=jsTOptions.begin(); it!=jsTOptions.end(); ) {
        if (it.key() != "Class" && !HasTOption(it.key())) it=jsTOptions.erase(it);
        else ++it;
    }
    if (!fFile) throw std::runtime_error("RomanaOptions has no DecFile");
    // Only update derived properties when their source exists in the header.
    if (HasCOption("F_start")) fFile->TStart = COptions.F_start/1000+788907600;
    if (HasTOption("Period")) fFile->Period = TOptions.Period;
    if (HasTOption("Comment")) fFile->Comment = romana_bin_detail::bounded(TOptions.Comment);
    const int count = HasTOption("Nchan") ? std::max(0, std::min(TOptions.Nchan, MAX_CHTP)) : 0;
    for (int i=0; i<count; ++i) {
        Channel ch;
        ch.Number=i;
        ch.On=HasCOption("on") ? COptions.on[i] : false;
        const int kind=HasTOption("chtype") ? TOptions.chtype[i] : 0;
        if (HasTOption("ch_name") && kind >= 1 && kind <= MAX_TP)
            ch.Name=romana_bin_detail::bounded(TOptions.ch_name[kind-1]);
        else
            ch.Name="Channel " + std::to_string(i);
        ch.Calibration.push_back(HasTOption("E0") ? TOptions.E0[i] : 0);
        ch.Calibration.push_back(HasTOption("E1") ? TOptions.E1[i] : 0);
        ch.Calibration.push_back(HasTOption("E2") ? TOptions.E2[i] : 0);
        Channels.push_back(ch);
    }

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
/*	char *buf2=buf+size;
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
}*/
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
	Int_t sz=0;
    auto readExact = [&](void *dst, unsigned n) -> bool {
        return gzread(ff, dst, n) == static_cast<int>(n);
    };
    if (!readExact(&fmt, sizeof(Short_t))) {
        gzclose(ff); ff=nullptr; return false;
    }
	if (fmt==129)
	{
		if (!readExact(&mod,sizeof(Short_t)) || !readExact(&sz,sizeof(Int_t))) {
            gzclose(ff); ff=nullptr; return false;
        }
	}
	else
	{
		mod=fmt;
		fmt=129;
		UShort_t shortSize=0;
        if (!readExact(&shortSize,sizeof(shortSize))) {
            gzclose(ff); ff=nullptr; return false;
        }
        sz=shortSize;
	}

	if (fmt!=129 || mod>100 || sz<=0 || sz>500000)
	{//возможно, это текстовый файл
		//или старый файл без параметров
		cout << "Header not found: "<< endl;
        gzclose(ff);
        ff=nullptr;
		return false;
	}
	
	std::vector<char> buf(static_cast<size_t>(sz));
    if (!readExact(buf.data(), static_cast<unsigned>(sz))) {
        std::cerr << "Incomplete Romana parameter header in " << Name << "\n";
        gzclose(ff); ff=nullptr; return false;
    }
    ROptions.fFile=this;
    try {
        ROptions.ReadFromBuffer(buf.data(),sz);
    } catch (const std::exception &e) {
        std::cerr << "Invalid Romana parameters in " << Name << ": " << e.what() << "\n";
        gzclose(ff); ff=nullptr; return false;
    }
	CountObject.fFile=this;
	
	/*cout<<"sz="<<sz<<"\n";
	cout<<"buf:"<<buf<<"\n";
	
	ofstream ofs("par.bin",std::ios::binary);
	ofs.write(buf,sz);
	ofs.close();*/
	
	
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
	ActiveFile=Files[Files.size()-1];
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
	if (!f->Open(filename))
    {
        delete f;
        return false;
    }
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
    std::fill(NEventsPerThread.begin(), NEventsPerThread.end(), 0);
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
void CheckAndFillBuffers(vector<DataBuffer> *Buffers, DecFile *f, ULong64_t &PrevWord)
{
    DecManager &dm = DecManager::Instance();
    // The state lock publishes complete buffers and prevents concurrent reset/assignment.
    // The file reader is single-threaded; no other thread may read the same gzFile.
    std::unique_lock<std::mutex> guard(dm.StateMutex);
    if (dm.FillingNow) return;
    dm.FillingNow = true;
    while (true)
    {
        bool anyFree = false;
        for (size_t i = 0; i < Buffers->size(); ++i)
        {
            DataBuffer &buffer = (*Buffers)[i];
            if (buffer.LastWord != 0) continue;
            anyFree = true;
            buffer.fFile = f;
            // A buffer can be assigned to a consumer only after Read returns.
            buffer.Read(f->ff, PrevWord);
            if (buffer.FileEnded) f->Finished = true;
            // Initialize only actual consumer threads, never by using an unchecked buffer index.
            for (size_t tid = 0; tid < dm.InitializedForThread.size(); ++tid)
            {
                if (dm.InitializedForThread[tid]) continue;
                if (!buffer.ReadyForRead || buffer.FirstEventWord == -1) continue;
                if (buffer.Active) continue;
                dm.ActiveBuffers[tid] = &buffer;
                buffer.Active = true;
                buffer.ThreadID = static_cast<int>(tid);
                dm.InitializedForThread[tid] = true;
                break;
            }
            if (buffer.FileEnded)
            {
                dm.FillingNow = false;
                return;
            }
        }
        if (!anyFree) break;
        // If there is no data and the file has ended, do not loop on empty buffers.
        if (f->Finished) break;
    }
    dm.FillingNow = false;
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
		bool freeBuffer = false;
        {
            std::lock_guard<std::mutex> guard(StateMutex);
            freeBuffer = AreThreeFreeBuffers(&Buffers);
        }
        if(freeBuffer)
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
				FileReadingFinished=true;
                if(verbose>1)
                    cout<<"This is DecManager::FillBuffers(): there are no files for reading.\n";
                return;
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
		if(verbose>1)
		{
			cout<<"This is DecManager::FillBuffers(): there are no files for reading.\n";
		}
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
    if (!ev || ThreadID < 0 || ThreadID >= NThreads) return false;
    while (true)
    {
        DataBuffer *buffer = nullptr;
        {
            std::lock_guard<std::mutex> guard(StateMutex);
            if (InitializedForThread[ThreadID])
                buffer = ActiveBuffers[ThreadID];
        }
        if (!buffer)
        {
            if (FileReadingFinished) return false;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        if (buffer->GetNextEvent(ev)) return true;
        {
            std::lock_guard<std::mutex> guard(StateMutex);
            // Release exhausted buffer; background reader may now reuse it.
            buffer->Reset();
            ActiveBuffers[ThreadID] = nullptr;
            if (AssignBuffers(ThreadID)) continue;
        }
        // No ready buffer at this moment. EOF is final only when no unclaimed
        // ready buffer remains; loop checks both conditions under the state lock.
        while (true)
        {
            {
                std::lock_guard<std::mutex> guard(StateMutex);
                if (AssignBuffers(ThreadID)) break;
                if (FileReadingFinished) return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

// Caller must hold StateMutex.
bool DecManager::AssignBuffers(int ThreadID)
{
    for (auto &buffer : Buffers)
    {
        if (buffer.LastWord > 0 && buffer.FirstEventWord != -1 &&
            buffer.ReadyForRead && !buffer.Active)
        {
            ActiveBuffers[ThreadID] = &buffer;
            buffer.Active = true;
            buffer.ThreadID = ThreadID;
            InitializedForThread[ThreadID] = true;
            return true;
        }
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
        if (Files[i]->ff) gzclose(Files[i]->ff);
        Files[i]->Finished = false;
		Files[i]->Open(Files[i]->Name);
	}
    PrevWord=0;
    FileReadingFinished=false;
    std::fill(InitializedForThread.begin(), InitializedForThread.end(), false);
    std::fill(ActiveBuffers.begin(), ActiveBuffers.end(), nullptr);
}
void DecManager::Reset()
{
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		Buffers[i].Reset();
	}
	Files.resize(0);
    ActiveFile=nullptr;
    PrevWord=0;
    std::fill(InitializedForThread.begin(), InitializedForThread.end(), false);
    std::fill(ActiveBuffers.begin(), ActiveBuffers.end(), nullptr);
	FileReadingFinished=false;
}
bool DecManager::IsAnalysisFinished()
{
    std::lock_guard<std::mutex> guard(StateMutex);
	for(unsigned int i=0;i<Buffers.size();i++)
	{
		//cout<<"Buffers["<<i<<"] "<<Buffers[i].Active<<"\n";
		if(Buffers[i].Active)
		{
			return false;
		}
	}
	if(FileReadingFinished)
	{
		return true;
	}
	cout<<"DecManager::IsAnalysisFinished(): return false\n";
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
