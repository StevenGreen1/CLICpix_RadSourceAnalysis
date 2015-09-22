/**
 *  @file   
 * 
 *  @brief  
 */

#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TText.h"
#include "TVector.h"

void makePlots(std::string setNumber, std::string resultsFileName, int pixelNumber, std::string pathForPlots, std::vector<float> *meanToT, std::vector<float> *pulseHeight, std::vector<float> *meanRiseTime, std::vector<float> *pulseHeightForRiseTime, int totCounter, std::string briefToTTitle, std::string totTitle);

void makePlots2(std::string setNumber, std::string resultsFileName, int pixelNumber, std::string pathForPlots, std::vector<float> *meanRiseTime, std::vector<float> *pulseHeightForRiseTime);

std::string toString(int a);

int rootColor(int a);
int rootMarker(int a);

int main(int argc, char **argv)
{
    gStyle->SetOptStat(kFALSE); 

    std::string pathForPlots = "/afs/cern.ch/work/s/sgreen/PixelData/Plots";

    static const int rootColorArray[] = {1,2,3,4,5,6,7,8,9};
    static const int rootMarkerArray[] = {20,21,22,23};

    static const int pixelArray[] = {0,3,7,11,15,19,23,27,31,35,39,43,47,51,55,59};
    //static const int pixelArray[] = {0};
    //static const int pixelArray[] = {0,3,7,11};
    std::vector<int> pixel (pixelArray, pixelArray + sizeof(pixelArray) / sizeof(pixelArray[0]) );

    static const std::string setArray[] = {"NoCLICpix","9","10","12","13","14","15","16"};
    //static const std::string setArray[] = {"16"};
    std::vector<std::string> set (setArray, setArray + sizeof(setArray) / sizeof(setArray[0]) );

    static const std::string positionNamesArray[] = {"Target Pixel ToT", "Target Plus X ToT", "Target Plus X Plus Y ToT", "Target Plus Y ToT", "Targer Minus X Plus Y ToT", "Target Minus X ToT"};
    std::vector<std::string> positionNames (positionNamesArray, positionNamesArray + sizeof(positionNamesArray) / sizeof(positionNamesArray[0]) );

    static const std::string briefPositionNamesArray[] = {"TargetPixelToT", "TargetPlusXToT", "TargetPlusXPlusYToT", "TargetPlusYToT", "TargetMinusXPlusYToT", "TargetMinusXToT"};
    std::vector<std::string> briefPositionNames (briefPositionNamesArray, briefPositionNamesArray + sizeof(briefPositionNamesArray) / sizeof(briefPositionNamesArray[0]) );

    for (int positionCounter = 0; positionCounter < positionNames.size(); positionCounter++)
    {
        TCanvas *pCanvas_TGraph_Full = new TCanvas("PixelCalibrationTGraphCanvasFull","Pixel Calibration TGraph Canvas Full",200,10,3000,2500);
        TCanvas *pCanvas_TGraph_RiseTimeFull = new TCanvas("PixelCalibrationTGraphCanvasRiseTimeFull","Pixel Calibration TGraph Canvas Rise Time Full",200,10,3000,2500);
        TLegend *pTLegend_TGraph_Full = new TLegend(0.1,0.6,0.4,0.9);
        TLegend *pTLegend_TGraph_RiseTimeFull = new TLegend(0.1,0.6,0.4,0.9);
        std::string pngFileName_TGraph_Full = pathForPlots + "/AllSETs_TGraph_" + briefPositionNames.at(positionCounter) + "vsPulseHeight.png";
        std::string pngFileName_TGraph_RiseTimeFull = pathForPlots + "/AllSETs_TGraph_RiseTimevsPulseHeight.png";
        std::string dotCFileName_TGraph_Full = pathForPlots + "/AllSETs_TGraph_" + briefPositionNames.at(positionCounter) + "vsPulseHeight.C";
        std::string dotCFileName_TGraph_RiseTimeFull = pathForPlots + "/AllSETs_TGraph_RiseTimevsPulseHeight.C";

        std::vector<float> meanToT, pulseHeight, meanRiseTime, pulseHeightForRiseTime;

        TGraph *pTGraphProfile, *pTGraphProfile_RiseTime;

        for (int setCounter = 0; setCounter < set.size(); setCounter++)
        {
            std::string resultsFileName = "/afs/cern.ch/work/s/sgreen/PixelData/Results/PixelCalibration_SET" + set.at(setCounter) + "_Detailed_v2.dat";
            //std::string resultsFileName = "/afs/cern.ch/work/s/sgreen/PixelData/Results/PixelCalibration_SET" + set.at(setCounter) + "_Brief.dat";
            bool isNoCLICpix((set.at(setCounter)=="NoCLICpix") ? true : false);

            //std::cout << "isNoCLICpix     : " << isNoCLICpix << std::endl;
            //std::cout << "resultsFileName : " << resultsFileName << std::endl;

            if (!isNoCLICpix)
            {
                TCanvas *pCanvas_TGraph_AllPixels = new TCanvas("PixelCalibrationTGraphCanvasAllPixels","Pixel Calibration TGraph Canvas AllPixels",200,10,3000,2500);
                TCanvas *pCanvas_TGraph_AllPixels_RiseTime = new TCanvas("PixelCalibrationTGraphCanvasAllPixelsRiseTime","Pixel Calibration TGraph Canvas AllPixels Rise Time",200,10,3000,2500);
                TLegend *pTLegend_TGraph_AllPixels = new TLegend(0.1,0.6,0.4,0.9); 
                TLegend *pTLegend_TGraph_AllPixels_RiseTime = new TLegend(0.1,0.6,0.4,0.9); 
                std::string pngFileName_TGraph = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_" + briefPositionNames.at(positionCounter) + "vsPulseHeight.png";
                std::string pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_RiseTimevsPulseHeight.png";
                std::string dotCFileName_TGraph = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_" + briefPositionNames.at(positionCounter) + "vsPulseHeight.C";
                std::string dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_RiseTimevsPulseHeight.C";

                for (int pixelCounter = 0; pixelCounter < pixel.size(); pixelCounter++)
                {
                    makePlots(set.at(setCounter),resultsFileName,pixel.at(pixelCounter),pathForPlots, &meanToT, &pulseHeight, &meanRiseTime, &pulseHeightForRiseTime, positionCounter, briefPositionNames.at(positionCounter), positionNames.at(positionCounter));

                    TVectorT<float> tVectorToT(meanToT.size(), &meanToT[0]);
                    TVectorT<float> tVectorPulseHeight(pulseHeight.size(), &pulseHeight[0]);

                    pTGraphProfile = new TGraph (tVectorPulseHeight,tVectorToT);
                    meanToT.clear();
                    pulseHeight.clear();
                    pTGraphProfile->SetLineColor(rootColor(pixelCounter));
                    pTGraphProfile->SetMarkerColor(rootColor(pixelCounter));
                    pTGraphProfile->SetMarkerStyle(rootMarker(pixelCounter));
                    pTGraphProfile->GetXaxis()->SetTitle("Pulse Height [V]");
                    pTGraphProfile->GetXaxis()->SetLimits(0.0,0.7);
                    pTGraphProfile->GetXaxis()->SetRangeUser(0.0,0.7);
                    pTGraphProfile->GetYaxis()->SetTitle("ToT");
                    pTGraphProfile->GetYaxis()->SetLimits(0.0,16.0);
                    pTGraphProfile->GetYaxis()->SetRangeUser(0.0,16.0);

                    TVectorT<float> tVectorRiseTime(meanRiseTime.size(), &meanRiseTime[0]);
                    TVectorT<float> tVectorPulseHeightForRiseTime(pulseHeightForRiseTime.size(), &pulseHeightForRiseTime[0]);

                    pTGraphProfile_RiseTime = new TGraph (tVectorPulseHeightForRiseTime,tVectorRiseTime);
                    meanRiseTime.clear();
                    pulseHeightForRiseTime.clear();
                    pTGraphProfile_RiseTime->SetLineColor(rootColor(pixelCounter));
                    pTGraphProfile_RiseTime->SetMarkerColor(rootColor(pixelCounter));
                    pTGraphProfile_RiseTime->SetMarkerStyle(rootMarker(pixelCounter));
                    pTGraphProfile_RiseTime->GetXaxis()->SetTitle("Pulse Height [V]");
                    pTGraphProfile_RiseTime->GetXaxis()->SetLimits(0.0,0.7);
                    pTGraphProfile_RiseTime->GetXaxis()->SetRangeUser(0.0,0.7);
                    pTGraphProfile_RiseTime->GetYaxis()->SetTitle("Rise Time [ns]");
                    pTGraphProfile_RiseTime->GetYaxis()->SetLimits(0.0,1200);
                    pTGraphProfile_RiseTime->GetYaxis()->SetRangeUser(0.0,1200);           
                    pTGraphProfile_RiseTime->GetYaxis()->SetTitleOffset(1.4);

                    if (pixelCounter != 0)
                    {
                        pCanvas_TGraph_AllPixels->cd();
                        std::string legendLabel = "Pixel " + toString(pixel.at(pixelCounter));
                        pTLegend_TGraph_AllPixels->AddEntry(pTGraphProfile,legendLabel.c_str(),"pl");
                        pCanvas_TGraph_AllPixels_RiseTime->cd();
                        pTLegend_TGraph_AllPixels_RiseTime->AddEntry(pTGraphProfile_RiseTime,legendLabel.c_str(),"pl");
                        if (pixelCounter == 1)
                        {
                            pCanvas_TGraph_AllPixels->cd();
                            std::string pTGraphProfileTitle = briefPositionNames.at(positionCounter) +  " vs Pulse Height for SET" + set.at(setCounter);
                            pTGraphProfile->SetTitle(pTGraphProfileTitle.c_str());
                            pTGraphProfile->Draw("AP");
                            pCanvas_TGraph_AllPixels_RiseTime->cd();
                            std::string pTGraphProfileTitleRiseTime = "Rise Time vs Pulse Height for SET" + set.at(setCounter);
                            pTGraphProfile_RiseTime->SetTitle(pTGraphProfileTitleRiseTime.c_str());
                            pTGraphProfile_RiseTime->Draw("AP");
                        }
                        else
                        {
                            pCanvas_TGraph_AllPixels->cd();
                            pTGraphProfile->Draw("P same");
                            pCanvas_TGraph_AllPixels_RiseTime->cd();
                            pTGraphProfile_RiseTime->Draw("P same");
                        }
                    }

                    else
                    {
                        pCanvas_TGraph_Full->cd();
                        pTGraphProfile->SetLineColor(rootColor(setCounter));
                        pTGraphProfile->SetMarkerColor(rootColor(setCounter));
                        pTGraphProfile->SetMarkerStyle(rootMarker(setCounter));
                        std::string legendLabel = "SET " + set.at(setCounter);
                        pTLegend_TGraph_Full->AddEntry(pTGraphProfile,legendLabel.c_str(),"pl");

                        pCanvas_TGraph_RiseTimeFull->cd();
                        pTGraphProfile_RiseTime->SetLineColor(rootColor(setCounter));
                        pTGraphProfile_RiseTime->SetMarkerColor(rootColor(setCounter));
                        pTGraphProfile_RiseTime->SetMarkerStyle(rootMarker(setCounter));
                        pTLegend_TGraph_RiseTimeFull->AddEntry(pTGraphProfile_RiseTime,legendLabel.c_str(),"pl");

                        if (setCounter == 0)
                        {
                            pCanvas_TGraph_Full->cd();
                            std::string pTGraphProfileTitle = briefPositionNames.at(positionCounter) + " vs Pulse Height for All Samples";
                            pTGraphProfile->SetTitle(pTGraphProfileTitle.c_str());
                            pTGraphProfile->Draw("AP");

                            pCanvas_TGraph_RiseTimeFull->cd();
                            std::string pTGraphProfileTitleRiseTime = "Rise Time vs Pulse Height for All Samples";
                            pTGraphProfile_RiseTime->SetTitle(pTGraphProfileTitleRiseTime.c_str());
                            pTGraphProfile_RiseTime->Draw("AP");
                        }
                        else
                        {
                            pCanvas_TGraph_Full->cd();
                            pTGraphProfile->Draw("P same");
                            pCanvas_TGraph_RiseTimeFull->cd();
                            pTGraphProfile_RiseTime->Draw("P same");
                        }
                    }
                }
                pCanvas_TGraph_AllPixels->cd();
                pTLegend_TGraph_AllPixels->Draw("same");
                pCanvas_TGraph_AllPixels->SaveAs(pngFileName_TGraph.c_str());
                pCanvas_TGraph_AllPixels->SaveAs(dotCFileName_TGraph.c_str());

                if (positionCounter == 0)
                {
                    pCanvas_TGraph_AllPixels_RiseTime->cd();
                    pTLegend_TGraph_AllPixels_RiseTime->Draw("same");
                    pCanvas_TGraph_AllPixels_RiseTime->SaveAs(pngFileName_TGraph_RiseTime.c_str());
                    pCanvas_TGraph_AllPixels_RiseTime->SaveAs(dotCFileName_TGraph_RiseTime.c_str());
                }
//                 delete pCanvas_TGraph_AllPixels, pTLegend_TGraph_AllPixels, pCanvas_TGraph_AllPixels_RiseTime, pTLegend_TGraph_AllPixels_RiseTime;
            }

            else
            {
                TCanvas *pCanvas_TGraph_AllPixels_RiseTime = new TCanvas("PixelCalibrationTGraphCanvasAllPixelsRiseTime","Pixel Calibration TGraph Canvas AllPixels Rise Time",200,10,3000,2500);
                TLegend *pTLegend_TGraph_AllPixels_RiseTime = new TLegend(0.1,0.6,0.4,0.9);
                std::string pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_RiseTimevsPulseHeight.png";
                std::string dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + set.at(setCounter) + "AllPixels_TGraph_RiseTimevsPulseHeight.C";

                for (int pixelCounter = 0; pixelCounter < pixel.size(); pixelCounter++)
                {
                    makePlots2(set.at(setCounter),resultsFileName,pixel.at(pixelCounter),pathForPlots, &meanRiseTime, &pulseHeightForRiseTime);

                    TVectorT<float> tVectorRiseTime(meanRiseTime.size(), &meanRiseTime[0]);
                    TVectorT<float> tVectorPulseHeightForRiseTime(pulseHeightForRiseTime.size(), &pulseHeightForRiseTime[0]);

                    //std::cout << "meanRiseTime.size() : " << meanRiseTime.size() << std::endl;

                    pTGraphProfile_RiseTime = new TGraph (tVectorPulseHeightForRiseTime,tVectorRiseTime);
                    meanRiseTime.clear();
                    pulseHeightForRiseTime.clear();
                    pTGraphProfile_RiseTime->SetLineColor(rootColor(pixelCounter));
                    pTGraphProfile_RiseTime->SetMarkerColor(rootColor(pixelCounter));
                    pTGraphProfile_RiseTime->SetMarkerStyle(rootMarker(pixelCounter));
                    pTGraphProfile_RiseTime->GetXaxis()->SetTitle("Pulse Height [V]");
                    pTGraphProfile_RiseTime->GetXaxis()->SetLimits(0.0,0.7);
                    pTGraphProfile_RiseTime->GetXaxis()->SetRangeUser(0.0,0.7);
                    pTGraphProfile_RiseTime->GetYaxis()->SetTitle("Rise Time [ns]");
                    pTGraphProfile_RiseTime->GetYaxis()->SetLimits(0.0,1200);
                    pTGraphProfile_RiseTime->GetYaxis()->SetRangeUser(0.0,1200);
                    pTGraphProfile_RiseTime->GetYaxis()->SetTitleOffset(1.4);

                    if (pixelCounter != 0)
                    {
                        std::string legendLabel = "Pixel " + toString(pixel.at(pixelCounter));
                        pCanvas_TGraph_AllPixels_RiseTime->cd();
                        pTLegend_TGraph_AllPixels_RiseTime->AddEntry(pTGraphProfile_RiseTime,legendLabel.c_str(),"pl");
                        if (pixelCounter == 1)
                        {
                            pCanvas_TGraph_AllPixels_RiseTime->cd();
                            std::string pTGraphProfileTitleRiseTime = "Rise Time vs Pulse Height for SET" + set.at(setCounter);
                            pTGraphProfile_RiseTime->SetTitle(pTGraphProfileTitleRiseTime.c_str());
                            pTGraphProfile_RiseTime->Draw("AP");
                        }
                        else
                        {
                            pCanvas_TGraph_AllPixels_RiseTime->cd();
                            pTGraphProfile_RiseTime->Draw("P same");
                        }
                    }

                    else
                    {
                        std::string legendLabel = "SET " + set.at(setCounter);
                        pCanvas_TGraph_RiseTimeFull->cd();
                        pTGraphProfile_RiseTime->SetLineColor(rootColor(setCounter));
                        pTGraphProfile_RiseTime->SetMarkerColor(rootColor(setCounter));
                        pTGraphProfile_RiseTime->SetMarkerStyle(rootMarker(setCounter));
                        pTLegend_TGraph_RiseTimeFull->AddEntry(pTGraphProfile_RiseTime,legendLabel.c_str(),"pl");

                        if (setCounter == 0)
                        {
                            pCanvas_TGraph_RiseTimeFull->cd();
                            std::string pTGraphProfileTitleRiseTime = "Rise Time vs Pulse Height for All Samples";
                            pTGraphProfile_RiseTime->SetTitle(pTGraphProfileTitleRiseTime.c_str());
                            pTGraphProfile_RiseTime->Draw("AP");
                        }
                        else
                        {
                            pCanvas_TGraph_RiseTimeFull->cd();
                            pTGraphProfile_RiseTime->Draw("P same");
                        }
                    }
                }
                if (positionCounter == 0)
                {
                    pCanvas_TGraph_AllPixels_RiseTime->cd();
                    pTLegend_TGraph_AllPixels_RiseTime->Draw("same");
                    pCanvas_TGraph_AllPixels_RiseTime->SaveAs(pngFileName_TGraph_RiseTime.c_str());
                    pCanvas_TGraph_AllPixels_RiseTime->SaveAs(dotCFileName_TGraph_RiseTime.c_str());
                }
            }
        }

        pCanvas_TGraph_Full->cd();
        pTLegend_TGraph_Full->Draw("same");
        pCanvas_TGraph_Full->SaveAs(pngFileName_TGraph_Full.c_str());
        pCanvas_TGraph_Full->SaveAs(dotCFileName_TGraph_Full.c_str());

        if (positionCounter == 0)
        {
            pCanvas_TGraph_RiseTimeFull->cd();
            pTLegend_TGraph_RiseTimeFull->Draw("same");
            pCanvas_TGraph_RiseTimeFull->SaveAs(pngFileName_TGraph_RiseTimeFull.c_str());
            pCanvas_TGraph_RiseTimeFull->SaveAs(dotCFileName_TGraph_RiseTimeFull.c_str());
        }

        delete pCanvas_TGraph_RiseTimeFull, pCanvas_TGraph_Full, pTLegend_TGraph_Full, pTLegend_TGraph_RiseTimeFull;
    }
}

