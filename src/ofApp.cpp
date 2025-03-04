#include "ofApp.h"
// globals

const int pastFramesSize = 120;
ofFbo pastFrames1[pastFramesSize];
ofFbo pastFrames2[pastFramesSize];
int pastFramesOffset = 0;
unsigned int pastFramesCount = 0;

ofTexture dummyTex;
// testing variables
int testSwitch1 = 1;
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(30);
	ofBackground(0);
	ofHideCursor();

	outputWidth = 1280;
	outputHeight = 720;

	inputSetup();

	framebufferSetup();

	// keep this last in setup for easier debugging
	shader1.load("shadersGL4/shader1");
	shader2.load("shadersGL4/shader2");
	shader3.load("shadersGL4/shader3");

	dummyTex.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

	sevenStar1Setup();
}

//--------------------------------------------------------------
void ofApp::update()
{

	inputUpdate();
	lfoUpdate();
}
//------------------------------------------------------------
float ofApp::lfo(float amp, float rate, int shape)
{

	return amp * sin(rate);
}
//-------------------------------------------------------------
void ofApp::lfoUpdate()
{

	// ch1 adjust
	ch1XDisplaceTheta += lfoRateC * (gui->ch1AdjustLfo[1]);
	ch1YDisplaceTheta += lfoRateC * (gui->ch1AdjustLfo[3]);
	ch1ZDisplaceTheta += lfoRateC * (gui->ch1AdjustLfo[5]);
	ch1RotateTheta += lfoRateC * (gui->ch1AdjustLfo[7]);
	ch1HueAttenuateTheta += lfoRateC * (gui->ch1AdjustLfo[9]);
	ch1SaturationAttenuateTheta += lfoRateC * (gui->ch1AdjustLfo[11]);
	ch1BrightAttenuateTheta += lfoRateC * (gui->ch1AdjustLfo[13]);
	ch1KaleidoscopeSliceTheta += lfoRateC * (gui->ch1AdjustLfo[15]);

	// ch2 mix and key
	ch2MixAmountTheta += lfoRateC * (gui->ch2MixAndKeyLfo[1]);
	ch2KeyThresholdTheta += lfoRateC * (gui->ch2MixAndKeyLfo[3]);
	ch2KeySoftTheta += lfoRateC * (gui->ch2MixAndKeyLfo[5]);

	// ch2 adjust
	ch2XDisplaceTheta += lfoRateC * (gui->ch2AdjustLfo[1]);
	ch2YDisplaceTheta += lfoRateC * (gui->ch2AdjustLfo[3]);
	ch2ZDisplaceTheta += lfoRateC * (gui->ch2AdjustLfo[5]);
	ch2RotateTheta += lfoRateC * (gui->ch2AdjustLfo[7]);
	ch2HueAttenuateTheta += lfoRateC * (gui->ch2AdjustLfo[9]);
	ch2SaturationAttenuateTheta += lfoRateC * (gui->ch2AdjustLfo[11]);
	ch2BrightAttenuateTheta += lfoRateC * (gui->ch2AdjustLfo[13]);
	ch2KaleidoscopeSliceTheta += lfoRateC * (gui->ch2AdjustLfo[15]);

	// fb1 mix and key
	fb1MixAmountTheta += lfoRateC * (gui->fb1MixAndKeyLfo[1]);
	fb1KeyThresholdTheta += lfoRateC * (gui->fb1MixAndKeyLfo[3]);
	fb1KeySoftTheta += lfoRateC * (gui->fb1MixAndKeyLfo[5]);

	// fb1 geo1
	fb1XDisplaceTheta += lfoRateC * (gui->fb1Geo1Lfo1[1]);
	fb1YDisplaceTheta += lfoRateC * (gui->fb1Geo1Lfo1[3]);
	fb1ZDisplaceTheta += lfoRateC * (gui->fb1Geo1Lfo1[5]);
	fb1RotateTheta += lfoRateC * (gui->fb1Geo1Lfo1[7]);

	// fb1 geo2
	fb1ShearMatrix1Theta += lfoRateC * (gui->fb1Geo1Lfo2[1]);
	fb1ShearMatrix2Theta += lfoRateC * (gui->fb1Geo1Lfo2[5]);
	fb1ShearMatrix3Theta += lfoRateC * (gui->fb1Geo1Lfo2[7]);
	fb1ShearMatrix4Theta += lfoRateC * (gui->fb1Geo1Lfo2[3]);
	fb1KaleidoscopeSliceTheta += lfoRateC * (gui->fb1Geo1Lfo2[9]);

	fb1HueAttenuateTheta += lfoRateC * (gui->fb1Color1Lfo1[1]);
	fb1SaturationAttenuateTheta += lfoRateC * (gui->fb1Color1Lfo1[3]);
	fb1BrightAttenuateTheta += lfoRateC * (gui->fb1Color1Lfo1[5]);

	// block2Input adjust
	block2InputXDisplaceTheta += lfoRateC * (gui->block2InputAdjustLfo[1]);
	block2InputYDisplaceTheta += lfoRateC * (gui->block2InputAdjustLfo[3]);
	block2InputZDisplaceTheta += lfoRateC * (gui->block2InputAdjustLfo[5]);
	block2InputRotateTheta += lfoRateC * (gui->block2InputAdjustLfo[7]);
	block2InputHueAttenuateTheta += lfoRateC * (gui->block2InputAdjustLfo[9]);
	block2InputSaturationAttenuateTheta += lfoRateC * (gui->block2InputAdjustLfo[11]);
	block2InputBrightAttenuateTheta += lfoRateC * (gui->block2InputAdjustLfo[13]);
	block2InputKaleidoscopeSliceTheta += lfoRateC * (gui->block2InputAdjustLfo[15]);

	// fb2 mix and key
	fb2MixAmountTheta += lfoRateC * (gui->fb2MixAndKeyLfo[1]);
	fb2KeyThresholdTheta += lfoRateC * (gui->fb2MixAndKeyLfo[3]);
	fb2KeySoftTheta += lfoRateC * (gui->fb2MixAndKeyLfo[5]);

	// fb2 geo1
	fb2XDisplaceTheta += lfoRateC * (gui->fb2Geo1Lfo1[1]);
	fb2YDisplaceTheta += lfoRateC * (gui->fb2Geo1Lfo1[3]);
	fb2ZDisplaceTheta += lfoRateC * (gui->fb2Geo1Lfo1[5]);
	fb2RotateTheta += lfoRateC * (gui->fb2Geo1Lfo1[7]);
	// fb2 geo2
	fb2ShearMatrix1Theta += lfoRateC * (gui->fb2Geo1Lfo2[1]);
	fb2ShearMatrix2Theta += lfoRateC * (gui->fb2Geo1Lfo2[5]);
	fb2ShearMatrix3Theta += lfoRateC * (gui->fb2Geo1Lfo2[7]);
	fb2ShearMatrix4Theta += lfoRateC * (gui->fb2Geo1Lfo2[3]);
	fb2KaleidoscopeSliceTheta += lfoRateC * (gui->fb2Geo1Lfo2[9]);

	// fb2 color
	fb2HueAttenuateTheta += lfoRateC * (gui->fb2Color1Lfo1[1]);
	fb2SaturationAttenuateTheta += lfoRateC * (gui->fb2Color1Lfo1[3]);
	fb2BrightAttenuateTheta += lfoRateC * (gui->fb2Color1Lfo1[5]);

	// BLOCK 3

	// block1 geo
	block1XDisplaceTheta += lfoRateC * (gui->block1Geo1Lfo1[1]);
	block1YDisplaceTheta += lfoRateC * (gui->block1Geo1Lfo1[3]);
	block1ZDisplaceTheta += lfoRateC * (gui->block1Geo1Lfo1[5]);
	block1RotateTheta += lfoRateC * (gui->block1Geo1Lfo1[7]);

	block1ShearMatrix1Theta += lfoRateC * (gui->block1Geo1Lfo2[1]);
	block1ShearMatrix2Theta += lfoRateC * (gui->block1Geo1Lfo2[5]);
	block1ShearMatrix3Theta += lfoRateC * (gui->block1Geo1Lfo2[7]);
	block1ShearMatrix4Theta += lfoRateC * (gui->block1Geo1Lfo2[3]);
	block1KaleidoscopeSliceTheta += lfoRateC * (gui->block1Geo1Lfo2[9]);

	// block1 colorize
	block1ColorizeHueBand1Theta += lfoRateC * (gui->block1ColorizeLfo1[3]);
	block1ColorizeSaturationBand1Theta += lfoRateC * (gui->block1ColorizeLfo1[4]);
	block1ColorizeBrightBand1Theta += lfoRateC * (gui->block1ColorizeLfo1[5]);
	block1ColorizeHueBand2Theta += lfoRateC * (gui->block1ColorizeLfo1[9]);
	block1ColorizeSaturationBand2Theta += lfoRateC * (gui->block1ColorizeLfo1[10]);
	block1ColorizeBrightBand2Theta += lfoRateC * (gui->block1ColorizeLfo1[11]);

	block1ColorizeHueBand3Theta += lfoRateC * (gui->block1ColorizeLfo2[3]);
	;
	block1ColorizeSaturationBand3Theta += lfoRateC * (gui->block1ColorizeLfo2[4]);
	block1ColorizeBrightBand3Theta += lfoRateC * (gui->block1ColorizeLfo2[5]);
	block1ColorizeHueBand4Theta += lfoRateC * (gui->block1ColorizeLfo2[9]);
	block1ColorizeSaturationBand4Theta += lfoRateC * (gui->block1ColorizeLfo2[10]);
	block1ColorizeBrightBand4Theta += lfoRateC * (gui->block1ColorizeLfo2[11]);

	block1ColorizeHueBand5Theta += lfoRateC * (gui->block1ColorizeLfo3[3]);
	block1ColorizeSaturationBand5Theta += lfoRateC * (gui->block1ColorizeLfo3[4]);
	block1ColorizeBrightBand5Theta += lfoRateC * (gui->block1ColorizeLfo3[5]);

	// block2 geo
	block2XDisplaceTheta += lfoRateC * (gui->block2Geo1Lfo1[1]);
	block2YDisplaceTheta += lfoRateC * (gui->block2Geo1Lfo1[3]);
	block2ZDisplaceTheta += lfoRateC * (gui->block2Geo1Lfo1[5]);
	block2RotateTheta += lfoRateC * (gui->block2Geo1Lfo1[7]);

	block2ShearMatrix1Theta += lfoRateC * (gui->block2Geo1Lfo2[1]);
	block2ShearMatrix2Theta += lfoRateC * (gui->block2Geo1Lfo2[5]);
	block2ShearMatrix3Theta += lfoRateC * (gui->block2Geo1Lfo2[7]);
	block2ShearMatrix4Theta += lfoRateC * (gui->block2Geo1Lfo2[3]);
	block2KaleidoscopeSliceTheta += lfoRateC * (gui->block2Geo1Lfo2[9]);

	// block2 colorize
	block2ColorizeHueBand1Theta += lfoRateC * (gui->block2ColorizeLfo1[3]);
	block2ColorizeSaturationBand1Theta += lfoRateC * (gui->block2ColorizeLfo1[4]);
	block2ColorizeBrightBand1Theta += lfoRateC * (gui->block2ColorizeLfo1[5]);
	block2ColorizeHueBand2Theta += lfoRateC * (gui->block2ColorizeLfo1[9]);
	block2ColorizeSaturationBand2Theta += lfoRateC * (gui->block2ColorizeLfo1[10]);
	block2ColorizeBrightBand2Theta += lfoRateC * (gui->block2ColorizeLfo1[11]);

	block2ColorizeHueBand3Theta += lfoRateC * (gui->block2ColorizeLfo2[3]);
	;
	block2ColorizeSaturationBand3Theta += lfoRateC * (gui->block2ColorizeLfo2[4]);
	block2ColorizeBrightBand3Theta += lfoRateC * (gui->block2ColorizeLfo2[5]);
	block2ColorizeHueBand4Theta += lfoRateC * (gui->block2ColorizeLfo2[9]);
	block2ColorizeSaturationBand4Theta += lfoRateC * (gui->block2ColorizeLfo2[10]);
	block2ColorizeBrightBand4Theta += lfoRateC * (gui->block2ColorizeLfo2[11]);

	block2ColorizeHueBand5Theta += lfoRateC * (gui->block2ColorizeLfo3[3]);
	block2ColorizeSaturationBand5Theta += lfoRateC * (gui->block2ColorizeLfo3[4]);
	block2ColorizeBrightBand5Theta += lfoRateC * (gui->block2ColorizeLfo3[5]);

	// matrix mixer
	matrixMixBgRedIntoFgRedTheta += lfoRateC * (gui->matrixMixLfo1[3]);
	matrixMixBgGreenIntoFgRedTheta += lfoRateC * (gui->matrixMixLfo1[4]);
	matrixMixBgBlueIntoFgRedTheta += lfoRateC * (gui->matrixMixLfo1[5]);

	matrixMixBgRedIntoFgGreenTheta += lfoRateC * (gui->matrixMixLfo1[9]);
	matrixMixBgGreenIntoFgGreenTheta += lfoRateC * (gui->matrixMixLfo1[10]);
	matrixMixBgBlueIntoFgGreenTheta += lfoRateC * (gui->matrixMixLfo1[11]);

	matrixMixBgRedIntoFgBlueTheta += lfoRateC * (gui->matrixMixLfo2[3]);
	matrixMixBgGreenIntoFgBlueTheta += lfoRateC * (gui->matrixMixLfo2[4]);
	matrixMixBgBlueIntoFgBlueTheta += lfoRateC * (gui->matrixMixLfo2[5]);

	// final mix and key
	finalMixAmountTheta += lfoRateC * (gui->finalMixAndKeyLfo[1]);
	finalKeyThresholdTheta += lfoRateC * (gui->finalMixAndKeyLfo[3]);
	finalKeySoftTheta += lfoRateC * (gui->finalMixAndKeyLfo[5]);
}

