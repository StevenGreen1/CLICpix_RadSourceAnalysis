#ifndef MASK_CONTINER_H
#define MASK_CONTINER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <math.h>
#include <sstream>
#include <string>
#include <vector>

#include "TAxis.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TProfile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TVector.h"

class AllPixelsPlotter
{
    private:
        /*
         * Private Variables
         */
        std::vector<std::string> m_FileNames;
        std::vector<int> m_SetNumber;
        std::vector<int> m_UniqueSetNumbers;
        std::vector<int> m_UniquePixelNumbers;
        std::vector<int> m_PixelNumber;
        std::vector<int> m_EventNumber;
        std::vector<int> m_ToTs;
        std::vector<int> m_ToTs_X;
        std::vector<int> m_ToTs_X_Y;
        std::vector<int> m_ToTs_Y;
        std::vector<int> m_ToTs_nX_Y;
        std::vector<int> m_ToTs_nX;
        std::vector<float> m_RiseTime;
        std::vector<float> m_SimplyPulseHeight;
        std::vector<float> m_FittedPulseHeight;
        std::vector<float> m_TroughStartTime;
        std::vector<float> m_TroughTime;
        std::vector<float> m_TenPercentPeakTroughStartTime;
        std::vector<float> m_TenPercentPeakTroughEndTime;
        std::vector<float> m_NinetyPercentPeakTroughStartTime;
        std::vector<float> m_NinetyPercentPeakTroughEndTime;
        std::vector<float> m_FrameCountOnDevice;
        std::string m_PlotPath;

    public:
        /*
         * Default Constructor
         */
        AllPixelsPlotter(std::vector<std::string> fileNames, std::string plotPath);

        /*
         * Default Destructor
         */
        ~AllPixelsPlotter();

        /*
         * Read data from all data files in m_FileNames
         */
        void ReadData();

        /*
         * Read an individual data file with name fileName
         */
        void ReadFileData(std::string fileName);

        void UniqueSetClassification();

        void UniquePixelClassification();

        void PlotToTPulseHeightAllSets(std::vector<int> activeToTs, std::string totDefinition);
        void PlotToTPulseHeightAllPixelsInSet(int activeSetNumber, std::vector<int> activeToTs, std::string totDefinition);
        void PlotRiseTimePulseHeightAllSets();
        void PlotRiseTimePulseHeightAllPixelsInSet(int activeSetNumber);

        int RootColor(int a);
        int RootMarker(int a);
        std::string SampleLabel(int setNumber);
        std::string IntToString(int a);

        /*
         * Get Parameter Functions
         */
};

#endif

