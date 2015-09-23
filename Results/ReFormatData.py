#!/usr/bin/python

import sys
import re

textFile = open("PixelCalibration_SETNoCLICpix_Detailed_v2_ReFormatted.dat", "w")

with open('PixelCalibration_SETNoCLICpix_Detailed_v2.dat') as f:
    for line in f:
        regex = re.compile("NoCLICpix\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\t(.*?)\n")
        q = regex.search(line)
        if q is not None:
            newline = 'NoCLICpix\t' + q.group(1) + '\t' + q.group(2) + '\t0\t0\t0\t0\t0\t0\t' + q.group(3) + '\t' + q.group(4) + '\t' + q.group(5) + '\t' + q.group(6) + '\t' + q.group(7) + '\t' + q.group(8) + '\t' + q.group(9) + '\t0\n'
            textFile.write(newline)

textFile.close()