//--------------------------------------------------------------
void ofApp::draw()
{

	// coefficients for parameters
	// namespacing shit up here
	// since everything is coming in as just arrays
	// we should properly name things here i think
	// coefficients will just be the name plus C at the end
	// eventually we will want to use gui stuff to rescale Coefficients

	// ch1 coefficients

	// ch1 adjust parameters
	float ch1XDisplace = ch1XDisplaceC * (gui->ch1Adjust[0]);
	float ch1YDisplace = ch1YDisplaceC * (gui->ch1Adjust[1]);
	float ch1ZDisplace = (1.0f + gui->ch1Adjust[2]);
	float ch1Rotate = ch1RotateC * (gui->ch1Adjust[3]);
	float ch1HueAttenuate = 1.0f + (gui->ch1Adjust[4]);
	float ch1SaturationAttenuate = 1.0f + (gui->ch1Adjust[5]);
	float ch1BrightAttenuate = 1.0f + (gui->ch1Adjust[6]);
	float ch1Posterize = ch1PosterizeC * (1.0f - gui->ch1Adjust[7]) + 1.0;
	float ch1PosterizeInvert = 1.0f / ch1Posterize;
	bool ch1PosterizeSwitch = 0;
	float ch1KaleidoscopeAmount = floor(ch1KaleidoscopeAmountC * (gui->ch1Adjust[8]));
	float ch1KaleidoscopeSlice = ch1KaleidoscopeSliceC * (gui->ch1Adjust[9]);
	float ch1BlurAmount = gui->ch1Adjust[10];
	float ch1BlurRadius = ch1FilterRadiusC * (gui->ch1Adjust[11]) + 1.0;
	float ch1SharpenAmount = ch1SharpenAmountC * (gui->ch1Adjust[12]);
	float ch1SharpenRadius = ch1FilterRadiusC * (gui->ch1Adjust[13]) + 1.0;
	float ch1FiltersBoost = (gui->ch1Adjust[14]);
	// ch2 mix and key
	float ch2MixAmount = mixAmountC * (gui->ch2MixAndKey[0]);
	float ch2KeyValueRed = keyC * (gui->ch2MixAndKey[1]);
	float ch2KeyValueGreen = keyC * (gui->ch2MixAndKey[2]);
	float ch2KeyValueBlue = keyC * (gui->ch2MixAndKey[3]);
	float ch2KeyThreshold = keyThresholdC * (gui->ch2MixAndKey[4]);
	float ch2KeySoft = (gui->ch2MixAndKey[5]);

	// ch2 adjust parameters
	float ch2XDisplace = ch2XDisplaceC * (gui->ch2Adjust[0]);
	float ch2YDisplace = ch2YDisplaceC * (gui->ch2Adjust[1]);
	float ch2ZDisplace = ch2ZDisplaceC * (1.0 + gui->ch2Adjust[2]);
	float ch2Rotate = ch2RotateC * (gui->ch2Adjust[3]);
	float ch2HueAttenuate = 1.0f + (gui->ch2Adjust[4]);
	float ch2SaturationAttenuate = 1.0f + (gui->ch2Adjust[5]);
	float ch2BrightAttenuate = 1.0f + (gui->ch2Adjust[6]);
	float ch2Posterize = ch2PosterizeC * (1.0f - gui->ch2Adjust[7]) + 1.0;
	float ch2PosterizeInvert = 1.0f / ch2Posterize;
	bool ch2PosterizeSwitch = 0;
	float ch2KaleidoscopeAmount = floor(ch2KaleidoscopeAmountC * (gui->ch2Adjust[8]));
	float ch2KaleidoscopeSlice = ch2KaleidoscopeSliceC * (gui->ch2Adjust[9]);
	float ch2BlurAmount = gui->ch2Adjust[10];
	float ch2BlurRadius = ch2FilterRadiusC * (gui->ch2Adjust[11]) + 1.0;
	float ch2SharpenAmount = ch2SharpenAmountC * (gui->ch2Adjust[12]);
	float ch2SharpenRadius = ch2FilterRadiusC * (gui->ch2Adjust[13]) + 1.0;
	float ch2FiltersBoost = (gui->ch2Adjust[14]);

	// fb1 mix and key
	float fb1MixAmount = mixAmountC * (gui->fb1MixAndKey[0]);
	float fb1KeyValueRed = keyC * (gui->fb1MixAndKey[1]);
	float fb1KeyValueGreen = keyC * (gui->fb1MixAndKey[2]);
	float fb1KeyValueBlue = keyC * (gui->fb1MixAndKey[3]);
	float fb1KeyThreshold = keyThresholdC * (gui->fb1MixAndKey[4]);
	float fb1KeySoft = (gui->fb1MixAndKey[5]);

	// fb1 geo1
	float fb1XDisplace = fb1XDisplaceC * (gui->fb1Geo1[0]);
	float fb1YDisplace = fb1YDisplaceC * (gui->fb1Geo1[1]);
	float fb1ZDisplace = 1.0f + fb1ZDisplaceC * (gui->fb1Geo1[2]);
	float fb1Rotate = fb1RotateC * (gui->fb1Geo1[3]);
	float fb1ShearMatrix1 = fb1ShearMatrix1C * (1.0f / fb1ShearMatrix1C + gui->fb1Geo1[4]);
	float fb1ShearMatrix2 = fb1ShearMatrix2C * (gui->fb1Geo1[6]);
	float fb1ShearMatrix3 = fb1ShearMatrix3C * (gui->fb1Geo1[7]);
	float fb1ShearMatrix4 = fb1ShearMatrix4C * (1.0f / fb1ShearMatrix1C + gui->fb1Geo1[5]);
	float fb1KaleidoscopeAmount = floor(fb1KaleidoscopeAmountC * (gui->fb1Geo1[8]));
	float fb1KaleidoscopeSlice = fb1KaleidoscopeSliceC * (gui->fb1Geo1[9]);

	// fb1 color1
	float fb1HueOffset = fb1HueOffsetC * (gui->fb1Color1[0]);
	float fb1SaturationOffset = fb1SaturationOffsetC * (gui->fb1Color1[1]);
	float fb1BrightOffset = fb1BrightOffsetC * (gui->fb1Color1[2]);
	float fb1HueAttenuate = 1.0f + fb1HueAttenuateC * (gui->fb1Color1[3]);
	float fb1SaturationAttenuate = 1.0f + fb1SaturationAttenuateC * (gui->fb1Color1[4]);
	float fb1BrightAttenuate = 1.0f + fb1BrightAttenuateC * (gui->fb1Color1[5]);
	float fb1HuePowmap = 1.0f + fb1HuePowmapC * (gui->fb1Color1[6]);
	float fb1SaturationPowmap = 1.0f + fb1SaturationPowmapC * (gui->fb1Color1[7]);
	float fb1BrightPowmap = 1.0f + fb1BrightPowmapC * (gui->fb1Color1[8]);
	float fb1HueShaper = fb1HueShaperC * (gui->fb1Color1[9]);
	float fb1Posterize = fb1PosterizeC * (1.0f - gui->fb1Color1[10]) + 1.0;
	float fb1PosterizeInvert = 1.0f / fb1Posterize;
	bool fb1PosterizeSwitch = 0;

	// fb1 filters
	float fb1BlurAmount = gui->fb1Filters[0];
	float fb1BlurRadius = fb1FilterRadiusC * (gui->fb1Filters[1]) + 1.0;
	float fb1SharpenAmount = fb1SharpenAmountC * (gui->fb1Filters[2]);
	float fb1SharpenRadius = fb1FilterRadiusC * (gui->fb1Filters[3]) + 1.0;
	float fb1TemporalFilter1Amount = fb1TemporalFilterAmountC * (gui->fb1Filters[4]);
	float fb1TemporalFilter1Resonance = (gui->fb1Filters[5]);
	float fb1TemporalFilter2Amount = fb1TemporalFilterAmountC * (gui->fb1Filters[6]);
	float fb1TemporalFilter2Resonance = (gui->fb1Filters[7]);
	float fb1FiltersBoost = (gui->fb1Filters[8]);

	// BLOCK 2

	// block2Input adjust parameters
	float block2InputXDisplace = block2InputXDisplaceC * (gui->block2InputAdjust[0]);
	float block2InputYDisplace = block2InputYDisplaceC * (gui->block2InputAdjust[1]);
	float block2InputZDisplace = (1.0f + gui->block2InputAdjust[2]);
	float block2InputRotate = block2InputRotateC * (gui->block2InputAdjust[3]);
	float block2InputHueAttenuate = 1.0f + (gui->block2InputAdjust[4]);
	float block2InputSaturationAttenuate = 1.0f + (gui->block2InputAdjust[5]);
	float block2InputBrightAttenuate = 1.0f + (gui->block2InputAdjust[6]);
	float block2InputPosterize = block2InputPosterizeC * (1.0f - gui->block2InputAdjust[7]) + 1.0;
	float block2InputPosterizeInvert = 1.0f / block2InputPosterize;
	bool block2InputPosterizeSwitch = 0;
	float block2InputKaleidoscopeAmount = floor(block2InputKaleidoscopeAmountC * (gui->block2InputAdjust[8]));
	float block2InputKaleidoscopeSlice = block2InputKaleidoscopeSliceC * (gui->block2InputAdjust[9]);
	float block2InputBlurAmount = gui->block2InputAdjust[10];
	float block2InputBlurRadius = block2InputFilterRadiusC * (gui->block2InputAdjust[11]) + 1.0;
	float block2InputSharpenAmount = block2InputSharpenAmountC * (gui->block2InputAdjust[12]);
	float block2InputSharpenRadius = block2InputFilterRadiusC * (gui->block2InputAdjust[13]) + 1.0;
	float block2InputFiltersBoost = (gui->block2InputAdjust[14]);

	// fb2 mix and key
	float fb2MixAmount = mixAmountC * (gui->fb2MixAndKey[0]);
	float fb2KeyValueRed = keyC * (gui->fb2MixAndKey[1]);
	float fb2KeyValueGreen = keyC * (gui->fb2MixAndKey[2]);
	float fb2KeyValueBlue = keyC * (gui->fb2MixAndKey[3]);
	float fb2KeyThreshold = keyThresholdC * (gui->fb2MixAndKey[4]);
	float fb2KeySoft = (gui->fb2MixAndKey[5]);

	// fb2 geo1
	float fb2XDisplace = fb2XDisplaceC * (gui->fb2Geo1[0]);
	float fb2YDisplace = fb2YDisplaceC * (gui->fb2Geo1[1]);
	float fb2ZDisplace = 1.0f + fb2ZDisplaceC * (gui->fb2Geo1[2]);
	float fb2Rotate = fb2RotateC * (gui->fb2Geo1[3]);
	float fb2ShearMatrix1 = fb2ShearMatrix1C * (1.0f / fb2ShearMatrix1C + gui->fb2Geo1[4]);
	float fb2ShearMatrix2 = fb2ShearMatrix2C * (gui->fb2Geo1[6]);
	float fb2ShearMatrix3 = fb2ShearMatrix3C * (gui->fb2Geo1[7]);
	float fb2ShearMatrix4 = fb2ShearMatrix4C * (1.0f / fb2ShearMatrix1C + gui->fb2Geo1[5]);
	float fb2KaleidoscopeAmount = floor(fb2KaleidoscopeAmountC * (gui->fb2Geo1[8]));
	float fb2KaleidoscopeSlice = fb2KaleidoscopeSliceC * (gui->fb2Geo1[9]);

	// fb2 color1
	float fb2HueOffset = fb2HueOffsetC * (gui->fb2Color1[0]);
	float fb2SaturationOffset = fb2SaturationOffsetC * (gui->fb2Color1[1]);
	float fb2BrightOffset = fb2BrightOffsetC * (gui->fb2Color1[2]);
	float fb2HueAttenuate = 1.0f + fb2HueAttenuateC * (gui->fb2Color1[3]);
	float fb2SaturationAttenuate = 1.0f + fb2SaturationAttenuateC * (gui->fb2Color1[4]);
	float fb2BrightAttenuate = 1.0f + fb2BrightAttenuateC * (gui->fb2Color1[5]);
	float fb2HuePowmap = 1.0f + fb2HuePowmapC * (gui->fb2Color1[6]);
	float fb2SaturationPowmap = 1.0f + fb2SaturationPowmapC * (gui->fb2Color1[7]);
	float fb2BrightPowmap = 1.0f + fb2BrightPowmapC * (gui->fb2Color1[8]);
	float fb2HueShaper = fb2HueShaperC * (gui->fb2Color1[9]);
	float fb2Posterize = fb2PosterizeC * (1.0f - gui->fb2Color1[10]) + 1.0;
	float fb2PosterizeInvert = 1.0f / fb2Posterize;
	bool fb2PosterizeSwitch = 0;

	// fb2 filters
	float fb2BlurAmount = gui->fb2Filters[0];
	float fb2BlurRadius = fb2FilterRadiusC * (gui->fb2Filters[1]) + 1.0;
	float fb2SharpenAmount = fb2SharpenAmountC * (gui->fb2Filters[2]);
	float fb2SharpenRadius = fb2FilterRadiusC * (gui->fb2Filters[3]) + 1.0;
	float fb2TemporalFilter1Amount = fb2TemporalFilterAmountC * (gui->fb2Filters[4]);
	float fb2TemporalFilter1Resonance = (gui->fb2Filters[5]);
	float fb2TemporalFilter2Amount = fb2TemporalFilterAmountC * (gui->fb2Filters[6]);
	float fb2TemporalFilter2Resonance = (gui->fb2Filters[7]);
	float fb2FiltersBoost = (gui->fb2Filters[8]);

	// BLOCK3

	// block1 Geo
	float block1XDisplace = block1XDisplaceC * (gui->block1Geo[0]);
	float block1YDisplace = block1YDisplaceC * (gui->block1Geo[1]);
	float block1ZDisplace = 1.0f + block1ZDisplaceC * (gui->block1Geo[2]);
	float block1Rotate = block1RotateC * (gui->block1Geo[3]);
	float block1ShearMatrix1 = block1ShearMatrix1C * (1.0f / block1ShearMatrix1C + gui->block1Geo[4]);
	float block1ShearMatrix2 = block1ShearMatrix2C * (gui->block1Geo[6]);
	float block1ShearMatrix3 = block1ShearMatrix3C * (gui->block1Geo[7]);
	float block1ShearMatrix4 = block1ShearMatrix4C * (1.0f / block1ShearMatrix1C + gui->block1Geo[5]);
	float block1KaleidoscopeAmount = floor(block1KaleidoscopeAmountC * (gui->block1Geo[8]));
	float block1KaleidoscopeSlice = block1KaleidoscopeSliceC * (gui->block1Geo[9]);

	// block1 colorize
	// yes these are poorly named b/c HSB and RGB switching.  deal with it
	float block1ColorizeHueBand1 = (gui->block1Colorize[0]);
	float block1ColorizeSaturationBand1 = (gui->block1Colorize[1]);
	float block1ColorizeBrightBand1 = (gui->block1Colorize[2]);
	float block1ColorizeHueBand2 = (gui->block1Colorize[3]);
	float block1ColorizeSaturationBand2 = (gui->block1Colorize[4]);
	float block1ColorizeBrightBand2 = (gui->block1Colorize[5]);
	float block1ColorizeHueBand3 = (gui->block1Colorize[6]);
	float block1ColorizeSaturationBand3 = (gui->block1Colorize[7]);
	float block1ColorizeBrightBand3 = (gui->block1Colorize[8]);
	float block1ColorizeHueBand4 = (gui->block1Colorize[9]);
	float block1ColorizeSaturationBand4 = (gui->block1Colorize[10]);
	float block1ColorizeBrightBand4 = (gui->block1Colorize[11]);
	float block1ColorizeHueBand5 = (gui->block1Colorize[12]);
	float block1ColorizeSaturationBand5 = (gui->block1Colorize[13]);
	float block1ColorizeBrightBand5 = (gui->block1Colorize[14]);

	// block1 filters
	float block1BlurAmount = gui->block1Filters[0];
	float block1BlurRadius = block1FilterRadiusC * (gui->block1Filters[1]) + 1.0;
	float block1SharpenAmount = block1SharpenAmountC * (gui->block1Filters[2]);
	float block1SharpenRadius = block1FilterRadiusC * (gui->block1Filters[3]) + 1.0;
	float block1FiltersBoost = (gui->block1Filters[4]);
	float block1Dither = block1DitherC * (1.0 - gui->block1Filters[5]) + 1.0;

	// block2 Geo
	float block2XDisplace = block2XDisplaceC * (gui->block2Geo[0]);
	float block2YDisplace = block2YDisplaceC * (gui->block2Geo[1]);
	float block2ZDisplace = 1.0f + block2ZDisplaceC * (gui->block2Geo[2]);
	float block2Rotate = block2RotateC * (gui->block2Geo[3]);
	float block2ShearMatrix1 = block2ShearMatrix1C * (1.0f / block2ShearMatrix1C + gui->block2Geo[4]);
	float block2ShearMatrix2 = block2ShearMatrix2C * (gui->block2Geo[6]);
	float block2ShearMatrix3 = block2ShearMatrix3C * (gui->block2Geo[7]);
	float block2ShearMatrix4 = block2ShearMatrix4C * (1.0f / block2ShearMatrix1C + gui->block2Geo[5]);
	float block2KaleidoscopeAmount = floor(block2KaleidoscopeAmountC * (gui->block2Geo[8]));
	float block2KaleidoscopeSlice = block2KaleidoscopeSliceC * (gui->block2Geo[9]);

	// block2 colorize
	float block2ColorizeHueBand1 = (gui->block2Colorize[0]);
	float block2ColorizeSaturationBand1 = (gui->block2Colorize[1]);
	float block2ColorizeBrightBand1 = (gui->block2Colorize[2]);
	float block2ColorizeHueBand2 = (gui->block2Colorize[3]);
	float block2ColorizeSaturationBand2 = (gui->block2Colorize[4]);
	float block2ColorizeBrightBand2 = (gui->block2Colorize[5]);
	float block2ColorizeHueBand3 = (gui->block2Colorize[6]);
	float block2ColorizeSaturationBand3 = (gui->block2Colorize[7]);
	float block2ColorizeBrightBand3 = (gui->block2Colorize[8]);
	float block2ColorizeHueBand4 = (gui->block2Colorize[9]);
	float block2ColorizeSaturationBand4 = (gui->block2Colorize[10]);
	float block2ColorizeBrightBand4 = (gui->block2Colorize[11]);
	float block2ColorizeHueBand5 = (gui->block2Colorize[12]);
	float block2ColorizeSaturationBand5 = (gui->block2Colorize[13]);
	float block2ColorizeBrightBand5 = (gui->block2Colorize[14]);

	// block2 filters
	float block2BlurAmount = gui->block2Filters[0];
	float block2BlurRadius = block2FilterRadiusC * (gui->block2Filters[1]) + 1.0;
	float block2SharpenAmount = block2SharpenAmountC * (gui->block2Filters[2]);
	float block2SharpenRadius = block2FilterRadiusC * (gui->block2Filters[3]) + 1.0;
	float block2FiltersBoost = (gui->block2Filters[4]);
	float block2Dither = block2DitherC * (1.0 - gui->block2Filters[5]) + 1.0;

	// matrixMixer
	float matrixMixBgRedIntoFgRed = matrixMixC * (gui->matrixMix[0]);
	float matrixMixBgGreenIntoFgRed = matrixMixC * (gui->matrixMix[1]);
	float matrixMixBgBlueIntoFgRed = matrixMixC * (gui->matrixMix[2]);

	float matrixMixBgRedIntoFgGreen = matrixMixC * (gui->matrixMix[3]);
	float matrixMixBgGreenIntoFgGreen = matrixMixC * (gui->matrixMix[4]);
	float matrixMixBgBlueIntoFgGreen = matrixMixC * (gui->matrixMix[5]);

	float matrixMixBgRedIntoFgBlue = matrixMixC * (gui->matrixMix[6]);
	float matrixMixBgGreenIntoFgBlue = matrixMixC * (gui->matrixMix[7]);
	float matrixMixBgBlueIntoFgBlue = matrixMixC * (gui->matrixMix[8]);

	// final mix and key
	float finalMixAmount = mixAmountC * (gui->finalMixAndKey[0]);
	float finalKeyValueRed = keyC * (gui->finalMixAndKey[1]);
	float finalKeyValueGreen = keyC * (gui->finalMixAndKey[2]);
	float finalKeyValueBlue = keyC * (gui->finalMixAndKey[3]);
	float finalKeyThreshold = keyThresholdC * (gui->finalMixAndKey[4]);
	float finalKeySoft = (gui->finalMixAndKey[5]);

	// lfo shit up

	// BLOCK1 inputs lfo

	// ch1
	ch1XDisplace += lfo(ch1XDisplaceC * (gui->ch1AdjustLfo[0]), ch1XDisplaceTheta, 0);
	ch1YDisplace += lfo(ch1YDisplaceC * (gui->ch1AdjustLfo[2]), ch1YDisplaceTheta, 0);
	ch1ZDisplace += lfo(ch1ZDisplaceC * (gui->ch1AdjustLfo[4]), ch1ZDisplaceTheta, 0);
	ch1Rotate += lfo(ch1RotateC * (gui->ch1AdjustLfo[6]), ch1RotateTheta, 0);
	ch1HueAttenuate += lfo((gui->ch1AdjustLfo[8]), ch1HueAttenuateTheta, 0);
	ch1SaturationAttenuate += lfo((gui->ch1AdjustLfo[10]), ch1SaturationAttenuateTheta, 0);
	ch1BrightAttenuate += lfo((gui->ch1AdjustLfo[12]), ch1BrightAttenuateTheta, 0);
	ch1KaleidoscopeSlice += lfo(ch1KaleidoscopeSliceC * (gui->ch1AdjustLfo[14]), ch1KaleidoscopeSliceTheta, 0);

	// ch2 lfo add on
	ch2MixAmount += lfo(mixAmountC * (gui->ch2MixAndKeyLfo[0]), ch2MixAmountTheta, 0);
	ch2KeyThreshold += lfo(keyThresholdC * (gui->ch2MixAndKeyLfo[2]), ch2KeyThresholdTheta, 0);
	ch2KeySoft += lfo((gui->ch2MixAndKeyLfo[4]), ch2KeySoftTheta, 0);

	ch2XDisplace += lfo(ch2XDisplaceC * (gui->ch2AdjustLfo[0]), ch2XDisplaceTheta, 0);
	ch2YDisplace += lfo(ch2YDisplaceC * (gui->ch2AdjustLfo[2]), ch2YDisplaceTheta, 0);
	ch2ZDisplace += lfo(ch2ZDisplaceC * (gui->ch2AdjustLfo[4]), ch2ZDisplaceTheta, 0);
	ch2Rotate += lfo(ch2RotateC * (gui->ch2AdjustLfo[6]), ch2RotateTheta, 0);
	ch2HueAttenuate += lfo((gui->ch2AdjustLfo[8]), ch2HueAttenuateTheta, 0);
	ch2SaturationAttenuate += lfo((gui->ch2AdjustLfo[10]), ch2SaturationAttenuateTheta, 0);
	ch2BrightAttenuate += lfo((gui->ch2AdjustLfo[12]), ch2BrightAttenuateTheta, 0);
	ch2KaleidoscopeSlice += lfo(ch2KaleidoscopeSliceC * (gui->ch2AdjustLfo[14]), ch2KaleidoscopeSliceTheta, 0);

	// fb1 lfo add on
	fb1MixAmount += lfo(mixAmountC * (gui->fb1MixAndKeyLfo[0]), fb1MixAmountTheta, 0);
	fb1KeyThreshold += lfo(keyThresholdC * (gui->fb1MixAndKeyLfo[2]), fb1KeyThresholdTheta, 0);
	fb1KeySoft += lfo((gui->fb1MixAndKeyLfo[4]), fb1KeySoftTheta, 0);

	fb1XDisplace += lfo(fb1XDisplaceC * (gui->fb1Geo1Lfo1[0]), fb1XDisplaceTheta, 0);
	fb1YDisplace += lfo(fb1YDisplaceC * (gui->fb1Geo1Lfo1[2]), fb1YDisplaceTheta, 0);
	fb1ZDisplace += lfo(fb1ZDisplaceC * (gui->fb1Geo1Lfo1[4]), fb1ZDisplaceTheta, 0);
	fb1Rotate += lfo(fb1RotateC * (gui->fb1Geo1Lfo1[6]), fb1RotateTheta, 0);

	fb1ShearMatrix1 += lfo(fb1ShearMatrix1C * (gui->fb1Geo1Lfo2[0]), fb1ShearMatrix1Theta, 0);
	fb1ShearMatrix2 += lfo(fb1ShearMatrix2C * (gui->fb1Geo1Lfo2[4]), fb1ShearMatrix2Theta, 0);
	fb1ShearMatrix3 += lfo(fb1ShearMatrix3C * (gui->fb1Geo1Lfo2[6]), fb1ShearMatrix3Theta, 0);
	fb1ShearMatrix4 += lfo(fb1ShearMatrix4C * (gui->fb1Geo1Lfo2[2]), fb1ShearMatrix4Theta, 0);
	fb1KaleidoscopeSlice += lfo(fb1KaleidoscopeSliceC * (gui->fb1Geo1Lfo2[8]), fb1KaleidoscopeSliceTheta, 0);

	fb1HueAttenuate += lfo(fb1HueAttenuateC * (gui->fb1Color1Lfo1[0]), fb1HueAttenuateTheta, 0);
	fb1SaturationAttenuate += lfo(fb1SaturationAttenuateC * (gui->fb1Color1Lfo1[2]), fb1SaturationAttenuateTheta, 0);
	fb1BrightAttenuate += lfo(fb1BrightAttenuateC * (gui->fb1Color1Lfo1[4]), fb1BrightAttenuateTheta, 0);

	// block2Input
	block2InputXDisplace += lfo(block2InputXDisplaceC * (gui->block2InputAdjustLfo[0]), block2InputXDisplaceTheta, 0);
	block2InputYDisplace += lfo(block2InputYDisplaceC * (gui->block2InputAdjustLfo[2]), block2InputYDisplaceTheta, 0);
	block2InputZDisplace += lfo(block2InputZDisplaceC * (gui->block2InputAdjustLfo[4]), block2InputZDisplaceTheta, 0);
	block2InputRotate += lfo(block2InputRotateC * (gui->block2InputAdjustLfo[6]), block2InputRotateTheta, 0);
	block2InputHueAttenuate += lfo((gui->block2InputAdjustLfo[8]), block2InputHueAttenuateTheta, 0);
	block2InputSaturationAttenuate += lfo((gui->block2InputAdjustLfo[10]), block2InputSaturationAttenuateTheta, 0);
	block2InputBrightAttenuate += lfo((gui->block2InputAdjustLfo[12]), block2InputBrightAttenuateTheta, 0);
	block2InputKaleidoscopeSlice += lfo(block2InputKaleidoscopeSliceC * (gui->block2InputAdjustLfo[14]),
										block2InputKaleidoscopeSliceTheta, 0);

	// fb2 lfo addon
	fb2MixAmount += lfo(mixAmountC * (gui->fb2MixAndKeyLfo[0]), fb2MixAmountTheta, 0);
	fb2KeyThreshold += lfo(keyThresholdC * (gui->fb2MixAndKeyLfo[2]), fb2KeyThresholdTheta, 0);
	fb2KeySoft += lfo((gui->fb2MixAndKeyLfo[4]), fb2KeySoftTheta, 0);

	fb2XDisplace += lfo(fb2XDisplaceC * (gui->fb2Geo1Lfo1[0]), fb2XDisplaceTheta, 0);
	fb2YDisplace += lfo(fb2YDisplaceC * (gui->fb2Geo1Lfo1[2]), fb2YDisplaceTheta, 0);
	fb2ZDisplace += lfo(fb2ZDisplaceC * (gui->fb2Geo1Lfo1[4]), fb2ZDisplaceTheta, 0);
	fb2Rotate += lfo(fb2RotateC * (gui->fb2Geo1Lfo1[6]), fb2RotateTheta, 0);

	fb2ShearMatrix1 += lfo(fb2ShearMatrix1C * (gui->fb2Geo1Lfo2[0]), fb2ShearMatrix1Theta, 0);
	fb2ShearMatrix2 += lfo(fb2ShearMatrix2C * (gui->fb2Geo1Lfo2[4]), fb2ShearMatrix2Theta, 0);
	fb2ShearMatrix3 += lfo(fb2ShearMatrix3C * (gui->fb2Geo1Lfo2[6]), fb2ShearMatrix3Theta, 0);
	fb2ShearMatrix4 += lfo(fb2ShearMatrix4C * (gui->fb2Geo1Lfo2[2]), fb2ShearMatrix4Theta, 0);
	fb2KaleidoscopeSlice += lfo(fb2KaleidoscopeSliceC * (gui->fb2Geo1Lfo2[8]), fb2KaleidoscopeSliceTheta, 0);

	fb2HueAttenuate += lfo(fb2HueAttenuateC * (gui->fb2Color1Lfo1[0]), fb2HueAttenuateTheta, 0);
	fb2SaturationAttenuate += lfo(fb2SaturationAttenuateC * (gui->fb2Color1Lfo1[2]), fb2SaturationAttenuateTheta, 0);
	fb2BrightAttenuate += lfo(fb2BrightAttenuateC * (gui->fb2Color1Lfo1[4]), fb2BrightAttenuateTheta, 0);

	// BLOCK3

	// block1 geo
	block1XDisplace += lfo(block1XDisplaceC * (gui->block1Geo1Lfo1[0]), block1XDisplaceTheta, 0);
	block1YDisplace += lfo(block1YDisplaceC * (gui->block1Geo1Lfo1[2]), block1YDisplaceTheta, 0);
	block1ZDisplace += lfo(block1ZDisplaceC * (gui->block1Geo1Lfo1[4]), block1ZDisplaceTheta, 0);
	block1Rotate += lfo(block1RotateC * (gui->block1Geo1Lfo1[6]), block1RotateTheta, 0);

	block1ShearMatrix1 += lfo(block1ShearMatrix1C * (gui->block1Geo1Lfo2[0]), block1ShearMatrix1Theta, 0);
	block1ShearMatrix2 += lfo(block1ShearMatrix2C * (gui->block1Geo1Lfo2[4]), block1ShearMatrix2Theta, 0);
	block1ShearMatrix3 += lfo(block1ShearMatrix3C * (gui->block1Geo1Lfo2[6]), block1ShearMatrix3Theta, 0);
	block1ShearMatrix4 += lfo(block1ShearMatrix4C * (gui->block1Geo1Lfo2[2]), block1ShearMatrix4Theta, 0);
	block1KaleidoscopeSlice += lfo(block1KaleidoscopeSliceC * (gui->block1Geo1Lfo2[8]), block1KaleidoscopeSliceTheta, 0);

	// block1 colorize
	block1ColorizeHueBand1 += lfo((gui->block1ColorizeLfo1[0]), block1ColorizeHueBand1Theta, 0);
	block1ColorizeSaturationBand1 += lfo((gui->block1ColorizeLfo1[1]), block1ColorizeSaturationBand1Theta, 0);
	block1ColorizeBrightBand1 += lfo((gui->block1ColorizeLfo1[2]), block1ColorizeBrightBand1Theta, 0);
	block1ColorizeHueBand2 += lfo((gui->block1ColorizeLfo1[6]), block1ColorizeHueBand2Theta, 0);
	block1ColorizeSaturationBand2 += lfo((gui->block1ColorizeLfo1[7]), block1ColorizeSaturationBand2Theta, 0);
	block1ColorizeBrightBand2 += lfo((gui->block1ColorizeLfo1[8]), block1ColorizeBrightBand2Theta, 0);

	block1ColorizeHueBand3 += lfo((gui->block1ColorizeLfo2[0]), block1ColorizeHueBand3Theta, 0);
	block1ColorizeSaturationBand3 += lfo((gui->block1ColorizeLfo2[1]), block1ColorizeSaturationBand3Theta, 0);
	block1ColorizeBrightBand3 += lfo((gui->block1ColorizeLfo2[2]), block1ColorizeBrightBand3Theta, 0);
	block1ColorizeHueBand4 += lfo((gui->block1ColorizeLfo2[6]), block1ColorizeHueBand4Theta, 0);
	block1ColorizeSaturationBand4 += lfo((gui->block1ColorizeLfo2[7]), block1ColorizeSaturationBand4Theta, 0);
	block1ColorizeBrightBand4 += lfo((gui->block1ColorizeLfo2[8]), block1ColorizeBrightBand4Theta, 0);

	block1ColorizeHueBand5 += lfo((gui->block1ColorizeLfo3[0]), block1ColorizeHueBand5Theta, 0);
	block1ColorizeSaturationBand5 += lfo((gui->block1ColorizeLfo3[1]), block1ColorizeSaturationBand5Theta, 0);
	block1ColorizeBrightBand5 += lfo((gui->block1ColorizeLfo3[2]), block1ColorizeBrightBand5Theta, 0);

	// block2 geo
	block2XDisplace += lfo(block2XDisplaceC * (gui->block2Geo1Lfo1[0]), block2XDisplaceTheta, 0);
	block2YDisplace += lfo(block2YDisplaceC * (gui->block2Geo1Lfo1[2]), block2YDisplaceTheta, 0);
	block2ZDisplace += lfo(block2ZDisplaceC * (gui->block2Geo1Lfo1[4]), block2ZDisplaceTheta, 0);
	block2Rotate += lfo(block2RotateC * (gui->block2Geo1Lfo1[6]), block2RotateTheta, 0);

	block2ShearMatrix1 += lfo(block2ShearMatrix1C * (gui->block2Geo1Lfo2[0]), block2ShearMatrix1Theta, 0);
	block2ShearMatrix2 += lfo(block2ShearMatrix2C * (gui->block2Geo1Lfo2[4]), block2ShearMatrix2Theta, 0);
	block2ShearMatrix3 += lfo(block2ShearMatrix3C * (gui->block2Geo1Lfo2[6]), block2ShearMatrix3Theta, 0);
	block2ShearMatrix4 += lfo(block2ShearMatrix4C * (gui->block2Geo1Lfo2[2]), block2ShearMatrix4Theta, 0);
	block2KaleidoscopeSlice += lfo(block2KaleidoscopeSliceC * (gui->block2Geo1Lfo2[8]), block2KaleidoscopeSliceTheta, 0);

	// block2 colorize
	block2ColorizeHueBand1 += lfo((gui->block2ColorizeLfo1[0]), block2ColorizeHueBand1Theta, 0);
	block2ColorizeSaturationBand1 += lfo((gui->block2ColorizeLfo1[1]), block2ColorizeSaturationBand1Theta, 0);
	block2ColorizeBrightBand1 += lfo((gui->block2ColorizeLfo1[2]), block2ColorizeBrightBand1Theta, 0);
	block2ColorizeHueBand2 += lfo((gui->block2ColorizeLfo1[6]), block2ColorizeHueBand2Theta, 0);
	block2ColorizeSaturationBand2 += lfo((gui->block2ColorizeLfo1[7]), block2ColorizeSaturationBand2Theta, 0);
	block2ColorizeBrightBand2 += lfo((gui->block2ColorizeLfo1[8]), block2ColorizeBrightBand2Theta, 0);

	block2ColorizeHueBand3 += lfo((gui->block2ColorizeLfo2[0]), block2ColorizeHueBand3Theta, 0);
	block2ColorizeSaturationBand3 += lfo((gui->block2ColorizeLfo2[1]), block2ColorizeSaturationBand3Theta, 0);
	block2ColorizeBrightBand3 += lfo((gui->block2ColorizeLfo2[2]), block2ColorizeBrightBand3Theta, 0);
	block2ColorizeHueBand4 += lfo((gui->block2ColorizeLfo2[6]), block2ColorizeHueBand4Theta, 0);
	block2ColorizeSaturationBand4 += lfo((gui->block2ColorizeLfo2[7]), block2ColorizeSaturationBand4Theta, 0);
	block2ColorizeBrightBand4 += lfo((gui->block2ColorizeLfo2[8]), block2ColorizeBrightBand4Theta, 0);

	block2ColorizeHueBand5 += lfo((gui->block2ColorizeLfo3[0]), block2ColorizeHueBand5Theta, 0);
	block2ColorizeSaturationBand5 += lfo((gui->block2ColorizeLfo3[1]), block2ColorizeSaturationBand5Theta, 0);
	block2ColorizeBrightBand5 += lfo((gui->block2ColorizeLfo3[2]), block2ColorizeBrightBand5Theta, 0);

	// matrix mixer
	matrixMixBgRedIntoFgRed += lfo(matrixMixC * (gui->matrixMixLfo1[0]), matrixMixBgRedIntoFgRedTheta, 0);
	matrixMixBgGreenIntoFgRed += lfo(matrixMixC * (gui->matrixMixLfo1[1]), matrixMixBgGreenIntoFgRedTheta, 0);
	matrixMixBgBlueIntoFgRed += lfo(matrixMixC * (gui->matrixMixLfo1[2]), matrixMixBgBlueIntoFgRedTheta, 0);

	matrixMixBgRedIntoFgGreen += lfo(matrixMixC * (gui->matrixMixLfo1[6]), matrixMixBgRedIntoFgGreenTheta, 0);
	matrixMixBgGreenIntoFgGreen += lfo(matrixMixC * (gui->matrixMixLfo1[7]), matrixMixBgGreenIntoFgGreenTheta, 0);
	matrixMixBgBlueIntoFgGreen += lfo(matrixMixC * (gui->matrixMixLfo1[8]), matrixMixBgBlueIntoFgGreenTheta, 0);

	matrixMixBgRedIntoFgBlue += lfo(matrixMixC * (gui->matrixMixLfo2[0]), matrixMixBgRedIntoFgBlueTheta, 0);
	matrixMixBgGreenIntoFgBlue += lfo(matrixMixC * (gui->matrixMixLfo2[1]), matrixMixBgGreenIntoFgBlueTheta, 0);
	matrixMixBgBlueIntoFgBlue += lfo(matrixMixC * (gui->matrixMixLfo2[2]), matrixMixBgBlueIntoFgBlueTheta, 0);

	// final lfo addon
	finalMixAmount += lfo(mixAmountC * (gui->finalMixAndKeyLfo[0]), finalMixAmountTheta, 0);
	finalKeyThreshold += lfo(keyThresholdC * (gui->finalMixAndKeyLfo[2]), finalKeyThresholdTheta, 0);
	finalKeySoft += lfo((gui->finalMixAndKeyLfo[4]), finalKeySoftTheta, 0);

	// this is where i do some jury rigged presets to speed up tests
	// remember to comment these out later lol

	// fb1KeyThreshold+=keyThresholdC*(.68);

	// is this still used...
	float ratio = input1.getWidth() / ofGetWidth();

	framebuffer1.begin();
	shader1.begin();

	// various test parameters to delete later
	float ch1CribX = 0;
	float ch2CribX = 0;
	float cribY = 60;
	float ch1HdZCrib = 0;
	float ch2HdZCrib = 0;
	shader1.setUniform1f("cribY", cribY);
	shader1.setUniform1f("width", ofGetWidth());
	shader1.setUniform1f("height", ofGetHeight());
	shader1.setUniform1f("inverseWidth", 1.0f / ofGetWidth());
	shader1.setUniform1f("inverseHeight", 1.0f / ofGetHeight());

	shader1.setUniform1f("inverseWidth1", 1.0f / 640.0);
	shader1.setUniform1f("inverseHeight1", 1.0f / 480.0);

	// shader1.setUniform1f("hdFixX",gui->hdFixX);
	// shader1.setUniform1f("hdFixY",gui->hdFixY);

	int pastFrames1Index = (abs(pastFramesOffset - pastFramesSize - (gui->fb1DelayTime) + 1) % pastFramesSize);
	int TemporalFilterIndex = (abs(pastFramesOffset - pastFramesSize + 1) % pastFramesSize);
	pastFrames1[pastFrames1Index].draw(0, 0);
	shader1.setUniformTexture("fb1TemporalFilter", pastFrames1[TemporalFilterIndex].getTexture(), 1);

	// channel selection
	if (gui->ch1InputSelect == 0)
	{
		shader1.setUniformTexture("ch1Tex", input1.getTexture(), 2);
	}
	if (gui->ch1InputSelect == 1)
	{
		shader1.setUniformTexture("ch1Tex", input2.getTexture(), 2);
	}
	// ch1 parameters

	// we can use this to fix the uncentering of the hd version for now
	shader1.setUniform2f("input1XYFix", ofVec2f(gui->input1XFix, gui->input1YFix));

	// we ADD a logic here for sd pillarbox vs sd fullscreen?
	float ch1ScaleFix = .5; // fullscreen

	// default aspect ratio is 4:3
	// but if we are only sending in 640x480 resolutions anyway
	// we should double check some shit
	float ch1AspectRatio = 1.0 + gui->sdFixX;
	if (gui->ch1AspectRatioSwitch == 0)
	{
		ch1AspectRatio = 1.1;
		ch1CribX = 64; // gui->input1XFix;
		// ch1HdZCrib=-.205;
		ch1HdZCrib = -.06; // gui->input1ScaleFix;
	}

	if (gui->ch1AspectRatioSwitch == 1)
	{
		ch1AspectRatio = .83;
		ch1CribX = -109;
		// ch1HdZCrib=-.205;
		ch1HdZCrib = .1;
	}
	shader1.setUniform1f("ch1ScaleFix", ch1ScaleFix);

	shader1.setUniform1f("ch1HdZCrib", ch1HdZCrib);
	shader1.setUniform1f("ch1CribX", ch1CribX);
	shader1.setUniform1f("ch1AspectRatio", ch1AspectRatio);

	shader1.setUniform2f("ch1XYDisplace", ofVec2f(ch1XDisplace, ch1YDisplace));
	// remapping z
	// but maybe not anymore
	/*
	float ch1ZDisplaceMapped=ch1ZDisplace;
	if(ch1ZDisplaceMapped>1.0){
		ch1ZDisplaceMapped=pow(2,(ch1ZDisplaceMapped-1.0f)*8.0f);
		if(ch1ZDisplace>=2.0){ch1ZDisplaceMapped=1000;}
	}
	*/

	shader1.setUniform1f("ch1ZDisplace", ch1ZDisplace);
	shader1.setUniform1f("ch1Rotate", ch1Rotate);
	shader1.setUniform3f("ch1HSBAttenuate", ofVec3f(ch1HueAttenuate, ch1SaturationAttenuate, ch1BrightAttenuate));
	if (gui->ch1Adjust[7] > 0)
	{
		ch1PosterizeSwitch = 1;
	}
	shader1.setUniform1f("ch1Posterize", ch1Posterize);
	shader1.setUniform1f("ch1PosterizeInvert", ch1PosterizeInvert);
	shader1.setUniform1i("ch1PosterizeSwitch", ch1PosterizeSwitch);
	shader1.setUniform1f("ch1KaleidoscopeAmount", ch1KaleidoscopeAmount);
	shader1.setUniform1f("ch1KaleidoscopeSlice", ch1KaleidoscopeSlice);
	shader1.setUniform1f("ch1BlurAmount", ch1BlurAmount);
	shader1.setUniform1f("ch1BlurRadius", ch1BlurRadius);
	shader1.setUniform1f("ch1SharpenAmount", ch1SharpenAmount);
	shader1.setUniform1f("ch1SharpenRadius", ch1SharpenRadius);
	shader1.setUniform1f("ch1FiltersBoost", ch1FiltersBoost);

	shader1.setUniform1i("ch1GeoOverflow", gui->ch1GeoOverflow);
	shader1.setUniform1i("ch1HMirror", gui->ch1HMirror);
	shader1.setUniform1i("ch1VMirror", gui->ch1VMirror);
	shader1.setUniform1i("ch1HFlip", gui->ch1HFlip);
	shader1.setUniform1i("ch1VFlip", gui->ch1VFlip);
	shader1.setUniform1i("ch1HueInvert", gui->ch1HueInvert);
	shader1.setUniform1i("ch1SaturationInvert", gui->ch1SaturationInvert);
	shader1.setUniform1i("ch1BrightInvert", gui->ch1BrightInvert);
	shader1.setUniform1i("ch1RGBInvert", gui->ch1RGBInvert);
	shader1.setUniform1i("ch1Solarize", gui->ch1Solarize);

	// channel selection
	if (gui->ch2InputSelect == 0)
	{
		shader1.setUniformTexture("ch2Tex", input1.getTexture(), 3);
	}
	if (gui->ch2InputSelect == 1)
	{
		shader1.setUniformTexture("ch2Tex", input2.getTexture(), 3);
	}

	// we ADD a logic here for sd pillarbox vs sd fullscreen?
	float ch2ScaleFix = .5; // fullscreen

	// default aspect ratio is 4:3
	// but if we are only sending in 640x480 resolutions anyway
	// we should double check some shit
	float ch2AspectRatio = 1.0 + gui->sdFixX;
	if (gui->ch2AspectRatioSwitch == 0)
	{
		ch2AspectRatio = 1.1;
		ch2CribX = 64; // gui->input1XFix;
		// ch2HdZCrib=-.205;
		ch2HdZCrib = -.06; // gui->input1ScaleFix;
	}

	if (gui->ch2AspectRatioSwitch == 1)
	{
		ch2AspectRatio = .83;
		ch2CribX = -109;
		// ch2HdZCrib=-.205;
		ch2HdZCrib = .1;
	}
	shader1.setUniform1f("ch2ScaleFix", ch2ScaleFix);
	shader1.setUniform1f("ch2CribX", ch2CribX);
	shader1.setUniform1f("ch2AspectRatio", ch2AspectRatio);
	shader1.setUniform1f("ch2HdZCrib", ch2HdZCrib);

	shader1.setUniform1f("ratio", ratio);

	// ch2 mix parameters
	shader1.setUniform1f("ch2MixAmount", ch2MixAmount);
	shader1.setUniform3f("ch2KeyValue", ofVec3f(ch2KeyValueRed, ch2KeyValueGreen, ch2KeyValueBlue));
	shader1.setUniform1f("ch2KeyThreshold", ch2KeyThreshold);
	shader1.setUniform1f("ch2KeySoft", ch2KeySoft);
	shader1.setUniform1i("ch2KeyOrder", gui->ch2KeyOrder);
	shader1.setUniform1i("ch2MixType", gui->ch2MixType);
	shader1.setUniform1i("ch2MixOverflow", gui->ch2MixOverflow);

	// ch2 adjust
	shader1.setUniform2f("ch2XYDisplace", ofVec2f(ch2XDisplace, ch2YDisplace));
	// remapping z
	float ch2ZDisplaceMapped = ch2ZDisplace;
	if (ch2ZDisplaceMapped > 1.0)
	{
		ch2ZDisplaceMapped = pow(2, (ch2ZDisplaceMapped - 1.0f) * 8.0f);
		if (ch2ZDisplace >= 2.0)
		{
			ch2ZDisplaceMapped = 1000;
		}
	}
	shader1.setUniform1f("ch2ZDisplace", ch2ZDisplaceMapped);
	shader1.setUniform1f("ch2Rotate", ch2Rotate);
	shader1.setUniform3f("ch2HSBAttenuate", ofVec3f(ch2HueAttenuate, ch2SaturationAttenuate, ch2BrightAttenuate));
	if (gui->ch2Adjust[7] > 0)
	{
		ch2PosterizeSwitch = 1;
	}
	shader1.setUniform1f("ch2Posterize", ch2Posterize);
	shader1.setUniform1f("ch2PosterizeInvert", ch2PosterizeInvert);
	shader1.setUniform1i("ch2PosterizeSwitch", ch2PosterizeSwitch);
	shader1.setUniform1f("ch2KaleidoscopeAmount", ch2KaleidoscopeAmount);
	shader1.setUniform1f("ch2KaleidoscopeSlice", ch2KaleidoscopeSlice);
	shader1.setUniform1f("ch2BlurAmount", ch2BlurAmount);
	shader1.setUniform1f("ch2BlurRadius", ch2BlurRadius);
	shader1.setUniform1f("ch2SharpenAmount", ch2SharpenAmount);
	shader1.setUniform1f("ch2SharpenRadius", ch2SharpenRadius);
	shader1.setUniform1f("ch2FiltersBoost", ch2FiltersBoost);

	shader1.setUniform1i("ch2GeoOverflow", gui->ch2GeoOverflow);
	shader1.setUniform1i("ch2HMirror", gui->ch2HMirror);
	shader1.setUniform1i("ch2VMirror", gui->ch2VMirror);
	shader1.setUniform1i("ch2HFlip", gui->ch2HFlip);
	shader1.setUniform1i("ch2VFlip", gui->ch2VFlip);
	shader1.setUniform1i("ch2HueInvert", gui->ch2HueInvert);
	shader1.setUniform1i("ch2SaturationInvert", gui->ch2SaturationInvert);
	shader1.setUniform1i("ch2BrightInvert", gui->ch2BrightInvert);
	shader1.setUniform1i("ch2RGBInvert", gui->ch2RGBInvert);
	shader1.setUniform1i("ch2Solarize", gui->ch2Solarize);

	// fb1 parameters
	// fb1mixnkey
	shader1.setUniform1f("fb1MixAmount", fb1MixAmount);
	shader1.setUniform3f("fb1KeyValue", ofVec3f(fb1KeyValueRed, fb1KeyValueGreen, fb1KeyValueBlue));
	shader1.setUniform1f("fb1KeyThreshold", fb1KeyThreshold);
	shader1.setUniform1f("fb1KeySoft", fb1KeySoft);
	shader1.setUniform1i("fb1KeyOrder", gui->fb1KeyOrder);
	shader1.setUniform1i("fb1MixType", gui->fb1MixType);
	shader1.setUniform1i("fb1MixOverflow", gui->fb1MixOverflow);
	// fb1geo1
	shader1.setUniform2f("fb1XYDisplace", ofVec2f(fb1XDisplace, fb1YDisplace));
	shader1.setUniform1f("fb1ZDisplace", fb1ZDisplace);
	shader1.setUniform1f("fb1Rotate", fb1Rotate);
	shader1.setUniform4f("fb1ShearMatrix", ofVec4f(fb1ShearMatrix1, fb1ShearMatrix2, fb1ShearMatrix3, fb1ShearMatrix4));
	shader1.setUniform1f("fb1KaleidoscopeAmount", fb1KaleidoscopeAmount);
	shader1.setUniform1f("fb1KaleidoscopeSlice", fb1KaleidoscopeSlice);

	shader1.setUniform1i("fb1HMirror", gui->fb1HMirror);
	shader1.setUniform1i("fb1VMirror", gui->fb1VMirror);
	shader1.setUniform1i("fb1HFlip", gui->fb1HFlip);
	shader1.setUniform1i("fb1VFlip", gui->fb1VFlip);
	shader1.setUniform1i("fb1RotateMode", gui->fb1RotateMode);
	shader1.setUniform1i("fb1GeoOverflow", gui->fb1GeoOverflow);

	shader1.setUniform3f("fb1HSBOffset", ofVec3f(fb1HueOffset, fb1SaturationOffset, fb1BrightOffset));
	shader1.setUniform3f("fb1HSBAttenuate", ofVec3f(fb1HueAttenuate, fb1SaturationAttenuate, fb1BrightAttenuate));
	shader1.setUniform3f("fb1HSBPowmap", ofVec3f(fb1HuePowmap, fb1SaturationPowmap, fb1BrightPowmap));
	shader1.setUniform1f("fb1HueShaper", fb1HueShaper);

	if (gui->fb1Color1[10] > 0)
	{
		fb1PosterizeSwitch = 1;
	}
	shader1.setUniform1f("fb1Posterize", fb1Posterize);
	shader1.setUniform1f("fb1PosterizeInvert", fb1PosterizeInvert);
	shader1.setUniform1i("fb1PosterizeSwitch", fb1PosterizeSwitch);

	shader1.setUniform1i("fb1HueInvert", gui->fb1HueInvert);
	shader1.setUniform1i("fb1SaturationInvert", gui->fb1SaturationInvert);
	shader1.setUniform1i("fb1BrightInvert", gui->fb1BrightInvert);

	// fb1 filters
	shader1.setUniform1f("fb1BlurAmount", fb1BlurAmount);
	shader1.setUniform1f("fb1BlurRadius", fb1BlurRadius);
	shader1.setUniform1f("fb1SharpenAmount", fb1SharpenAmount);
	shader1.setUniform1f("fb1SharpenRadius", fb1SharpenRadius);
	shader1.setUniform1f("fb1TemporalFilter1Amount", fb1TemporalFilter1Amount);
	shader1.setUniform1f("fb1TemporalFilter1Resonance", fb1TemporalFilter1Resonance);
	shader1.setUniform1f("fb1TemporalFilter2Amount", fb1TemporalFilter2Amount);
	shader1.setUniform1f("fb1TemporalFilter2Resonance", fb1TemporalFilter2Resonance);
	shader1.setUniform1f("fb1FiltersBoost", fb1FiltersBoost);

	shader1.end();

	if (gui->block1HypercubeSwitch == 1)
	{
		hypercube_draw();
	}

	if (gui->block1LineSwitch == 1)
	{
		line_draw();
	}
	if (gui->block1SevenStar1Switch == 1)
	{
		sevenStar1Draw();
	}
	if (gui->block1LissaBallSwitch == 1)
	{
		drawSpiralEllipse();
	}
	framebuffer1.end();

	// BLOCK_2

	framebuffer2.begin();
	shader2.begin();

	shader2.setUniform1f("width", ofGetWidth());
	shader2.setUniform1f("height", ofGetHeight());
	shader2.setUniform1f("inverseWidth", 1.0f / ofGetWidth());
	shader2.setUniform1f("inverseHeight", 1.0f / ofGetHeight());

	shader2.setUniform1f("inverseWidth1", 1.0f / 640.0);
	shader2.setUniform1f("inverseHeight1", 1.0f / 480.0);

	// draw pastframes2
	int pastFrames2Index = (abs(pastFramesOffset - pastFramesSize - (gui->fb2DelayTime) + 1) % pastFramesSize);
	pastFrames2[pastFrames2Index].draw(0, 0);
	// send the temporal filter
	int fb2TemporalFilterIndex = (abs(pastFramesOffset - pastFramesSize + 1) % pastFramesSize);
	shader2.setUniformTexture("fb2TemporalFilter", pastFrames2[fb2TemporalFilterIndex].getTexture(), 5);

	bool block2InputMasterSwitch = 0;
	float block2InputWidth = 1280;
	float block2InputHeight = 720;
	float block2InputWidthHalf = 640;
	float block2InputHeightHalf = 360;
	// choose block2 input
	float block2AspectRatio = 1.0;
	if (gui->block2InputSelect == 0)
	{
		ratio = 1.0;
		shader2.setUniformTexture("block2InputTex", framebuffer1.getTexture(), 6);
	}

	if (gui->block2InputSelect == 1)
	{
		ratio = input1.getWidth() / ofGetWidth();
		block2InputMasterSwitch = 1;
		shader2.setUniformTexture("block2InputTex", input1.getTexture(), 6);
		block2InputWidth = 640;
		block2InputHeight = 480;
		block2InputWidthHalf = 320;
		block2InputHeightHalf = 240;
	}

	if (gui->block2InputSelect == 2)
	{
		ratio = input2.getWidth() / ofGetWidth();
		block2InputMasterSwitch = 1;
		shader2.setUniformTexture("block2InputTex", input2.getTexture(), 6);
		block2InputWidth = 640;
		block2InputHeight = 480;
		block2InputWidthHalf = 320;
		block2InputHeightHalf = 240;
	}

	shader2.setUniform1f("ratio", ratio);
	shader2.setUniform1i("block2InputMasterSwitch", block2InputMasterSwitch);
	shader2.setUniform1f("block2InputWidth", block2InputWidth);
	shader2.setUniform1f("block2InputHeight", block2InputHeight);
	shader2.setUniform1f("block2InputWidthHalf", block2InputWidthHalf);
	shader2.setUniform1f("block2InputHeightHalf", block2InputHeightHalf);
	// we ADD a logic here for sd pillarbox vs sd fullscreen?
	float block2InputScaleFix = .5; // fullscreen
	float block2InputCribX = 0;
	float block2InputHdZCrib = 0;
	// default aspect ratio is 4:3
	// but if we are only sending in 640x480 resolutions anyway
	// we should double check some shit
	float block2InputAspectRatio = 1.0 + gui->sdFixX;
	if (gui->block2InputAspectRatioSwitch == 0)
	{
		block2InputAspectRatio = 1.1;
		block2InputCribX = 64; // gui->input1XFix;
		// block2InputHdZCrib=-.205;
		block2InputHdZCrib = -.06; // gui->input1ScaleFix;
	}

	if (gui->block2InputAspectRatioSwitch == 1)
	{
		block2InputAspectRatio = .83;
		block2InputCribX = -109;
		// block2InputHdZCrib=-.205;
		block2InputHdZCrib = .1;
	}
	shader2.setUniform1f("block2InputScaleFix", block2InputScaleFix);

	shader2.setUniform1f("block2InputHdZCrib", block2InputHdZCrib);
	shader2.setUniform1f("block2InputCribX", block2InputCribX);
	shader2.setUniform1f("block2InputAspectRatio", block2InputAspectRatio);

	shader2.setUniform2f("block2InputXYDisplace", ofVec2f(block2InputXDisplace, block2InputYDisplace));
	// remapping z
	float block2InputZDisplaceMapped = block2InputZDisplace;
	if (block2InputZDisplaceMapped > 1.0)
	{
		block2InputZDisplaceMapped = pow(2, (block2InputZDisplaceMapped - 1.0f) * 8.0f);
		if (block2InputZDisplace >= 2.0)
		{
			block2InputZDisplaceMapped = 1000;
		}
	}
	shader2.setUniform1f("block2InputZDisplace", block2InputZDisplaceMapped);
	shader2.setUniform1f("block2InputRotate", block2InputRotate);
	shader2.setUniform3f("block2InputHSBAttenuate", ofVec3f(block2InputHueAttenuate, block2InputSaturationAttenuate, block2InputBrightAttenuate));
	if (gui->block2InputAdjust[7] > 0)
	{
		block2InputPosterizeSwitch = 1;
	}
	shader2.setUniform1f("block2InputPosterize", block2InputPosterize);
	shader2.setUniform1f("block2InputPosterizeInvert", block2InputPosterizeInvert);
	shader2.setUniform1i("block2InputPosterizeSwitch", block2InputPosterizeSwitch);
	shader2.setUniform1f("block2InputKaleidoscopeAmount", block2InputKaleidoscopeAmount);
	shader2.setUniform1f("block2InputKaleidoscopeSlice", block2InputKaleidoscopeSlice);
	shader2.setUniform1f("block2InputBlurAmount", block2InputBlurAmount);
	shader2.setUniform1f("block2InputBlurRadius", block2InputBlurRadius);
	shader2.setUniform1f("block2InputSharpenAmount", block2InputSharpenAmount);
	shader2.setUniform1f("block2InputSharpenRadius", block2InputSharpenRadius);
	shader2.setUniform1f("block2InputFiltersBoost", block2InputFiltersBoost);

	shader2.setUniform1i("block2InputGeoOverflow", gui->block2InputGeoOverflow);
	shader2.setUniform1i("block2InputHMirror", gui->block2InputHMirror);
	shader2.setUniform1i("block2InputVMirror", gui->block2InputVMirror);
	shader2.setUniform1i("block2InputHFlip", gui->block2InputHFlip);
	shader2.setUniform1i("block2InputVFlip", gui->block2InputVFlip);
	shader2.setUniform1i("block2InputHueInvert", gui->block2InputHueInvert);
	shader2.setUniform1i("block2InputSaturationInvert", gui->block2InputSaturationInvert);
	shader2.setUniform1i("block2InputBrightInvert", gui->block2InputBrightInvert);
	shader2.setUniform1i("block2InputRGBInvert", gui->block2InputRGBInvert);
	shader2.setUniform1i("block2InputSolarize", gui->block2InputSolarize);

	// fb2 parameters
	// fb2mixnkey
	shader2.setUniform1f("fb2MixAmount", fb2MixAmount);
	shader2.setUniform3f("fb2KeyValue", ofVec3f(fb2KeyValueRed, fb2KeyValueGreen, fb2KeyValueBlue));
	shader2.setUniform1f("fb2KeyThreshold", fb2KeyThreshold);
	shader2.setUniform1f("fb2KeySoft", fb2KeySoft);
	shader2.setUniform1i("fb2KeyOrder", gui->fb2KeyOrder);
	shader2.setUniform1i("fb2MixType", gui->fb2MixType);
	shader2.setUniform1i("fb2MixOverflow", gui->fb2MixOverflow);
	// fb2geo1
	shader2.setUniform2f("fb2XYDisplace", ofVec2f(fb2XDisplace, fb2YDisplace));
	shader2.setUniform1f("fb2ZDisplace", fb2ZDisplace);
	shader2.setUniform1f("fb2Rotate", fb2Rotate);
	shader2.setUniform4f("fb2ShearMatrix", ofVec4f(fb2ShearMatrix1, fb2ShearMatrix2, fb2ShearMatrix3, fb2ShearMatrix4));
	shader2.setUniform1f("fb2KaleidoscopeAmount", fb2KaleidoscopeAmount);
	shader2.setUniform1f("fb2KaleidoscopeSlice", fb2KaleidoscopeSlice);

	shader2.setUniform1i("fb2HMirror", gui->fb2HMirror);
	shader2.setUniform1i("fb2VMirror", gui->fb2VMirror);
	shader2.setUniform1i("fb2HFlip", gui->fb2HFlip);
	shader2.setUniform1i("fb2VFlip", gui->fb2VFlip);
	shader2.setUniform1i("fb2RotateMode", gui->fb2RotateMode);
	shader2.setUniform1i("fb2GeoOverflow", gui->fb2GeoOverflow);

	shader2.setUniform3f("fb2HSBOffset", ofVec3f(fb2HueOffset, fb2SaturationOffset, fb2BrightOffset));
	shader2.setUniform3f("fb2HSBAttenuate", ofVec3f(fb2HueAttenuate, fb2SaturationAttenuate, fb2BrightAttenuate));
	shader2.setUniform3f("fb2HSBPowmap", ofVec3f(fb2HuePowmap, fb2SaturationPowmap, fb2BrightPowmap));
	shader2.setUniform1f("fb2HueShaper", fb2HueShaper);

	if (gui->fb2Color1[10] > 0)
	{
		fb2PosterizeSwitch = 1;
	}
	shader2.setUniform1f("fb2Posterize", fb2Posterize);
	shader2.setUniform1f("fb2PosterizeInvert", fb2PosterizeInvert);
	shader2.setUniform1i("fb2PosterizeSwitch", fb2PosterizeSwitch);

	shader2.setUniform1i("fb2HueInvert", gui->fb2HueInvert);
	shader2.setUniform1i("fb2SaturationInvert", gui->fb2SaturationInvert);
	shader2.setUniform1i("fb2BrightInvert", gui->fb2BrightInvert);

	// fb2 filters
	shader2.setUniform1f("fb2BlurAmount", fb2BlurAmount);
	shader2.setUniform1f("fb2BlurRadius", fb2BlurRadius);
	shader2.setUniform1f("fb2SharpenAmount", fb2SharpenAmount);
	shader2.setUniform1f("fb2SharpenRadius", fb2SharpenRadius);
	shader2.setUniform1f("fb2TemporalFilter1Amount", fb2TemporalFilter1Amount);
	shader2.setUniform1f("fb2TemporalFilter1Resonance", fb2TemporalFilter1Resonance);
	shader2.setUniform1f("fb2TemporalFilter2Amount", fb2TemporalFilter2Amount);
	shader2.setUniform1f("fb2TemporalFilter2Resonance", fb2TemporalFilter2Resonance);
	shader2.setUniform1f("fb2FiltersBoost", fb2FiltersBoost);

	shader2.end();

	if (gui->block2HypercubeSwitch == 1)
	{
		hypercube_draw();
	}

	if (gui->block2LineSwitch == 1)
	{
		line_draw();
	}
	if (gui->block2SevenStar1Switch == 1)
	{
		sevenStar1Draw();
	}
	if (gui->block2LissaBallSwitch == 1)
	{
		drawSpiralEllipse();
	}

	framebuffer2.end();

	// FINAL MIX OUT
	framebuffer3.begin();
	shader3.begin();
	dummyTex.draw(0, 0);

	shader3.setUniformTexture("block2Output", framebuffer2.getTexture(), 8);
	shader3.setUniformTexture("block1Output", framebuffer1.getTexture(), 9);

	shader3.setUniform1f("width", ofGetWidth());
	shader3.setUniform1f("height", ofGetHeight());
	shader3.setUniform1f("inverseWidth", 1.0f / ofGetWidth());
	shader3.setUniform1f("inverseHeight", 1.0f / ofGetHeight());

	// block1geo1
	shader3.setUniform2f("block1XYDisplace", ofVec2f(block1XDisplace, block1YDisplace));
	// remapping z
	float block1ZDisplaceMapped = block1ZDisplace;
	if (block1ZDisplaceMapped > 1.0)
	{
		block1ZDisplaceMapped = pow(2, (block1ZDisplaceMapped - 1.0f) * 8.0f);
		if (block1ZDisplace >= 2.0)
		{
			block1ZDisplaceMapped = 1000;
		}
	}
	shader3.setUniform1f("block1ZDisplace", block1ZDisplaceMapped);
	shader3.setUniform1f("block1Rotate", block1Rotate);
	shader3.setUniform4f("block1ShearMatrix", ofVec4f(block1ShearMatrix1,
													  block1ShearMatrix2, block1ShearMatrix3, block1ShearMatrix4));
	shader3.setUniform1f("block1KaleidoscopeAmount", block1KaleidoscopeAmount);
	shader3.setUniform1f("block1KaleidoscopeSlice", block1KaleidoscopeSlice);

	shader3.setUniform1i("block1HMirror", gui->block1HMirror);
	shader3.setUniform1i("block1VMirror", gui->block1VMirror);
	shader3.setUniform1i("block1HFlip", gui->block1HFlip);
	shader3.setUniform1i("block1VFlip", gui->block1VFlip);
	shader3.setUniform1i("block1RotateMode", gui->block1RotateMode);
	shader3.setUniform1i("block1GeoOverflow", gui->block1GeoOverflow);

	// block1 colorize
	shader3.setUniform1i("block1ColorizeSwitch", gui->block1ColorizeSwitch);
	shader3.setUniform1i("block1ColorizeHSB_RGB", gui->block1ColorizeHSB_RGB);

	shader3.setUniform3f("block1ColorizeBand1", ofVec3f(block1ColorizeHueBand1,
														block1ColorizeSaturationBand1, block1ColorizeBrightBand1));
	shader3.setUniform3f("block1ColorizeBand2", ofVec3f(block1ColorizeHueBand2,
														block1ColorizeSaturationBand2, block1ColorizeBrightBand2));
	shader3.setUniform3f("block1ColorizeBand3", ofVec3f(block1ColorizeHueBand3,
														block1ColorizeSaturationBand3, block1ColorizeBrightBand3));
	shader3.setUniform3f("block1ColorizeBand4", ofVec3f(block1ColorizeHueBand4,
														block1ColorizeSaturationBand4, block1ColorizeBrightBand4));
	shader3.setUniform3f("block1ColorizeBand5", ofVec3f(block1ColorizeHueBand5,
														block1ColorizeSaturationBand5, block1ColorizeBrightBand5));

	// block1 filters
	shader3.setUniform1f("block1BlurAmount", block1BlurAmount);
	shader3.setUniform1f("block1BlurRadius", block1BlurRadius);
	shader3.setUniform1f("block1SharpenAmount", block1SharpenAmount);
	shader3.setUniform1f("block1SharpenRadius", block1SharpenRadius);
	shader3.setUniform1f("block1FiltersBoost", block1FiltersBoost);
	shader3.setUniform1f("block1Dither", block1Dither);
	bool block1DitherSwitch = 0;
	if (gui->block1Filters[5] > 0)
	{
		block1DitherSwitch = 1;
	}
	shader3.setUniform1i("block1DitherSwitch", block1DitherSwitch);

	// block2geo1
	shader3.setUniform2f("block2XYDisplace", ofVec2f(block2XDisplace, block2YDisplace));
	// remapping z
	float block2ZDisplaceMapped = block2ZDisplace;
	if (block2ZDisplaceMapped > 1.0)
	{
		block2ZDisplaceMapped = pow(2, (block2ZDisplaceMapped - 1.0f) * 8.0f);
		if (block2ZDisplace >= 2.0)
		{
			block2ZDisplaceMapped = 1000;
		}
	}
	shader3.setUniform1f("block2ZDisplace", block2ZDisplaceMapped);
	shader3.setUniform1f("block2Rotate", block2Rotate);
	shader3.setUniform4f("block2ShearMatrix", ofVec4f(block2ShearMatrix1,
													  block2ShearMatrix2, block2ShearMatrix3, block2ShearMatrix4));
	shader3.setUniform1f("block2KaleidoscopeAmount", block2KaleidoscopeAmount);
	shader3.setUniform1f("block2KaleidoscopeSlice", block2KaleidoscopeSlice);

	shader3.setUniform1i("block2HMirror", gui->block2HMirror);
	shader3.setUniform1i("block2VMirror", gui->block2VMirror);
	shader3.setUniform1i("block2HFlip", gui->block2HFlip);
	shader3.setUniform1i("block2VFlip", gui->block2VFlip);
	shader3.setUniform1i("block2RotateMode", gui->block2RotateMode);
	shader3.setUniform1i("block2GeoOverflow", gui->block2GeoOverflow);

	// block2 colorize
	shader3.setUniform1i("block2ColorizeSwitch", gui->block2ColorizeSwitch);
	shader3.setUniform1i("block2ColorizeHSB_RGB", gui->block2ColorizeHSB_RGB);

	shader3.setUniform3f("block2ColorizeBand1", ofVec3f(block2ColorizeHueBand1,
														block2ColorizeSaturationBand1, block2ColorizeBrightBand1));
	shader3.setUniform3f("block2ColorizeBand2", ofVec3f(block2ColorizeHueBand2,
														block2ColorizeSaturationBand2, block2ColorizeBrightBand2));
	shader3.setUniform3f("block2ColorizeBand3", ofVec3f(block2ColorizeHueBand3,
														block2ColorizeSaturationBand3, block2ColorizeBrightBand3));
	shader3.setUniform3f("block2ColorizeBand4", ofVec3f(block2ColorizeHueBand4,
														block2ColorizeSaturationBand4, block2ColorizeBrightBand4));
	shader3.setUniform3f("block2ColorizeBand5", ofVec3f(block2ColorizeHueBand5,
														block2ColorizeSaturationBand5, block2ColorizeBrightBand5));

	// block2 filters
	shader3.setUniform1f("block2BlurAmount", block2BlurAmount);
	shader3.setUniform1f("block2BlurRadius", block2BlurRadius);
	shader3.setUniform1f("block2SharpenAmount", block2SharpenAmount);
	shader3.setUniform1f("block2SharpenRadius", block2SharpenRadius);
	shader3.setUniform1f("block2FiltersBoost", block2FiltersBoost);
	shader3.setUniform1f("block2Dither", block2Dither);
	bool block2DitherSwitch = 0;
	if (gui->block2Filters[5] > 0)
	{
		block2DitherSwitch = 1;
	}
	shader3.setUniform1i("block2DitherSwitch", block2DitherSwitch);

	// final mix parameters
	shader3.setUniform1f("finalMixAmount", finalMixAmount);
	shader3.setUniform3f("finalKeyValue", ofVec3f(finalKeyValueRed, finalKeyValueGreen, finalKeyValueBlue));
	shader3.setUniform1f("finalKeyThreshold", finalKeyThreshold);
	shader3.setUniform1f("finalKeySoft", finalKeySoft);
	shader3.setUniform1i("finalKeyOrder", gui->finalKeyOrder);
	shader3.setUniform1i("finalMixType", gui->finalMixType);
	shader3.setUniform1i("finalMixOverflow", gui->finalMixOverflow);

	// matrixMixer
	shader3.setUniform1i("matrixMixType", gui->matrixMixType);
	shader3.setUniform1i("matrixMixOverflow", gui->matrixMixOverflow);
	shader3.setUniform3f("bgRGBIntoFgRed", ofVec3f(matrixMixBgRedIntoFgRed,
												   matrixMixBgGreenIntoFgRed,
												   matrixMixBgBlueIntoFgRed));
	shader3.setUniform3f("bgRGBIntoFgGreen", ofVec3f(matrixMixBgRedIntoFgGreen,
													 matrixMixBgGreenIntoFgGreen,
													 matrixMixBgBlueIntoFgGreen));
	shader3.setUniform3f("bgRGBIntoFgBlue", ofVec3f(matrixMixBgRedIntoFgBlue,
													matrixMixBgGreenIntoFgBlue,
													matrixMixBgBlueIntoFgBlue));

	/*
				//block2 colorize
	shader3.setUniform1i("block2ColorizeSwitch",gui->block2ColorizeSwitch);
	shader3.setUniform1i("block2ColorizeHSB_RGB",gui->block2ColorizeHSB_RGB);
	shader3.setUniform3f("block2ColorizeBand1",ofVec3f(block2ColorizeHueBand1,
				block2ColorizeSaturationBand1,block2ColorizeBrightBand1));
	shader3.setUniform3f("block2ColorizeBand2",ofVec3f(block2ColorizeHueBand2,
				block2ColorizeSaturationBand2,block2ColorizeBrightBand2));
	shader3.setUniform3f("block2ColorizeBand3",ofVec3f(block2ColorizeHueBand3,
				block2ColorizeSaturationBand3,block2ColorizeBrightBand3));
	shader3.setUniform3f("block2ColorizeBand4",ofVec3f(block2ColorizeHueBand4,
				block2ColorizeSaturationBand4,block2ColorizeBrightBand4));
	shader3.setUniform3f("block2ColorizeBand5",ofVec3f(block2ColorizeHueBand5,
				block2ColorizeSaturationBand5,block2ColorizeBrightBand5));

	*/

	shader3.end();
	framebuffer3.end();

	// draw to screen
	if (gui->drawMode == 0)
	{
		framebuffer1.draw(0, 0);
	}
	if (gui->drawMode == 1)
	{
		framebuffer2.draw(0, 0);
	}
	if (gui->drawMode == 2)
	{
		framebuffer3.draw(0, 0);
	}
	if (gui->drawMode == 3)
	{
		framebuffer1.draw(0, 0, ofGetWidth() / 2, ofGetHeight() / 2);
		framebuffer2.draw(ofGetWidth() / 2, 0, ofGetWidth() / 2, ofGetHeight() / 2);
		framebuffer3.draw(0, ofGetHeight() / 2, ofGetWidth() / 2, ofGetHeight() / 2);
	}

	pastFrames1[abs(pastFramesSize - pastFramesOffset) - 1].begin();
	framebuffer1.draw(0, 0);
	pastFrames1[abs(pastFramesSize - pastFramesOffset) - 1].end();

	pastFrames2[abs(pastFramesSize - pastFramesOffset) - 1].begin();
	framebuffer2.draw(0, 0);
	// ofSetColor(255);
	// ofDrawRectangle(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()/4,ofGetHeight()/4);
	pastFrames2[abs(pastFramesSize - pastFramesOffset) - 1].end();

	pastFramesOffset++;
	pastFramesOffset = pastFramesOffset % pastFramesSize;

	// inputTest();

	// clear the framebuffers
	framebuffer1.begin();
	ofClear(0, 0, 0, 255);
	framebuffer1.end();

	framebuffer2.begin();
	ofClear(0, 0, 0, 255);
	framebuffer2.end();

	framebuffer3.begin();
	ofClear(0, 0, 0, 255);
	framebuffer3.end();

	if (gui->fb1FramebufferClearSwitch == 1)
	{
		for (int i = 0; i < pastFramesSize; i++)
		{
			pastFrames1[i].begin();
			ofClear(0, 0, 0, 255);
			pastFrames1[i].end();
		}
	}

	if (gui->fb2FramebufferClearSwitch == 1)
	{
		for (int i = 0; i < pastFramesSize; i++)
		{
			pastFrames2[i].begin();
			ofClear(0, 0, 0, 255);
			pastFrames2[i].end();
		}
	}
}

