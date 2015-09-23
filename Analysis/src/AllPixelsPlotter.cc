#include "AllPixelsPlotter.h"

//===========================================

AllPixelsPlotter::AllPixelsPlotter(std::vector<std::string> fileNames, std::string plotPath) :
    m_FileNames(fileNames),
    m_PlotPath(plotPath)
{
    this->ReadData();
    this->PlotToTPulseHeightAllSets(m_ToTs,"TargetTot");
    this->PlotToTPulseHeightAllSets(m_ToTs_X,"Tot_X");
    this->PlotToTPulseHeightAllSets(m_ToTs_X_Y,"Tot_X_Y");
    this->PlotToTPulseHeightAllSets(m_ToTs_Y,"Tot");
    this->PlotToTPulseHeightAllSets(m_ToTs_nX_Y,"Tot_nX_Y");
    this->PlotToTPulseHeightAllSets(m_ToTs_nX,"Tot_nX");
    this->PlotRiseTimePulseHeightAllSets();

    for (std::vector<int>::const_iterator it = m_UniqueSetNumbers.begin(); it != m_UniqueSetNumbers.end(); ++it)
    {
        int setNumber = *it;
        this->PlotToTPulseHeightAllPixelsInSet(setNumber,m_ToTs,"TargetTot");
        this->PlotToTPulseHeightAllPixelsInSet(setNumber,m_ToTs_X,"Tot_X");
        this->PlotRiseTimePulseHeightAllPixelsInSet(setNumber);
    }
}

//===========================================

AllPixelsPlotter::~AllPixelsPlotter()
{
}

//===========================================

void AllPixelsPlotter::ReadData()
{
    for(std::vector<std::string>::iterator it = m_FileNames.begin(); it != m_FileNames.end(); ++it)
    {
        std::string fileName = *it;
        this->ReadFileData(fileName);
    }
}

//===========================================

void AllPixelsPlotter::ReadFileData(std::string fileName)
{
    std::string setNumber;
    int pixelNumber;
    int eventNumber;
    int tots;
    int tots_X;
    int tots_X_Y;
    int tots_Y;
    int tots_nX_Y;
    int tots_nX;
    float simplyPulseHeight;
    float fittedPulseHeight;
    float troughStartTime;
    float troughTime;
    float tenPercentPeakTroughStartTime;
    float tenPercentPeakTroughEndTime;
    float ninetyPercentPeakTroughStartTime;
    float ninetyPercentPeakTroughEndTime;
    float frameCountOnDevice;

    std::ifstream resultsFile;
    resultsFile.open(fileName.c_str());

    while (resultsFile >> setNumber >> pixelNumber >> eventNumber >> tots >> tots_X >> tots_X_Y >> tots_Y >> tots_nX_Y >> tots_nX >> simplyPulseHeight >> fittedPulseHeight >> troughStartTime >> troughTime >> tenPercentPeakTroughStartTime >> tenPercentPeakTroughEndTime >> ninetyPercentPeakTroughStartTime >> ninetyPercentPeakTroughEndTime >> frameCountOnDevice)
    {
        float riseTime = 1e9 * (ninetyPercentPeakTroughStartTime - tenPercentPeakTroughStartTime);

        // Define NoCLICpix sample as SET0
        if (setNumber != "NoCLICpix")
            m_SetNumber.push_back(atoi(setNumber.c_str()));
        else
            m_SetNumber.push_back(0);

        m_RiseTime.push_back(riseTime);
        m_PixelNumber.push_back(pixelNumber);
        m_EventNumber.push_back(eventNumber);
        m_ToTs.push_back(tots);
        m_ToTs_X.push_back(tots_X);
        m_ToTs_X_Y.push_back(tots_X_Y);
        m_ToTs_Y.push_back(tots_Y);
        m_ToTs_nX_Y.push_back(tots_nX_Y);
        m_ToTs_nX.push_back(tots_nX);
        m_SimplyPulseHeight.push_back(simplyPulseHeight*1000);
        m_FittedPulseHeight.push_back(fittedPulseHeight*1000);
        m_TroughStartTime.push_back(troughStartTime);
        m_TroughTime.push_back(troughTime);
        m_TenPercentPeakTroughStartTime.push_back(tenPercentPeakTroughStartTime);
        m_TenPercentPeakTroughEndTime.push_back(tenPercentPeakTroughEndTime);
        m_NinetyPercentPeakTroughStartTime.push_back(ninetyPercentPeakTroughStartTime);
        m_NinetyPercentPeakTroughEndTime.push_back(ninetyPercentPeakTroughEndTime);
        m_FrameCountOnDevice.push_back(frameCountOnDevice);
    }

    resultsFile.close();

    this->UniqueSetClassification();
    this->UniquePixelClassification();
}

