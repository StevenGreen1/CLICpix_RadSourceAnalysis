using namespace std;

void display(){
	
	gROOT->ProcessLine(".x lhcbStyle.C");
	gStyle->SetPalette(1);
	
//	string rootDir = "/Users/danielhynds/Desktop/Work/2014_clicTestbeamTest/scope/calibrationSr90/";
	string rootDir = "./";
	
	TCanvas* canv = new TCanvas();

	plotData(rootDir,canv,"calibrationSET9", 1,20,1.,true); //ideal
	plotData(rootDir,canv,"calibrationSET10",4,20,1.); //quarter
	plotData(rootDir,canv,"calibrationSET12",2,20,1.); //thick
	plotData(rootDir,canv,"calibrationSET13",1,22,1.); //ideal
	plotData(rootDir,canv,"calibrationSET14",8,20,1.); //half
	
	return;
}

plotData(string rootDir, TCanvas* canv, string file, int color, int style, double factor, bool first=false){
	string filename = rootDir + file + ".dat";
	ifstream inFile; string line;
	int row(0),col(0),tot(0),count(0);
	float amp(0);
	
	TH2F* spectrum = new TH2F("spectrum","spectrum",200,0,800,200,0,20);
	TH1F* hvcmosSpectrum = new TH1F("hvcmosSpectrum","hvcmosSpectrum",200,0,800);
	TH1F* clicpixSpectrum = new TH1F("clicpixSpectrum","clicpixSpectrum",20,0,20);
	TH1F* hvcmosToT = new TH1F("hvcmosToT","hvcmosToT",200,0,800);
	TH1F* hvcmosCounts = new TH1F("hvcmosCounts","hvcmosCounts",200,0,800);
	
	// Open file
	inFile.open(filename.c_str());
	while(inFile.good()){
		// Loop over lines
		getline(inFile,line);
		// If this is a header ignore it
		size_t check1 = line.find("==");
		if(check1!=string::npos){
			//			sscanf(line.c_str(),"%*s %*i %f %*s",&amp);
			continue;
		}
		// Get the data
		//		sscanf(line.c_str(),"%i %i %i %i",&col,&row,&tot,&count);
		sscanf(line.c_str(),"%f %i %i",&amp,&tot,&count);
		// Fill the tot spectrum if only one hit registered
		//		if(count == 1 && row == 0 && col == 3) spectrum->Fill(1000.*amp,tot);
		//			if(count == 1){
		//      if(count == 1 && (1000.*amp > 140.)){
		if(count == 1 ){
			
			if( (1000.*factor*amp < 100.) && (tot > 10.) ) continue;
			//        spectrum->Fill(1000.*2.*amp,tot);
			hvcmosToT->Fill(1000.*factor*amp,tot);
			hvcmosCounts->Fill(1000.*factor*amp,1);
			hvcmosSpectrum->Fill(1000.*amp);
			clicpixSpectrum->Fill(tot);
		}
	}
	
	for(double mv=55;mv<800.;mv+=4.){
		spectrum->Fill(mv, (double)hvcmosToT->GetBinContent(hvcmosToT->FindBin(mv)) / (double)hvcmosCounts->GetBinContent(hvcmosCounts->FindBin(mv)) );
	}
	canv->cd();
	spectrum->GetXaxis()->SetTitle("Pulse height (mV)");
	spectrum->GetYaxis()->SetTitle("ToT");
	spectrum->SetMarkerColor(color);
	spectrum->SetMarkerStyle(style);
	if(first)spectrum->DrawCopy("p");
	if(!first)spectrum->DrawCopy("p,same");
	delete spectrum;
	
	//		canv2->cd();
	//		hvcmosSpectrum->GetXaxis()->SetTitle("Pulse height (mV)");
	//		hvcmosSpectrum->SetLineColor(i+1);
	//		if(i==0)hvcmosSpectrum->DrawCopy("box");
	//		if(i!=0)hvcmosSpectrum->DrawCopy("box,same");
	delete hvcmosSpectrum;
	
	//		canv3->cd();
	//		clicpixSpectrum->GetXaxis()->SetTitle("CLICPix ToT");
	//		clicpixSpectrum->SetLineColor(i+1);
	//		if(i==0)clicpixSpectrum->DrawCopy("box");
	//		if(i!=0)clicpixSpectrum->DrawCopy("box,same");
	delete clicpixSpectrum;
	delete hvcmosToT;
	delete hvcmosCounts;
}


string makestring(double sigma){
	std::ostringstream strs;
	strs  << sigma;
	std::string legs = strs.str();
	return legs;
}

