#include "DecLibTest.hh"
#include <TH2F.h>
#include <TStopwatch.h>
#include <sstream>
#include <fstream>
#include <iostream>

void ReadParameters(string filename="run07_TUMC_Co_top.dec")
{
	ROOT::EnableThreadSafety();
	TH1::AddDirectory(false);
	
	DecManager &dm=DecManager::Instance();
	dm.SetNBuffers(2);//количество буферов
	dm.SetNThreads(1);//количество потоков. В простейшем случае, 1
	dm.SetBufferSize(1e7);//размер буфера с событиями
	dm.AddFile(filename);
	//самые важные параметры (время начала измерения, продолжительность (если была задана), период оцифровки и комментарий
	//доступны непосредственно из ActiveFile
	cout<<"TStart: "<<dm.ActiveFile->TStart<<"\n";
	cout<<"Duration: "<<dm.ActiveFile->Duration<<"\n";
	cout<<"Period: "<<dm.ActiveFile->Period<<"\n";
	cout<<"Comment: "<<dm.ActiveFile->Comment<<"\n";
	//Все остальные параметры хранятся в ActiveFile.ROptions.TOptions и ActiveFile.ROptions.СOptions
	//Например, выведем список включенных каналов с их калибровками:
	//они хранятся в Coptions::on[MAX_CHTP], Toptions:E0[MAX_CHTP],E1[MAX_CHTP],E2[MAX_CHTP];  
	cout<<"Calibration:\nch\ta0\ta1\ta2\n";
	for(int i=0;i<MAX_CHTP;i++)
	{
		if(dm.ActiveFile->ROptions.COptions.on[i])
		{
			cout<<i<<"\t"<<dm.ActiveFile->ROptions.TOptions.E0[i]<<"\t"<<dm.ActiveFile->ROptions.TOptions.E1[i]<<"\t"<<dm.ActiveFile->ROptions.TOptions.E2[i]<<"\n";
		}
	}
	cout<<"Channel data:\nch\tOn\tName\ta0\ta1\ta2\n";
	//также в ROptions есть вектор Channels, в котором хранятся номера каналов, названия, состояния и калибровки
	for(unsigned int i=0;i<dm.ActiveFile->ROptions.Channels.size();i++)
	{
		Channel *ch=&dm.ActiveFile->ROptions.Channels[i];
		cout<<ch->Number<<"\t"<<ch->On<<"\t"<<ch->Name<<"\t"<<ch->Calibration[0]<<"\t"<<ch->Calibration[1]<<"\t"<<ch->Calibration[2]<<"\n";
	}
	
	//можем сохранить все параметры в json
	ofstream TOptions("TOptions.json");
	TOptions<<dm.ActiveFile->ROptions.jsTOptions.dump(4);
	TOptions.close();
	
	ofstream COptions("COptions.json");
	COptions<<dm.ActiveFile->ROptions.jsCOptions.dump(4);
	COptions.close();
}