//============================================================

void makePlots(std::string setNumber, std::string resultsFileName, int pixelNumber, std::string pathForPlots, std::vector<float> *meanToT, std::vector<float> *pulseHeight, std::vector<float> *meanRiseTime, std::vector<float> *pulseHeightForRiseTime, int totCounter, std::string briefToTTitle, std::string totTitle)
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
    while (resultsFile >> setNumberFromFile >> pixelNumberFromFile >> eventNumber >> tot >> totPlusx >> totPlusxPlusy >> totPlusy >> totMinusxPlusy >> totMinusx >> basicPulseHeightFromFile >> fitPulseHeightFromFile >> troughStartTime >> troughTime >> tenPercentStart >> tenPercentEnd >> ninetyPercentStart >> ninetyPercentEnd >> frameCount)
    {
        if (totCounter == 0)
           desiredToT = tot;
        else if (totCounter == 1)
           desiredToT = totPlusx;
        else if (totCounter == 2)
           desiredToT = totPlusxPlusy;
        else if (totCounter == 3)
           desiredToT = totPlusy;
        else if (totCounter == 4)
           desiredToT = totMinusxPlusy;
        else if (totCounter == 5)
           desiredToT = totMinusx;

        //float riseTime = 1e9 * (troughTime - troughStartTime);
        float riseTime = 1e9 * (ninetyPercentStart - tenPercentStart);
        if (pixelNumber == 0)
        {
            if (frameCount == 1)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,desiredToT);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,desiredToT);

                if (totCounter == 0)
                {
                    pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                    pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
                }
            }
        }
        else
        {
            if (frameCount == 1 && pixelNumberFromFile == pixelNumber)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,desiredToT);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,desiredToT);

                if (totCounter == 0)
                {
                    pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                    pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
                }
            }
        }
    }
