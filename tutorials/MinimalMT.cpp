#include "DecLibTest.hh"

void FillInThread(DecManager *dm,TH1D *h, int ThreadID)
/*функция, которая выполняется независимо в каждом потоке. ГИСТОГРАММЫ ДОЛЖНЫ БЫТЬ РАЗНЫЕ!*/ 
{
	Event ev;
	int count=0;
	while(dm->GetNextEvent(&ev,ThreadID))
	{
		Pulse* pulse_hpge=ev.GetPulse(17);//импульс с hpge детектора
		if(pulse_hpge)//если в событии этот импульс есть
		{
			h->Fill(pulse_hpge->Area);
		}
		count++;
		if(count%10000==0)
		{
			cout<<count<<" events analysed in thread "<<ThreadID<<"\n";
		}
	}
}

void MinimalMT(string filename="run07_TUMC_Co_top.dec")
{
	ROOT::EnableThreadSafety();
	TH1::AddDirectory(false);
	int NThreads=4;
	int NBuffers=2*NThreads;//по-видимому, оптимально
	
	DecManager &dm=DecManager::Instance();//создаем DecManager
	dm.SetNBuffers(NBuffers);//количество буферов
	dm.SetNThreads(NThreads);//количество потоков. В простейшем случае, 1
	dm.SetBufferSize(1e3);//размер буфера с событиями. Лучше ставить ~1e7, 1e3 стоит для демонстрации многопоточности
	dm.AddFile(filename);//открываем файл
	dm.Start();
	//Для получения счетчиков нужен указатель на DecFile. Счетчики заполняются автоматически в процессе считывания
	DecFile *df=dm.Files[0];
	
	vector<thread> t;
	vector<TH1D> histograms;
	
	for(int i=0;i<NThreads;i++)//создаем гистограммы
	{
		histograms.emplace_back(TString::Format("h_%d",i),TString::Format("h_%d",i),10000,0,10000);
	}
	
	for(int i=0;i<NThreads;i++)//запускаем потоки
	{
		t.emplace_back(FillInThread,&dm,&histograms[i],i);
	}
	for(int i=0;i<NThreads;i++)//останавливаем по завершении
	{
		t[i].join();
	}
	
	for(unsigned int i=1;i<NThreads;i++)//суммируем гистограммы
	{
		histograms[0].Add(&histograms[i]);
	}
	
	/*получаем счетчики
	можно обратиться напрямую к Counter::vector<list<Long64_t> > Tstmp;
	и Counter::vector<list<Long64_t> > HWCounts;
	 */
	vector<TGraph> Counts=df->CountObject.ToTGraphs();
	
	TFile f("Spectrum_MT.root","recreate");
	f.WriteTObject(&histograms[0]);
	f.Close();
}
