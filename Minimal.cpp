#include "DecLibTest.hh"
#include <TH2F.h>
#include <TStopwatch.h>
#include <sstream>
#include <fstream>
#include <iostream>

void Minimal(string filename="test72_HPGe_2_LaBr_4_Ing27_30perc_C_Ba_Cs_Co.dec")
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

	while(dm.GetNextEvent(&ev,0))
	{
		Pulse* pulse_hpge=ev.GetPulse(16);//импульс с hpge детектора
		if(pulse_hpge)//если в событии этот импульс есть
		{
			Spectrum.Fill(pulse_hpge->Area);
		}
	}
	TFile result("result.root","recreate");
	result.WriteTObject(&Spectrum);
	result.Close();	
}