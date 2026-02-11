#include "DecLibTest.hh"
#include <TH2F.h>
#include <TStopwatch.h>
#include <sstream>
#include <fstream>
#include <iostream>

void MinimalVector(string filename="run07_TUMC_Co_top.dec")
{
	ROOT::EnableThreadSafety();
	TH1::AddDirectory(false);
	
	DecManager &dm=DecManager::Instance();
	dm.SetNBuffers(2);//количество буферов
	dm.SetNThreads(1);//количество потоков. В простейшем случае, 1
	dm.SetBufferSize(1e7);//размер буфера с событиями
	dm.AddFile(filename);
	TH1F Spectrum("Spectrum","Spectrum",10000,0,10000);
	Event ev;
	dm.Start();
	
	while(1)
	{
		vector<Event> Events=dm.GetVectorOfEvents(10000,0);
		if(Events.size()==0)
		{
			break;
		}
		for(unsigned int i=0;i<Events.size();i++)
		{
			Pulse* pulse_hpge=Events[i].GetPulse(17);
			if(pulse_hpge)//если в событии этот импульс есть
			{
				Spectrum.Fill(pulse_hpge->Area);
			}
		}
	}
	TFile result("resultVector.root","recreate");
	result.WriteTObject(&Spectrum);
	result.Close();	
}