/*}

else
{
    while (resultsFile >> setNumberFromFile >> pixelNumberFromFile >> eventNumber >> tot >> basicPulseHeightFromFile >> fitPulseHeightFromFile >> troughStartTime >> troughTime >> frameCount)
    {
        float riseTime = 1e9 * (troughTime - troughStartTime);
        //float riseTime = 1e9 * (ninetyPercentStart - tenPercentStart);
        if (pixelNumber == 0)
        {
            if (frameCount == 1)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,tot);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,tot);
                pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
            }
        }
        else
        {
            if (frameCount == 1 && pixelNumberFromFile == pixelNumber)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,tot);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,tot);
                pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
            }
        }
    }
}*/
    resultsFile.close();

    //std::vector<float> meanToT, pulseHeight;
    TH1D *pTH2F_ProfileX_ToTvsPulseHeight = pTH2F_ToTvsPulseHeight->ProfileX();
    for (int k = 0; k < pTH2F_ProfileX_ToTvsPulseHeight->GetNbinsX(); k++)
    {
        if (pTH2F_ProfileX_ToTvsPulseHeight->GetBinContent(k) != 0)
        {
            meanToT->push_back(pTH2F_ProfileX_ToTvsPulseHeight->GetBinContent(k));
            pulseHeight->push_back(pTH2F_ProfileX_ToTvsPulseHeight->GetBinCenter(k));
        } 
    }

    if (totCounter == 0)
    {
        //std::vector<float> meanRiseTime, pulseHeightForRiseTime;
        TH1D *pTH2F_ProfileX_RiseTimevsPulseHeight = pTH2F_RiseTimevsPulseHeight->ProfileX();
        for (int k = 0; k < pTH2F_ProfileX_RiseTimevsPulseHeight->GetNbinsX(); k++)
        {
            if (pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinContent(k) != 0)
            {
                meanRiseTime->push_back(pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinContent(k));
                pulseHeightForRiseTime->push_back(pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinCenter(k));
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
        TGraph *pTGraphProfile = new TGraph (tVectorPulseHeight,tVectorToT);
        pTGraphProfile->SetLineWidth(4);
        pTGraphProfile->SetLineStyle(2);
        pTGraphProfile->SetLineColor(kOrange);
        pTGraphProfile->Draw("LP same");
        pCanvas_TH2F->SaveAs(pngFileName_TH2F.c_str());
        pCanvas_TH2F->SaveAs(dotCFileName_TH2F.c_str());
        delete pTGraphProfile;
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
            TGraph *pTGraphProfileRiseTime = new TGraph (tVectorPulseHeightForRiseTime,tVectorRiseTime);
            pTGraphProfileRiseTime->SetLineWidth(10);
            pTGraphProfileRiseTime->SetLineStyle(2);
            pTGraphProfileRiseTime->SetLineColor(kBlack);
            pTGraphProfileRiseTime->Draw("LP same");
            pCanvas_TH2F_RiseTime->SaveAs(pngFileName_TH2F_RiseTime.c_str());
            pCanvas_TH2F_RiseTime->SaveAs(dotCFileName_TH2F_RiseTime.c_str());
            delete pTGraphProfileRiseTime;
            delete pTH2F_RiseTimevsPulseHeight;
            delete pCanvas_TH2F_RiseTime;
        }
    }
}