//--------------------------------------------------------------
void ofApp::inputSetup()
{
	// i guess we might just be
	// stuck at sd inputs!
	// lets just get to work and
	// if we get the chance to fix this later
	// then do so
	input1.listDevices();
	input1.setVerbose(true);
	input1.setDeviceID(0);
	input1.setDesiredFrameRate(30);
	// input1.setPixelFormat(OF_PIXELS_NATIVE);
	input1.initGrabber(640, 480);

	input2.setDeviceID(1);
	input2.setDesiredFrameRate(30);
	input2.initGrabber(640, 480);
}

//--------------------------------------------------------------
void ofApp::inputUpdate()
{
	// inputUpdate
	input1.update();
	input2.update();
}

//--------------------------------------------------------------
void ofApp::inputTest()
{

	if (testSwitch1 == 1)
	{
		input1.draw(0, 0);
	}
	if (testSwitch1 == 2)
	{
		input2.draw(0, 0);
	}
}

//---------------------------------------------------------
void ofApp::framebufferSetup()
{
	framebuffer1.allocate(outputWidth, outputHeight);
	framebuffer2.allocate(outputWidth, outputHeight);
	framebuffer3.allocate(outputWidth, outputHeight);

	framebuffer1.begin();
	ofClear(0, 0, 0, 255);
	framebuffer1.end();

	framebuffer2.begin();
	ofClear(0, 0, 0, 255);
	framebuffer2.end();

	framebuffer3.begin();
	ofClear(0, 0, 0, 255);
	framebuffer3.end();

	for (int i = 0; i < pastFramesSize; i++)
	{
		pastFrames1[i].allocate(ofGetWidth(), ofGetHeight());
		pastFrames2[i].allocate(ofGetWidth(), ofGetHeight());

		pastFrames1[i].begin();
		ofClear(0, 0, 0, 255);
		pastFrames1[i].end();

		pastFrames2[i].begin();
		ofClear(0, 0, 0, 255);
		pastFrames2[i].end();
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '1')
	{
		testSwitch1 = 1;
	}
	if (key == '2')
	{
		testSwitch1 = 2;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}
//--------------------------
void ofApp::line_draw()
{
	ofVec3f linePosition1;
	ofVec3f linePosition2;

	line_theta += .01;
	line_phi += .013;
	line_eta += .0079;

	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2 + ofGetHeight() / 8.0f * (cos(line_theta)), ofGetHeight() / 2 + ofGetHeight() / 8.0f * (cos(line_eta)));
	linePosition1.x = ofGetWidth() / 16.0f;
	linePosition1.y = ofGetHeight() / 16.0f;
	linePosition1.z = 0;

	linePosition2.x = -ofGetWidth() / 16.0f;
	linePosition2.y = -ofGetHeight() / 16.0f;
	linePosition2.z = 0;

	ofRotateZRad(line_theta);
	ofRotateXRad(line_eta);
	ofRotateYRad(line_phi);

	ofSetColor(180.0f + 63.0f * (sin(line_eta)), 127.0f + 127.0f * (sin(line_theta)), 180.0f + 63.0f * (sin(line_phi)));

	ofDrawLine(linePosition1, linePosition2);
	ofPopMatrix();
}