//===========================================

void AllPixelsPlotter::UniqueSetClassification()
{
    for (std::vector<int>::const_iterator it = m_SetNumber.begin(); it != m_SetNumber.end(); ++it)
    {
        int setNumber = *it;
        if (std::find(m_UniqueSetNumbers.begin(), m_UniqueSetNumbers.end(), setNumber) == m_UniqueSetNumbers.end())
        {
            m_UniqueSetNumbers.push_back(setNumber); 
        }
    }
}

//===========================================

void AllPixelsPlotter::UniquePixelClassification()
{
    for (std::vector<int>::const_iterator it = m_PixelNumber.begin(); it != m_PixelNumber.end(); ++it)
    {
        int pixelNumber = *it;
        if (std::find(m_UniquePixelNumbers.begin(), m_UniquePixelNumbers.end(), pixelNumber) == m_UniquePixelNumbers.end())
        {
            m_UniquePixelNumbers.push_back(pixelNumber);
        }
    }
}

//===========================================

void AllPixelsPlotter::PlotToTPulseHeightAllSets(std::vector<int> activeToTs, std::string totDefinition)
{
    TCanvas *pCanvas = new TCanvas("Name","Title",200,10,3000,2500);
    pCanvas->cd();
    TLegend *pTLegend = new TLegend(0.1,0.7,0.3,0.9);

    for (std::vector<int>::const_iterator it = m_UniqueSetNumbers.begin(); it != m_UniqueSetNumbers.end(); ++it)
    {
        int setNumber = *it;
        TH2F *pTH2F = new TH2F("TH2F","ToT vs Pulse Height", 175, 0, 700, 16, -0.5, 15.5);

        for (std::vector<int>::const_iterator iter = m_SetNumber.begin(); iter != m_SetNumber.end(); ++iter)
        {
            if (setNumber != *iter)
                continue;

            int counter = iter - m_SetNumber.begin();
            int tot = activeToTs.at(counter);
            float pulseHeight = m_FittedPulseHeight.at(counter);
            pTH2F->Fill(pulseHeight,tot);
        }

        std::vector<float> meanToT;
        std::vector<float> pulseHeight;

        TH1D *pTH1D_ProfileX = pTH2F->ProfileX();
        for (int k = 0; k < pTH1D_ProfileX->GetNbinsX(); k++)
        {
            if (pTH1D_ProfileX->GetBinContent(k) != 0)
            {
                meanToT.push_back(pTH1D_ProfileX->GetBinContent(k));
                pulseHeight.push_back(pTH1D_ProfileX->GetBinCenter(k));
            }
        }

        if (meanToT.empty())
            continue;

        TVectorT<float> tVectorToT(meanToT.size(), &meanToT[0]);
        TVectorT<float> tVectorPulseHeight(pulseHeight.size(), &pulseHeight[0]);

        TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
        pTGraph->SetTitle("ToT vs Pulse Height");
        pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
        pTGraph->GetXaxis()->SetLimits(0.0,700);
        pTGraph->GetXaxis()->SetRangeUser(0.0,700);
        pTGraph->GetYaxis()->SetTitle("ToT");
        pTGraph->GetYaxis()->SetLimits(0.0,16.0);
        pTGraph->GetYaxis()->SetRangeUser(0.0,16.0);
        int setCounter = it - m_UniqueSetNumbers.begin();
        pTGraph->SetLineColor(this->RootColor(setCounter));
        pTGraph->SetMarkerColor(this->RootColor(setCounter));
        pTGraph->SetMarkerStyle(this->RootMarker(setCounter));

        std::string label = this->SampleLabel(setNumber);
        pTLegend->AddEntry(pTGraph,label.c_str(),"p");

        pCanvas->cd();
        if (it - m_UniqueSetNumbers.begin() == 0)
            pTGraph->Draw("AP");
        else
            pTGraph->Draw("same P");

        delete pTH2F, pTH1D_ProfileX;
    }

    pCanvas->cd();
    pTLegend->Draw("same");

    std::string pngFileName(m_PlotPath + "AllSets/AllSETs_" + totDefinition + "_PulseHeight.png");
    std::string dotCFileName(m_PlotPath + "AllSets/AllSETs_" + totDefinition + "_PulseHeight.C");
    pCanvas->SaveAs(pngFileName.c_str());
    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

void AllPixelsPlotter::PlotToTPulseHeightAllPixelsInSet(int activeSetNumber, std::vector<int> activeToTs, std::string totDefinition)
{
    TCanvas *pCanvas = new TCanvas("Name","Title",200,10,3000,2500);
    pCanvas->cd();
    TLegend *pTLegend = new TLegend(0.1,0.7,0.3,0.9);

    for (std::vector<int>::const_iterator it = m_UniquePixelNumbers.begin(); it != m_UniquePixelNumbers.end(); ++it)
    {
        int pixelNumber = *it;
        TH2F *pTH2F = new TH2F("TH2F","ToT vs Pulse Height", 175, 0, 700, 16, -0.5, 15.5);

        for (std::vector<int>::const_iterator iter = m_PixelNumber.begin(); iter != m_PixelNumber.end(); ++iter)
        {
            if (pixelNumber != *iter)
                continue;

            int counter = iter - m_PixelNumber.begin();
            int setNumber = m_SetNumber.at(counter);

            if (setNumber != activeSetNumber)
                continue;

            int tot = activeToTs.at(counter);
            float pulseHeight = m_FittedPulseHeight.at(counter);
            pTH2F->Fill(pulseHeight,tot);
        }

        std::vector<float> meanToT;
        std::vector<float> pulseHeight;

        TH1D *pTH1D_ProfileX = pTH2F->ProfileX();
        for (int k = 0; k < pTH1D_ProfileX->GetNbinsX(); k++)
        {
            if (pTH1D_ProfileX->GetBinContent(k) != 0)
            {
                meanToT.push_back(pTH1D_ProfileX->GetBinContent(k));
                pulseHeight.push_back(pTH1D_ProfileX->GetBinCenter(k));
            }
        }

        if (meanToT.empty())
            continue;

        TVectorT<float> tVectorToT(meanToT.size(), &meanToT[0]);
        TVectorT<float> tVectorPulseHeight(pulseHeight.size(), &pulseHeight[0]);

        TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
        pTGraph->SetTitle("ToT vs Pulse Height");
        pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
        pTGraph->GetXaxis()->SetLimits(0.0,700);
        pTGraph->GetXaxis()->SetRangeUser(0.0,700);
        pTGraph->GetYaxis()->SetTitle("ToT");
        pTGraph->GetYaxis()->SetLimits(0.0,16.0);
        pTGraph->GetYaxis()->SetRangeUser(0.0,16.0);
        int setCounter = it - m_UniqueSetNumbers.begin();
        pTGraph->SetLineColor(this->RootColor(setCounter));
        pTGraph->SetMarkerColor(this->RootColor(setCounter));
        pTGraph->SetMarkerStyle(this->RootMarker(setCounter));

        std::string label = "Pixel " + IntToString(pixelNumber);
        pTLegend->AddEntry(pTGraph,label.c_str(),"p");

        pCanvas->cd();
        if (it - m_UniquePixelNumbers.begin() == 0)
            pTGraph->Draw("AP");
        else
            pTGraph->Draw("same P");

        delete pTH2F, pTH1D_ProfileX;
    }

    pCanvas->cd();
    pTLegend->Draw("same");

    std::string pngFileName(m_PlotPath + "AllPixels/AllPixels_Set" + IntToString(activeSetNumber) + "_" + totDefinition + "_PulseHeight.png");
    std::string dotCFileName(m_PlotPath + "AllPixels/AllPixels_Set" + IntToString(activeSetNumber) + "_" + totDefinition + "_PulseHeight.C");
    pCanvas->SaveAs(pngFileName.c_str());
    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

void AllPixelsPlotter::PlotRiseTimePulseHeightAllSets()
{
    TCanvas *pCanvas = new TCanvas("Name","Title",200,10,3000,2500);
    pCanvas->cd();
    TLegend *pTLegend = new TLegend(0.6,0.6,0.9,0.9);

    for (std::vector<int>::const_iterator it = m_UniqueSetNumbers.begin(); it != m_UniqueSetNumbers.end(); ++it)
    {
        int setNumber = *it;
        TH2F *pTH2F = new TH2F("TH2F","Rise Time vs Pulse Height", 70, 0, 700, 120, 0, 1200);

        for (std::vector<int>::const_iterator iter = m_SetNumber.begin(); iter != m_SetNumber.end(); ++iter)
        {
            if (setNumber != *iter)
                continue;

            int counter = iter - m_SetNumber.begin();
            int riseTime = m_RiseTime.at(counter);
            float pulseHeight = m_FittedPulseHeight.at(counter);
            pTH2F->Fill(pulseHeight,riseTime);
        }

        std::vector<float> meanRiseTime;
        std::vector<float> pulseHeight;

        TH1D *pTH1D_ProfileX = pTH2F->ProfileX();
        for (int k = 0; k < pTH1D_ProfileX->GetNbinsX(); k++)
        {
            if (pTH1D_ProfileX->GetBinContent(k) != 0)
            {
                meanRiseTime.push_back(pTH1D_ProfileX->GetBinContent(k));
                pulseHeight.push_back(pTH1D_ProfileX->GetBinCenter(k));
            }
        }

        if (meanRiseTime.empty())
            continue;

        TVectorT<float> tVectorToT(meanRiseTime.size(), &meanRiseTime[0]);
        TVectorT<float> tVectorPulseHeight(pulseHeight.size(), &pulseHeight[0]);

        TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
        pTGraph->SetTitle("Rise Time vs Pulse Height");
        pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
        pTGraph->GetXaxis()->SetLimits(0.0,700);
        pTGraph->GetXaxis()->SetRangeUser(0.0,700);
        pTGraph->GetYaxis()->SetTitle("Rise Time");
        pTGraph->GetYaxis()->SetLimits(0.0,1200.0);
        pTGraph->GetYaxis()->SetRangeUser(0.0,1200.0);
        pTGraph->GetYaxis()->SetTitleOffset(1.4);
        int setCounter = it - m_UniqueSetNumbers.begin();
        pTGraph->SetLineColor(this->RootColor(setCounter));
        pTGraph->SetMarkerColor(this->RootColor(setCounter));
        pTGraph->SetMarkerStyle(this->RootMarker(setCounter));

        std::string label = this->SampleLabel(setNumber);
        pTLegend->AddEntry(pTGraph,label.c_str(),"p");

        pCanvas->cd();
        if (it - m_UniqueSetNumbers.begin() == 0)
            pTGraph->Draw("AP");
        else
            pTGraph->Draw("same P");

        delete pTH2F, pTH1D_ProfileX;
    }

    pCanvas->cd();
    pTLegend->Draw("same");
    std::string pngFileName(m_PlotPath + "AllSets/AllSETs_RiseTime_PulseHeight.png");
    std::string dotCFileName(m_PlotPath + "AllSets/AllSETs_RiseTime_PulseHeight.C");
    pCanvas->SaveAs(pngFileName.c_str());
    pCanvas->SaveAs(dotCFileName.c_str());
}

//===========================================

void AllPixelsPlotter::PlotRiseTimePulseHeightAllPixelsInSet(int activeSetNumber)
{
    TCanvas *pCanvas = new TCanvas("Name","Title",200,10,3000,2500);
    pCanvas->cd();
    TLegend *pTLegend = new TLegend(0.1,0.7,0.3,0.9);

    for (std::vector<int>::const_iterator it = m_UniquePixelNumbers.begin(); it != m_UniquePixelNumbers.end(); ++it)
    {
        int pixelNumber = *it;
        TH2F *pTH2F = new TH2F("TH2F","Rise Time vs Pulse Height", 175, 0, 700, 16, -0.5, 15.5);

        for (std::vector<int>::const_iterator iter = m_PixelNumber.begin(); iter != m_PixelNumber.end(); ++iter)
        {
            if (pixelNumber != *iter)
                continue;

            int counter = iter - m_PixelNumber.begin();
            int setNumber = m_SetNumber.at(counter);

            if (setNumber != activeSetNumber)
                continue;

            int riseTime = m_RiseTime.at(counter);
            float pulseHeight = m_FittedPulseHeight.at(counter);
            pTH2F->Fill(pulseHeight,riseTime);
        }

        std::vector<float> meanRiseTime;
        std::vector<float> pulseHeight;

        TH1D *pTH1D_ProfileX = pTH2F->ProfileX();
        for (int k = 0; k < pTH1D_ProfileX->GetNbinsX(); k++)
        {
            if (pTH1D_ProfileX->GetBinContent(k) != 0)
            {
                meanRiseTime.push_back(pTH1D_ProfileX->GetBinContent(k));
                pulseHeight.push_back(pTH1D_ProfileX->GetBinCenter(k));
            }
        }

        if (meanRiseTime.empty())
            continue;

        TVectorT<float> tVectorRiseTime(meanRiseTime.size(), &meanRiseTime[0]);
        TVectorT<float> tVectorPulseHeight(pulseHeight.size(), &pulseHeight[0]);

        TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorRiseTime);
        pTGraph->SetTitle("RiseTime vs Pulse Height");
        pTGraph->GetXaxis()->SetTitle("Pulse Height [mV]");
        pTGraph->GetXaxis()->SetLimits(0.0,700);
        pTGraph->GetXaxis()->SetRangeUser(0.0,700);
        pTGraph->GetYaxis()->SetTitle("Rise Time");
        pTGraph->GetYaxis()->SetLimits(0.0,1200.0);
        pTGraph->GetYaxis()->SetRangeUser(0.0,1200.0);
        pTGraph->GetYaxis()->SetTitleOffset(1.4);
        int setCounter = it - m_UniqueSetNumbers.begin();
        pTGraph->SetLineColor(this->RootColor(setCounter));
        pTGraph->SetMarkerColor(this->RootColor(setCounter));
        pTGraph->SetMarkerStyle(this->RootMarker(setCounter));

        std::string label = "Pixel " + IntToString(pixelNumber);
        pTLegend->AddEntry(pTGraph,label.c_str(),"p");

        pCanvas->cd();
        if (it - m_UniquePixelNumbers.begin() == 0)
            pTGraph->Draw("AP");
        else
            pTGraph->Draw("same P");

        delete pTH2F, pTH1D_ProfileX;
    }

    pCanvas->cd();
    pTLegend->Draw("same");

    std::string pngFileName(m_PlotPath + "AllPixels/AllPixels_Set" + IntToString(activeSetNumber) + "_RiseTime_PulseHeight.png");
    std::string dotCFileName(m_PlotPath + "AllPixels/AllPixels_Set" + IntToString(activeSetNumber) + "_RiseTime_PulseHeight.C");
    pCanvas->SaveAs(pngFileName.c_str());
    pCanvas->SaveAs(dotCFileName.c_str());
}

//============================================================

int AllPixelsPlotter::RootColor(int a)
{
    static const int rootColorArray[] = {1,2,3,4,6,7,8,9};
    return rootColorArray[(a % (sizeof(rootColorArray) / sizeof(int)))];
}

//============================================================

int AllPixelsPlotter::RootMarker(int a)
{
    static const int rootMarkerArray[] = {20,21,22,23};
    if (a <= 7)
        return rootMarkerArray[0];
    else if (a <= 15)
        return rootMarkerArray[1];
    else if (a <= 23)
        return rootMarkerArray[2];
    else if (a <= 31)
        return rootMarkerArray[3];
}
                                                  
//===========================================

std::string AllPixelsPlotter::SampleLabel(int setNumber)
{
    std::string set9 = "Set 9";
    std::string set10 = "Set 10";
    std::string set12 = "Set 12";
    std::string set13 = "Set 13";
    std::string set14 = "Set 14";
    std::string set15 = "Set 15";
    std::string set16 = "Set 16";
    std::string noCLICpix = "No CLICpix";
    if (setNumber == 9)
        return set9;
    else if (setNumber == 10)
        return set10;
    else if (setNumber == 12)
        return set12;
    else if (setNumber == 13)
        return set13;
    else if (setNumber == 14)
        return set14;
    else if (setNumber == 15)
        return set15;
    else if (setNumber == 16)
        return set16;
    else if (setNumber == 0)
        return noCLICpix;
}

//===========================================

std::string AllPixelsPlotter::IntToString(int a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//===========================================

/*
void AllPixelsPlotter::Plot()
{
    std::string graphTitle, pngFileName_TGraph, dotCFileName_TGraph, pngFileName_TH2F, dotCFileName_TH2F, graphTitleRiseTime, pngFileName_TGraph_RiseTime, dotCFileName_TGraph_RiseTime, pngFileName_TH2F_RiseTime, dotCFileName_TH2F_RiseTime;

    if (pixelNumber == 0)
    {
        graphTitle = totTitle + " vs Pulse Height for SET" + setNumber;
        pngFileName_TGraph = pathForPlots + "/SET" + setNumber + "_TGraph_" + briefToTTitle + "vsPulseHeight.png";
        dotCFileName_TGraph = pathForPlots + "/SET" + setNumber + "_TGraph_" + briefToTTitle + "vsPulseHeight.C";
        pngFileName_TH2F = pathForPlots + "/SET" + setNumber + "_TH2F_" + briefToTTitle + "vsPulseHeight.png";
        dotCFileName_TH2F = pathForPlots + "/SET" + setNumber + "_TH2F_" + briefToTTitle + "vsPulseHeight.C";

        graphTitleRiseTime = "Rise Time vs Pulse Height for SET" + setNumber;
        pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_TGraph_RiseTimevsPulseHeight.png";
        dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_TGraph_RiseTimevsPulseHeight.C";
        pngFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_TH2F_RiseTimevsPulseHeight.png";
        dotCFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_TH2F_RiseTimevsPulseHeight.C";
    }
    else 
    {
        graphTitle = totTitle + " vs Pulse Height for SET" + setNumber + " Pixel " + toString(pixelNumber);
        pngFileName_TGraph = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_" + briefToTTitle + "vsPulseHeight.png";
        dotCFileName_TGraph = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_" + briefToTTitle + "vsPulseHeight.C";
        pngFileName_TH2F = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_" + briefToTTitle + "vsPulseHeight.png";
        dotCFileName_TH2F = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_" + briefToTTitle + "vsPulseHeight.C";

        graphTitleRiseTime = "Rise Time vs Pulse Height for SET" + setNumber + " Pixel " + toString(pixelNumber);
        pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_RiseTimevsPulseHeight.png";
        dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_RiseTimevsPulseHeight.C";
        pngFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_RiseTimevsPulseHeight.png";
        dotCFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_RiseTimevsPulseHeight.C";
    }

    TGraph *pTGraph_ToTvsPulseHeight = new TGraph();
    pTGraph_ToTvsPulseHeight->SetTitle(graphTitle.c_str());
    pTGraph_ToTvsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
    pTGraph_ToTvsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
    pTGraph_ToTvsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
    pTGraph_ToTvsPulseHeight->GetYaxis()->SetTitle("ToT");
    pTGraph_ToTvsPulseHeight->GetYaxis()->SetLimits(0.0,16.0);
    pTGraph_ToTvsPulseHeight->GetYaxis()->SetRangeUser(0.0,16.0);

    TH2F *pTH2F_ToTvsPulseHeight = new TH2F("TH2F","ToT vs Pulse Height 2D Histogram", 175, 0, 0.7, 16, -0.5, 15.5);
    pTH2F_ToTvsPulseHeight->SetTitle(graphTitle.c_str());
    pTH2F_ToTvsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
    pTH2F_ToTvsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
    pTH2F_ToTvsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
    pTH2F_ToTvsPulseHeight->GetYaxis()->SetTitle("ToT");
//    pTH2F_ToTvsPulseHeight->GetYaxis()->SetLimits(0.0,16.0);
    pTH2F_ToTvsPulseHeight->GetYaxis()->SetRangeUser(0.0,16.0);

    TGraph *pTGraph_RiseTimevsPulseHeight = new TGraph();
    pTGraph_RiseTimevsPulseHeight->SetTitle(graphTitleRiseTime.c_str());
    pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
    pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
    pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
    pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetTitle("Rise Time [ns]");
    pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetTitleOffset(1.4);
    pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetLimits(0.0,1200);
    pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetRangeUser(0.0,1200);

    TH2F *pTH2F_RiseTimevsPulseHeight = new TH2F("TH2F","Rise Time vs Pulse Height 2D Histogram", 70, 0, 0.7, 120, 0, 1200);
    pTH2F_RiseTimevsPulseHeight->SetTitle(graphTitleRiseTime.c_str());
    pTH2F_RiseTimevsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
    pTH2F_RiseTimevsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
    pTH2F_RiseTimevsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
    pTH2F_RiseTimevsPulseHeight->GetYaxis()->SetTitle("Rise Time [ns]");
    pTH2F_RiseTimevsPulseHeight->GetYaxis()->SetTitleOffset(1.4);
    pTH2F_RiseTimevsPulseHeight->GetYaxis()->SetLimits(0.0,1200);
    pTH2F_RiseTimevsPulseHeight->GetYaxis()->SetRangeUser(0.0,1200);

    int setNumberFromFile, pixelNumberFromFile, eventNumber, frameCount;
    float tot, basicPulseHeightFromFile, fitPulseHeightFromFile, troughStartTime, troughTime, tenPercentStart, tenPercentEnd, ninetyPercentStart, ninetyPercentEnd, totPlusx, totPlusxPlusy, totPlusy, totMinusxPlusy, totMinusx;

    float desiredToT;

    ifstream resultsFile;
    resultsFile.open(resultsFileName.c_str());

//if (setNumber == "16")
//{
    resultsFile.close();

    TH1D *pTH1D_ProfileX_ToTvsPulseHeight = pTH2F_ToTvsPulseHeight->ProfileX();
    for (int k = 0; k < pTH1D_ProfileX_ToTvsPulseHeight->GetNbinsX(); k++)
    {
        if (pTH1D_ProfileX_ToTvsPulseHeight->GetBinContent(k) != 0)
        {
            meanToT->push_back(pTH1D_ProfileX_ToTvsPulseHeight->GetBinContent(k));
            pulseHeight->push_back(pTH1D_ProfileX_ToTvsPulseHeight->GetBinCenter(k));
        } 
    }

    if (totCounter == 0)
    {
        TH1D *pTH1D_ProfileX_RiseTimevsPulseHeight = pTH2F_RiseTimevsPulseHeight->ProfileX();
        for (int k = 0; k < pTH1D_ProfileX_RiseTimevsPulseHeight->GetNbinsX(); k++)
        {
            if (pTH1D_ProfileX_RiseTimevsPulseHeight->GetBinContent(k) != 0)
            {
                meanRiseTime->push_back(pTH1D_ProfileX_RiseTimevsPulseHeight->GetBinContent(k));
                pulseHeightForRiseTime->push_back(pTH1D_ProfileX_RiseTimevsPulseHeight->GetBinCenter(k));
            }
        }
    }

    if (!meanToT->empty())
    {
        TVectorT<float> tVectorToT(meanToT->size(), &meanToT->at(0));
        TVectorT<float> tVectorPulseHeight(pulseHeight->size(), &pulseHeight->at(0));

        TCanvas *pCanvas_TGraph = new TCanvas("PixelCalibrationTGraphCanvas","Pixel Calibration TGraph Canvas",200,10,3000,2500);
        pCanvas_TGraph->cd();
        pTGraph_ToTvsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
        pTGraph_ToTvsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
        pTGraph_ToTvsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
        pTGraph_ToTvsPulseHeight->GetYaxis()->SetTitle("ToT");
        pTGraph_ToTvsPulseHeight->GetYaxis()->SetLimits(0.0,16.0);
        pTGraph_ToTvsPulseHeight->GetYaxis()->SetRangeUser(0.0,16.0);
        pTGraph_ToTvsPulseHeight->Draw("AP");
        pCanvas_TGraph->SaveAs(pngFileName_TGraph.c_str());
        pCanvas_TGraph->SaveAs(dotCFileName_TGraph.c_str());
        delete pTGraph_ToTvsPulseHeight;
        delete pCanvas_TGraph;

        TCanvas *pCanvas_TH2F = new TCanvas("PixelCalibrationTH2FCanvas","Pixel Calibration TH2F Canvas",200,10,3000,2500);
        pCanvas_TH2F->cd();
        pTH2F_ToTvsPulseHeight->Draw("COLZ");
        TGraph *pTGraph = new TGraph (tVectorPulseHeight,tVectorToT);
        pTGraph->SetLineWidth(4);
        pTGraph->SetLineStyle(2);
        pTGraph->SetLineColor(kOrange);
        pTGraph->Draw("LP same");
        pCanvas_TH2F->SaveAs(pngFileName_TH2F.c_str());
        pCanvas_TH2F->SaveAs(dotCFileName_TH2F.c_str());
        delete pTGraph;
        delete pTH2F_ToTvsPulseHeight;
        delete pCanvas_TH2F;
    }

    if (totCounter == 0)
    {
        if (!meanRiseTime->empty())
        {
            TVectorT<float> tVectorRiseTime(meanRiseTime->size(), &meanRiseTime->at(0));
            TVectorT<float> tVectorPulseHeightForRiseTime(pulseHeightForRiseTime->size(), &pulseHeightForRiseTime->at(0));

            TCanvas *pCanvas_TGraph_RiseTime = new TCanvas("PixelCalibrationTGraphCanvasRiseTime","Pixel Calibration TGraph Canvas Rise Time",200,10,3000,2500);
            pCanvas_TGraph_RiseTime->cd();
//           pCanvas_TGraph_RiseTime->SetLeftMargin(0.2);
            pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetTitle("Pulse Height [V]");
            pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetLimits(0.0,0.7);
            pTGraph_RiseTimevsPulseHeight->GetXaxis()->SetRangeUser(0.0,0.7);
            pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetTitle("Rise Time [ns]");
            pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetTitleOffset(1.4);
            pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetLimits(0.0,1200);
            pTGraph_RiseTimevsPulseHeight->GetYaxis()->SetRangeUser(0.0,1200);
            pTGraph_RiseTimevsPulseHeight->Draw("AP");
            pCanvas_TGraph_RiseTime->SaveAs(pngFileName_TGraph_RiseTime.c_str());
            pCanvas_TGraph_RiseTime->SaveAs(dotCFileName_TGraph_RiseTime.c_str());
            delete pTGraph_RiseTimevsPulseHeight;
            delete pCanvas_TGraph_RiseTime;

            TCanvas *pCanvas_TH2F_RiseTime = new TCanvas("PixelCalibrationTH2FCanvasRiseTime","Pixel Calibration TH2F Canvas Rise Time",200,10,3000,2500);
            pCanvas_TH2F_RiseTime->cd();
//            pCanvas_TH2F_RiseTime->SetLeftMargin(0.2);
            pTH2F_RiseTimevsPulseHeight->Draw("COLZ");
            TGraph *pTGraphRiseTime = new TGraph (tVectorPulseHeightForRiseTime,tVectorRiseTime);
            pTGraphRiseTime->SetLineWidth(10);
            pTGraphRiseTime->SetLineStyle(2);
            pTGraphRiseTime->SetLineColor(kBlack);
            pTGraphRiseTime->Draw("LP same");
            pCanvas_TH2F_RiseTime->SaveAs(pngFileName_TH2F_RiseTime.c_str());
            pCanvas_TH2F_RiseTime->SaveAs(dotCFileName_TH2F_RiseTime.c_str());
            delete pTGraphRiseTime;
            delete pTH2F_RiseTimevsPulseHeight;
            delete pCanvas_TH2F_RiseTime;
        }
    }

}*/
