/*******************************************************************************
* Copyright (c) 2017-2020
* School of Electrical, Computer and Energy Engineering, Arizona State University
* PI: Prof. Yu Cao
* All rights reserved.
*
* This source code is part of NeuroSim - a device-circuit-algorithm framework to benchmark
* neuro-inspired architectures with synaptic devices(e.g., SRAM and emerging non-volatile memory).
* Copyright of the model is maintained by the developers, and the model is distributed under
* the terms of the Creative Commons Attribution-NonCommercial 4.0 International Public License
* http://creativecommons.org/licenses/by-nc/4.0/legalcode.
* The source code is free and you can redistribute and/or modify it
* by providing that the following conditions are met:
*
*  1) Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*  2) Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer in the documentation
*     and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Developer list:
*		Pragnya Nalla Email: nalla052@umn.edu

* Credits: Prof.Shimeng Yu and his research group for NeuroSim
********************************************************************************/

#include <cmath>
#include <iostream>
#include <vector>
#include "constant.h"
#include "formula.h"
#include "DAC.h"
#include "Param.h"

using namespace std;
extern Param *param;

DAC::DAC(const InputParameter& _inputParameter, const Technology& _tech, const MemCell& _cell): inputParameter(_inputParameter), tech(_tech), cell(_cell), FunctionUnit() {
	initialized = false;
}

void DAC::Initialize(int _numRow, double _clkFreq){
    if (initialized)
		cout << "[DAC] Warning: Already initialized!" << endl;
    numRow=_numRow;
    clkFreq = _clkFreq;
    initialized = true;
}

void DAC::CalculateArea(double _newHeight, double _newWidth, AreaModify _option) {
	if (!initialized) {
		cout << "[DFFDAC] Error: Require initialization first!" << endl;
	} else {
        height=70e-6*numRow*3/7;
        width=22e-6;
    }
    float factor = pow((float)param->technode/32,2);
    //cout<<"factor_ar"<<factor<<endl;
    area = height * width*factor;
    // Modify layout
    newHeight = _newHeight;
    newWidth = _newWidth;
    switch (_option) {
        case MAGIC:
            MagicLayout();
            break;
        case OVERRIDE:
            OverrideLayout();
            break;
        default:    // NONE
            break;
    }
}

void DAC::CalculateLatency(double _rampInput, double _capLoad, double _resLoad, double numRead) {	
	if (!initialized) {
		cout << "[DAC] Error: Require initialization first!" << endl;
	} else {
		capLoad = _capLoad;
		resLoad = _resLoad;
		readLatency = 0;
        float factor = (float)param->technode/32;
        //cout<<"factor"<<factor<<endl;
		readLatency = resLoad * capLoad*(2.65+2.48+1.86+1.16+0.47)*factor;
		readLatency *= numRead;
	}
}

void DAC::CalculatePower(double numRead) {
	if (!initialized) {
		cout << "[DAC] Error: Require initialization first!" << endl;
	} else {
        float factor = pow((float)param->readVoltage/0.9,2);
        //cout<<"factor"<<factor<<endl;
        readDynamicEnergy=numRow*2.10e-4*1*factor/clkFreq;
        readDynamicEnergy *= numRead;
    }    
}