//--------------------------
void ofApp::hypercube_draw()
{

	int limit = 3;
	for (int i = 0; i < limit; i++)
	{
		hypercube_theta += .1 * gui->hypercube_theta_rate;

		hypercube_phi += .1 * gui->hypercube_phi_rate;

		hypercube_r = ofGetWidth() / 32.0f * (gui->hypercube_size);

		float xr = hypercube_r * (1);
		hypercube_x[0] = xr * (cos(hypercube_theta) - sin(hypercube_theta)) * (1 - .5 * (cos(hypercube_phi)));
		hypercube_x[1] = xr * (cos(hypercube_theta) + sin(hypercube_theta)) * (1 - .5 * (cos(PI / 4 + hypercube_phi)));
		hypercube_x[2] = xr * (-cos(hypercube_theta) + sin(hypercube_theta)) * (1 - .5 * (cos(PI / 2 + hypercube_phi)));
		hypercube_x[3] = xr * (-cos(hypercube_theta) - sin(hypercube_theta)) * (1 - .5 * (cos(3 * PI / 4 + hypercube_phi)));
		hypercube_x[4] = xr * (cos(hypercube_theta) - sin(hypercube_theta)) * (1 - .5 * (cos(PI + hypercube_phi)));
		hypercube_x[5] = xr * (cos(hypercube_theta) + sin(hypercube_theta)) * (1 - .5 * (cos(5 * PI / 4 + hypercube_phi)));
		hypercube_x[6] = xr * (-cos(hypercube_theta) + sin(hypercube_theta)) * (1 - .5 * (cos(3 * PI / 2 + hypercube_phi)));
		hypercube_x[7] = xr * (-cos(hypercube_theta) - sin(hypercube_theta)) * (1 - .5 * (cos(7 * PI / 4 + hypercube_phi)));

		float yr = hypercube_r * (1);
		hypercube_y[0] = yr * (sin(hypercube_theta) + cos(hypercube_theta)) * (1 - .5 * (cos(hypercube_phi)));
		hypercube_y[1] = yr * (sin(hypercube_theta) - cos(hypercube_theta)) * (1 - .5 * (cos(PI / 4 + hypercube_phi)));
		hypercube_y[2] = yr * (-sin(hypercube_theta) - cos(hypercube_theta)) * (1 - .5 * (cos(PI / 2 + hypercube_phi)));
		hypercube_y[3] = yr * (-sin(hypercube_theta) + cos(hypercube_theta)) * (1 - .5 * (cos(3 * PI / 4 + hypercube_phi)));
		hypercube_y[4] = yr * (sin(hypercube_theta) + cos(hypercube_theta)) * (1 - .5 * (cos(PI + hypercube_phi)));
		hypercube_y[5] = yr * (sin(hypercube_theta) - cos(hypercube_theta)) * (1 - .5 * (cos(5 * PI / 4 + hypercube_phi)));
		hypercube_y[6] = yr * (-sin(hypercube_theta) - cos(hypercube_theta)) * (1 - .5 * (cos(3 * PI / 2 + hypercube_phi)));
		hypercube_y[7] = yr * (-sin(hypercube_theta) + cos(hypercube_theta)) * (1 - .5 * (cos(7 * PI / 4 + hypercube_phi)));

		float zr = hypercube_r * (1);
		hypercube_z[0] = -zr / 2 * cos(hypercube_phi) + hypercube_r;
		hypercube_z[1] = -zr / 2 * cos(PI / 4 + hypercube_phi) + hypercube_r;
		hypercube_z[2] = -zr / 2 * cos(PI / 2 + hypercube_phi) + hypercube_r;
		hypercube_z[3] = -zr / 2 * cos(3 * PI / 4 + hypercube_phi) + hypercube_r;
		hypercube_z[4] = -zr / 2 * cos(PI + hypercube_phi) + hypercube_r;
		hypercube_z[5] = -zr / 2 * cos(5 * PI / 4 + hypercube_phi) + hypercube_r;
		hypercube_z[6] = -zr / 2 * cos(3 * PI / 2 + hypercube_phi) + hypercube_r;
		hypercube_z[7] = -zr / 2 * cos(7 * PI / 8 + hypercube_phi) + hypercube_r;

		hypercube_color_theta += .01;
		ofSetColor(127 + 127 * sin(hypercube_color_theta), 0 + 192 * abs(cos(hypercube_color_theta * .2)), 127 + 127 * cos(hypercube_color_theta / 3.0f));
		ofNoFill();
		ofPushMatrix();
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		ofRotateYRad(-PI / 2);
		ofRotateZRad(hypercube_phi);
		ofRotateXRad(hypercube_theta / 3);
		ofRotateXRad(hypercube_theta / 5);

		// list up the vertexes, give them some kind of grouping and
		// set up a set of 3 rotatios for each
		// i think just pick like an inner cube and an outer cube
		// if thats even possible

		ofDrawLine(hypercube_x[0], hypercube_y[0], hypercube_z[0], hypercube_x[1], hypercube_y[1], hypercube_z[1]);
		ofDrawLine(hypercube_x[1], hypercube_y[1], hypercube_z[1], hypercube_x[2], hypercube_y[2], hypercube_z[2]);
		ofDrawLine(hypercube_x[2], hypercube_y[2], hypercube_z[2], hypercube_x[3], hypercube_y[3], hypercube_z[3]);
		ofDrawLine(hypercube_x[3], hypercube_y[3], hypercube_z[3], hypercube_x[4], hypercube_y[4], hypercube_z[4]);
		ofDrawLine(hypercube_x[4], hypercube_y[4], hypercube_z[4], hypercube_x[5], hypercube_y[5], hypercube_z[5]);
		ofDrawLine(hypercube_x[5], hypercube_y[5], hypercube_z[5], hypercube_x[6], hypercube_y[6], hypercube_z[6]);
		ofDrawLine(hypercube_x[6], hypercube_y[6], hypercube_z[6], hypercube_x[7], hypercube_y[7], hypercube_z[7]);
		ofDrawLine(hypercube_x[7], hypercube_y[7], hypercube_z[7], hypercube_x[0], hypercube_y[0], hypercube_z[0]);

		ofDrawLine(hypercube_x[0], hypercube_y[0], -hypercube_z[0], hypercube_x[1], hypercube_y[1], -hypercube_z[1]);
		ofDrawLine(hypercube_x[1], hypercube_y[1], -hypercube_z[1], hypercube_x[2], hypercube_y[2], -hypercube_z[2]);
		ofDrawLine(hypercube_x[2], hypercube_y[2], -hypercube_z[2], hypercube_x[3], hypercube_y[3], -hypercube_z[3]);
		ofDrawLine(hypercube_x[3], hypercube_y[3], -hypercube_z[3], hypercube_x[4], hypercube_y[4], -hypercube_z[4]);
		ofDrawLine(hypercube_x[4], hypercube_y[4], -hypercube_z[4], hypercube_x[5], hypercube_y[5], -hypercube_z[5]);
		ofDrawLine(hypercube_x[5], hypercube_y[5], -hypercube_z[5], hypercube_x[6], hypercube_y[6], -hypercube_z[6]);
		ofDrawLine(hypercube_x[6], hypercube_y[6], -hypercube_z[6], hypercube_x[7], hypercube_y[7], -hypercube_z[7]);
		ofDrawLine(hypercube_x[7], hypercube_y[7], -hypercube_z[7], hypercube_x[0], hypercube_y[0], -hypercube_z[0]);

		ofDrawLine(hypercube_x[0], hypercube_y[0], hypercube_z[0], hypercube_x[0], hypercube_y[0], -hypercube_z[0]);
		ofDrawLine(hypercube_x[1], hypercube_y[1], hypercube_z[1], hypercube_x[1], hypercube_y[1], -hypercube_z[1]);
		ofDrawLine(hypercube_x[2], hypercube_y[2], hypercube_z[2], hypercube_x[2], hypercube_y[2], -hypercube_z[2]);
		ofDrawLine(hypercube_x[3], hypercube_y[3], hypercube_z[3], hypercube_x[3], hypercube_y[3], -hypercube_z[3]);
		ofDrawLine(hypercube_x[4], hypercube_y[4], hypercube_z[4], hypercube_x[4], hypercube_y[4], -hypercube_z[4]);
		ofDrawLine(hypercube_x[5], hypercube_y[5], hypercube_z[5], hypercube_x[5], hypercube_y[5], -hypercube_z[5]);
		ofDrawLine(hypercube_x[6], hypercube_y[6], hypercube_z[6], hypercube_x[6], hypercube_y[6], -hypercube_z[6]);
		ofDrawLine(hypercube_x[7], hypercube_y[7], hypercube_z[7], hypercube_x[7], hypercube_y[7], -hypercube_z[7]);

		ofDrawLine(hypercube_x[0], hypercube_y[0], -hypercube_z[0], hypercube_x[4], hypercube_y[4], -hypercube_z[4]);
		ofDrawLine(hypercube_x[1], hypercube_y[1], -hypercube_z[1], hypercube_x[5], hypercube_y[5], -hypercube_z[5]);
		ofDrawLine(hypercube_x[2], hypercube_y[2], -hypercube_z[2], hypercube_x[6], hypercube_y[6], -hypercube_z[6]);
		ofDrawLine(hypercube_x[3], hypercube_y[3], -hypercube_z[3], hypercube_x[7], hypercube_y[7], -hypercube_z[7]);

		ofDrawLine(hypercube_x[0], hypercube_y[0], hypercube_z[0], hypercube_x[4], hypercube_y[4], hypercube_z[4]);
		ofDrawLine(hypercube_x[1], hypercube_y[1], hypercube_z[1], hypercube_x[5], hypercube_y[5], hypercube_z[5]);
		ofDrawLine(hypercube_x[2], hypercube_y[2], hypercube_z[2], hypercube_x[6], hypercube_y[6], hypercube_z[6]);
		ofDrawLine(hypercube_x[3], hypercube_y[3], hypercube_z[3], hypercube_x[7], hypercube_y[7], hypercube_z[7]);

		ofPopMatrix();

	} // endifor
}