//============================================================

void makePlots2(std::string setNumber, std::string resultsFileName, int pixelNumber, std::string pathForPlots, std::vector<float> *meanRiseTime, std::vector<float> *pulseHeightForRiseTime)
{
    //std::cout << "==== makePlots2 ==== " << std::endl;

    std::string graphTitleRiseTime, pngFileName_TGraph_RiseTime, dotCFileName_TGraph_RiseTime, pngFileName_TH2F_RiseTime, dotCFileName_TH2F_RiseTime;

    if (pixelNumber == 0)
    {
        graphTitleRiseTime = "Rise Time vs Pulse Height for SET" + setNumber;
        pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_TGraph_RiseTimevsPulseHeight.png";
        dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_TGraph_RiseTimevsPulseHeight.C";
        pngFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_TH2F_RiseTimevsPulseHeight.png";
        dotCFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_TH2F_RiseTimevsPulseHeight.C";
    }
    else 
    {
        graphTitleRiseTime = "Rise Time vs Pulse Height for SET" + setNumber + " Pixel " + toString(pixelNumber);
        pngFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_RiseTimevsPulseHeight.png";
        dotCFileName_TGraph_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TGraph_RiseTimevsPulseHeight.C";
        pngFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_RiseTimevsPulseHeight.png";
        dotCFileName_TH2F_RiseTime = pathForPlots + "/SET" + setNumber + "_Pixel" + toString(pixelNumber) + "_TH2F_RiseTimevsPulseHeight.C";
    }

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

    int pixelNumberFromFile, eventNumber;
    float tot, basicPulseHeightFromFile, fitPulseHeightFromFile, troughStartTime, troughTime, tenPercentStart, tenPercentEnd, ninetyPercentStart, ninetyPercentEnd, totPlusx, totPlusxPlusy, totPlusy, totMinusxPlusy, totMinusx;

    std::string setNumberFromFile;

    float desiredToT;

    ifstream resultsFile;
    resultsFile.open(resultsFileName.c_str());

    while (resultsFile >> setNumberFromFile >> pixelNumberFromFile >> eventNumber >> basicPulseHeightFromFile >> fitPulseHeightFromFile >> troughStartTime >> troughTime >> tenPercentStart >> tenPercentEnd >> ninetyPercentStart >> ninetyPercentEnd)
    {
        //std::cout << "setNumberFromFile      : " << setNumberFromFile << std::endl;
        //std::cout << "eventNumber            : " << eventNumber << std::endl;
        //std::cout << "fitPulseHeightFromFile : " << fitPulseHeightFromFile << std::endl;
        float riseTime = 1e9 * (ninetyPercentStart - tenPercentStart);
        if (pixelNumber == 0)
        {
            pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
            pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
        }
        else
        {
            pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
            pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
        }
    }
/*}

else
{
    while (resultsFile >> setNumberFromFile >> pixelNumberFromFile >> eventNumber >> tot >> basicPulseHeightFromFile >> fitPulseHeightFromFile >> troughStartTime >> troughTime >> frameCount)
    {
        float riseTime = 1e9 * (troughTime - troughStartTime);
        //float riseTime = 1e9 * (ninetyPercentStart - tenPercentStart);
        if (pixelNumber == 0)
        {
            if (frameCount == 1)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,tot);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,tot);
                pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
            }
        }
        else
        {
            if (frameCount == 1 && pixelNumberFromFile == pixelNumber)
            {
                pTGraph_ToTvsPulseHeight->SetPoint(pTGraph_ToTvsPulseHeight->GetN(),fitPulseHeightFromFile,tot);
                pTH2F_ToTvsPulseHeight->Fill(fitPulseHeightFromFile,tot);
                pTGraph_RiseTimevsPulseHeight->SetPoint(pTGraph_RiseTimevsPulseHeight->GetN(),fitPulseHeightFromFile,riseTime);
                pTH2F_RiseTimevsPulseHeight->Fill(fitPulseHeightFromFile,riseTime);
            }
        }
    }
}*/
    resultsFile.close();

    //std::vector<float> meanRiseTime, pulseHeightForRiseTime;
    TH1D *pTH2F_ProfileX_RiseTimevsPulseHeight = pTH2F_RiseTimevsPulseHeight->ProfileX();
    for (int k = 0; k < pTH2F_ProfileX_RiseTimevsPulseHeight->GetNbinsX(); k++)
    {
        if (pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinContent(k) != 0)
        {
            meanRiseTime->push_back(pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinContent(k));
            pulseHeightForRiseTime->push_back(pTH2F_ProfileX_RiseTimevsPulseHeight->GetBinCenter(k));
        }
    }

    if (!meanRiseTime->empty())
    {
        TVectorT<float> tVectorRiseTime(meanRiseTime->size(), &meanRiseTime->at(0));
        TVectorT<float> tVectorPulseHeightForRiseTime(pulseHeightForRiseTime->size(), &pulseHeightForRiseTime->at(0));

        TCanvas *pCanvas_TGraph_RiseTime = new TCanvas("PixelCalibrationTGraphCanvasRiseTime","Pixel Calibration TGraph Canvas Rise Time",200,10,3000,2500);
        pCanvas_TGraph_RiseTime->cd();
//       pCanvas_TGraph_RiseTime->SetLeftMargin(0.2);
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
        TGraph *pTGraphProfileRiseTime = new TGraph (tVectorPulseHeightForRiseTime,tVectorRiseTime);
        pTGraphProfileRiseTime->SetLineWidth(10);
        pTGraphProfileRiseTime->SetLineStyle(2);
        pTGraphProfileRiseTime->SetLineColor(kBlack);
        pTGraphProfileRiseTime->Draw("LP same");
        pCanvas_TH2F_RiseTime->SaveAs(pngFileName_TH2F_RiseTime.c_str());
        pCanvas_TH2F_RiseTime->SaveAs(dotCFileName_TH2F_RiseTime.c_str());
        delete pTGraphProfileRiseTime;
        delete pTH2F_RiseTimevsPulseHeight;
        delete pCanvas_TH2F_RiseTime;
    }
}

//============================================================

std::string toString(int a)
{
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    return str;
}

//============================================================

int rootColor(int a)
{
    static const int rootColorArray[] = {1,2,3,4,6,7,8,9};
    return rootColorArray[(a % (sizeof(rootColorArray) / sizeof(int)))]; 
}

//============================================================

int rootMarker(int a)
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