//---------------------------------------------------------
void ofApp::sevenStar1Setup()
{
	// how to auto generate coordinates for n division of circumference of unit circle
	for (int i = 0; i < reps; i++)
	{
		float theta = float(i) * 2.0f * PI / float(reps);
		points[i].set(cos(theta), sin(theta));
	}
	int starIncrementer = 0;
	for (int i = 0; i < reps1; i++)
	{
		points1[i].set(ofGetHeight() / 4.0f * points[starIncrementer]);
		starIncrementer = (starIncrementer + 3) % reps;
	}
	// second odd star styling
	starIncrementer = 0;
	for (int i = 0; i < reps; i++)
	{
		points2[i].set(ofGetHeight() / 4.0f * points[starIncrementer]);
		starIncrementer = (starIncrementer + 2) % reps;
	}
	position1 = points1[0];
	position2 = points2[0];
}
//------------------------------------------------------
void ofApp::sevenStar1Draw()
{

	thetaHue1 += hueInc1;
	thetaHue2 += hueInc2;
	thetaSaturation1 += saturationInc1;
	thetaChaos += .000125 * (thetaHue1 * (sin(thetaHue2 * .00001)) - thetaHue2 * (sin(thetaHue1 * .00001)));

	float squareSize = ofGetWidth() / 64;

	ofPushMatrix();

	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	position1.x = ofLerp(position1.x, points1[index1].x, increment1);
	position1.y = ofLerp(position1.y, points1[index1].y, increment1);
	// this is kinda neat in that it seems to create a bit of a sense of depth
	// could be pretty cool to have for odd stars 2 or more different paths like this that are moving in different orders over one another??
	float shapedX = position1.x;
	float shapedY = position1.y;
	// probably cool thing to do here with 'shaping' would be to work with beziers, arcs, splines etc
	// and then lerp between the two different paths with some kind of oscillator

	ofColor hsbC1;
	hsbC1.setHsb(127.0f + 63.0f * sin(thetaHue1 + thetaChaos) + 63.0f * cos(thetaHue2 - thetaChaos), 190.0f + 63.0f * cos(thetaSaturation1), 255);
	ofSetColor(hsbC1);
	ofDrawEllipse(shapedX, shapedY, squareSize, squareSize);
	hsbC1.setHsb(255.0f - (127.0f + 63.0f * sin(thetaHue1 + thetaChaos) + 63.0f * cos(thetaHue2 - thetaChaos)), 190.0f + 63.0f * cos(thetaSaturation1), 200);
	ofSetColor(hsbC1);
	ofDrawEllipse(shapedX, shapedY, squareSize - 2, squareSize - 2);

	if (position1 != points1[index1])
	{
		increment1 += acceleration1;
	}
	if (position1.distance(points1[index1]) < threshold)
	{
		index1++;
		index1 = index1 % reps1;
		increment1 = 0;
	}

	position2.x = ofLerp(position2.x, points2[index2].x, increment2);
	position2.y = ofLerp(position2.y, points2[index2].y, increment2);

	hsbC1.setHsb(127.0f + 63.0f * sin(thetaHue2 + thetaChaos) - 63.0f * cos(thetaHue1 - thetaChaos), 190.0f + 63.0f * cos(thetaSaturation1 - thetaHue1), 255);
	ofSetColor(hsbC1);
	ofDrawEllipse(position2.x, position2.y, squareSize, squareSize);
	hsbC1.setHsb(255.0f - (127.0f + 63.0f * sin(thetaHue2 + thetaChaos) - 63.0f * cos(thetaHue1 - thetaChaos)), 190.0f + 63.0f * cos(thetaSaturation1 - thetaHue1), 200);
	ofSetColor(hsbC1);
	ofDrawEllipse(position2.x, position2.y, squareSize - 2, squareSize - 2);

	if (position2 != points2[index2])
	{
		increment2 += acceleration2;
	}
	if (position2.distance(points2[index2]) < threshold)
	{
		index2++;
		index2 = index2 % reps;
		increment2 = 0;
	}

	ofPopMatrix();
}
// -------------------------------------------------------------- -
void ofApp::drawSpiralEllipse()
{
	spiralTheta1 += spiralTheta1Inc;
	spiralRadius1 += radius1Inc;

	spiralTheta2 += spiralTheta2Inc;
	spiralRadius2 += radius2Inc;

	spiralTheta3 -= spiralTheta3Inc;
	spiralRadius3 += radius3Inc;

	float x1 = spiralRadius1 * .5 * (sin(spiralTheta1 - .001 * sin(.01 * spiralTheta2)) + cos(spiralTheta3));
	float y1 = spiralRadius1 * .5 * (cos(spiralTheta1 - .001 * cos(.01 * spiralTheta3) + sin(spiralTheta3)));

	float x2 = spiralRadius2 * sin(spiralTheta2);
	float y2 = spiralRadius2 * cos(spiralTheta2);

	float x3 = spiralRadius3 * sin(spiralTheta3);
	float y3 = spiralRadius3 * cos(spiralTheta3);

	float size = (ofGetHeight() / 64) + 22 * abs((x1 + y1) / ((ofGetWidth() / 2 + ofGetHeight() / 2)));

	ofPushMatrix();
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
	/*
	ofSetColor(190 + 63 * sin(spiralTheta3Inc - .00001*y2), 127 + 127 * cos(spiralTheta2Inc + .00001*x3), 190 + 63 * sin(spiralTheta1Inc - .00001*y1), 255);
	ofDrawEllipse(x1, y1, size, size);
	ofSetColor(190 + 63 * sin(spiralTheta1Inc - .00001*x3), 127 + 127 * cos(spiralTheta3Inc + .00001*x2), 190 + 63 * cos(spiralTheta2Inc - .00001*y1), 255);
	ofDrawEllipse(x2, y2, size, size);
	ofSetColor(190 + 63 * sin(spiralTheta2Inc - .00001*y1), 127 + 127 * cos(spiralTheta2Inc + .00001*x1), 190 + 63 * sin(spiralTheta1Inc - .00001*y3), 255);
	ofDrawEllipse(x3, y3, size, size);
	*/

	ofSetColor(190 + 63 * sin(spiralTheta3 - .00001 * y2), 127 + 127 * cos(spiralTheta2 + .00001 * x3), 190 + 63 * sin(spiralTheta1 - .00001 * y1), 255);
	ofDrawEllipse(x1, y1, size, size);
	ofSetColor(255 - (63 + 190 * sin(spiralTheta3 - .00001 * y2)), 255 - (127 + 127 * cos(spiralTheta2 + .00001 * x3)), 255 - (63 + 190 * sin(spiralTheta1 - .00001 * y1)), 255);
	ofDrawEllipse(x1, y1, size - 2, size - 2);

	/*
	ofSetColor(190 + 63 * sin(.1*spiralTheta1 - .00001*x3), 127 + 127 * cos(.1*spiralTheta3 + .00001*x2), 190 + 63 * cos(.1*spiralTheta2 - .00001*y1), 255);
	ofDrawEllipse(x2, y2, size, size);
	ofSetColor(255 - (190 + 63 * sin(.1*spiralTheta1 - .00001*x3)), 255 - (127 + 127 * cos(.1*spiralTheta3 + .00001*x2)), 255 - (190 + 63 * cos(.1*spiralTheta2 - .00001*y1)), 255);
	ofDrawEllipse(x2, y2, size - 2, size - 2);

	ofSetColor(190 + 63 * sin(.1*spiralTheta2 - .00001*y1), 127 + 127 * cos(.1*spiralTheta2 + .00001*x1), 190 + 63 * sin(.1*spiralTheta1 - .00001*y3), 255);
	ofDrawEllipse(x3, y3, size, size);
	ofSetColor(255 - (190 + 63 * sin(.1*spiralTheta2 - .00001*y1)), 255 - (127 + 127 * cos(.1*spiralTheta2 + .00001*x1)), 255 - (190 + 63 * sin(.1*spiralTheta1 - .00001*y3)), 255);
	ofDrawEllipse(x3, y3, size - 2, size - 2);
	*/
	ofPopMatrix();

	spiralTheta1 = fmod(spiralTheta1, TWO_PI);
	spiralTheta2 = fmod(spiralTheta2, TWO_PI);
	spiralTheta3 = fmod(spiralTheta3, TWO_PI);
	if (x1 + size > ofGetWidth() / 2 || y1 + size > ofGetHeight() / 2)
	{
		spiralRadius1 = 0;
		radius1Inc = .99 * ofRandomf();
		spiralTheta1Inc = .08 * ofRandomf();
	}

	if (x2 + size > ofGetWidth() / 2 || y2 + size > ofGetHeight() / 2)
	{
		spiralRadius2 = 0;
		radius2Inc = .8 * ofRandomf();
		spiralTheta2Inc = .09 * ofRandomf();
	}

	if (x3 + size > ofGetWidth() / 2 || y3 + size > ofGetHeight() / 2)
	{
		spiralRadius3 = 0;
		radius3Inc = .9 * ofRandomf();
		spiralTheta3Inc = .13 * ofRandomf();
	}
}
