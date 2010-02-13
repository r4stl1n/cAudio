// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio.h

#ifndef IEFFECTPARAMETERS_H
#define IEFFECTPARAMETERS_H

#include "cVector3.h"
#include "cAudioDefines.h"

#ifdef CAUDIO_EFX_ENABLED

namespace cAudio
{
	struct sEAXReverbParameters
	{
		sEAXReverbParameters(
			float density = 1.0f,
			float diffusion = 1.0f, 
			float gain = 0.32f,
			float gainHF = 0.89f, 
			float gainLF = 0.0f, 
			float decayTime = 1.49f, 
			float decayHFRatio = 0.83f, 
			float decayLFRatio = 1.0f, 
			float reflectionsGain = 0.05f, 
			float reflectionsDelay = 0.007f, 
			cVector3 reflectionsPan = cVector3(0.0f, 0.0f, 0.0f), 
			float lateReverbGain = 1.26f, 
			float lateReverbDelay = 0.011f, 
			cVector3 lateReverbPan = cVector3(0.0f, 0.0f, 0.0f), 
			float echoTime = 0.25f, 
			float echoDepth = 0.0f, 
			float modulationTime = 0.25f, 
			float modulationDepth = 0.0f, 
			float airAbsorptionGainHF = 0.994f, 
			float hFReference = 5000.0f, 
			float lFReference = 250.0f, 
			float roomRolloffFactor = 0.0f, 
			bool decayHFLimit = true) : 
			Density(density), Diffusion(diffusion), Gain(gain), GainHF(gainHF), GainLF(gainLF),
			DecayTime(decayTime), DecayHFRatio(decayHFRatio), DecayLFRatio(decayLFRatio), 
			ReflectionsGain(reflectionsGain), ReflectionsDelay(reflectionsDelay),
			ReflectionsPan(reflectionsPan), LateReverbGain(lateReverbGain),
			LateReverbDelay(lateReverbDelay), LateReverbPan(lateReverbPan),
			EchoTime(echoTime), EchoDepth(echoDepth),ModulationTime(modulationTime), 
			ModulationDepth(modulationDepth), AirAbsorptionGainHF(airAbsorptionGainHF), 
			HFReference(hFReference), LFReference(lFReference), 
			RoomRolloffFactor(roomRolloffFactor), DecayHFLimit(decayHFLimit) { }

		float Density;
		float Diffusion;
		float Gain;
		float GainHF;
		float GainLF;
		float DecayTime;
		float DecayHFRatio;
		float DecayLFRatio;
		float ReflectionsGain;
		float ReflectionsDelay;
		cVector3 ReflectionsPan;
		float LateReverbGain;
		float LateReverbDelay;
		cVector3 LateReverbPan;
		float EchoTime;
		float EchoDepth;
		float ModulationTime;
		float ModulationDepth;
		float AirAbsorptionGainHF;
		float HFReference;
		float LFReference;
		float RoomRolloffFactor;
		bool DecayHFLimit;
	};

	struct sReverbParameters
	{
		sReverbParameters(
			float density = 1.0f,
			float diffusion = 1.0f, 
			float gain = 0.32f,
			float gainHF = 0.89f, 
			float decayTime = 1.49f, 
			float decayHFRatio = 0.83f, 
			float reflectionsGain = 0.05f, 
			float reflectionsDelay = 0.007f, 
			float lateReverbGain = 1.26f, 
			float lateReverbDelay = 0.011f, 
			float airAbsorptionGainHF = 0.994f, 
			float roomRolloffFactor = 0.0f, 
			bool decayHFLimit = true) : 
			Density(density), Diffusion(diffusion), Gain(gain), GainHF(gainHF),
			DecayTime(decayTime), DecayHFRatio(decayHFRatio), 
			ReflectionsGain(reflectionsGain), ReflectionsDelay(reflectionsDelay),
			LateReverbGain(lateReverbGain), LateReverbDelay(lateReverbDelay),
			AirAbsorptionGainHF(airAbsorptionGainHF), RoomRolloffFactor(roomRolloffFactor), 
			DecayHFLimit(decayHFLimit) { }

		float Density;
		float Diffusion;
		float Gain;
		float GainHF;
		float DecayTime;
		float DecayHFRatio;
		float ReflectionsGain;
		float ReflectionsDelay;
		float LateReverbGain;
		float LateReverbDelay;
		float AirAbsorptionGainHF;
		float RoomRolloffFactor;
		bool DecayHFLimit;
	};

	struct sChorusParameters
	{
		enum ChorusWaveform
		{
			ECW_SINUSOID,
			ECW_TRIANGLE,
			ECW_COUNT
		};
		sChorusParameters(
			ChorusWaveform waveform = ECW_TRIANGLE,
			int phase = 90,
			float rate = 1.1f, 
			float depth = 0.1f,
			float feedback = 0.25f, 
			float delay = 0.016f) : 
			Waveform(waveform), Phase(phase), Rate(rate), Depth(depth), Feedback(feedback), 
			Delay(delay) { }

		ChorusWaveform Waveform;
		int Phase;
		float Rate;
		float Depth;
		float Feedback;
		float Delay;
	};

	struct sDistortionParameters
	{
		sDistortionParameters(
			float edge = 0.2f,
			float gain = 0.05f, 
			float lowpassCutoff = 8000.0f,
			float eqCenter = 3600.0f, 
			float eqBandwidth = 3600.0f) : 
			Edge(edge), Gain(gain), LowpassCutoff(lowpassCutoff), EqCenter(eqCenter), 
			EqBandwidth(eqBandwidth) { }

		float Edge;
		float Gain;
		float LowpassCutoff;
		float EqCenter;
		float EqBandwidth;
	};

	struct sEchoParameters
	{
		sEchoParameters(
			float delay = 0.1f,
			float lRDelay = 0.1f, 
			float damping = 0.5f,
			float feedback = 0.5f, 
			float spread = -1.0f) : 
			Delay(delay), LRDelay(lRDelay), Damping(damping), Feedback(feedback), 
			Spread(spread) { }

		float Delay;
		float LRDelay;
		float Damping;
		float Feedback;
		float Spread;
	};

	struct sFlangerParameters
	{
		enum FlangerWaveform
		{
			EFW_SINUSOID,
			EFW_TRIANGLE,
			EFW_COUNT
		};
		sFlangerParameters(
			FlangerWaveform waveform = EFW_TRIANGLE,
			int phase = 0,
			float rate = 0.27f, 
			float depth = 1.0f,
			float feedback = -0.5f, 
			float delay = 0.002f) : 
			Waveform(waveform), Phase(phase), Rate(rate), Depth(depth), Feedback(feedback), 
			Delay(delay) { }

		FlangerWaveform Waveform;
		int Phase;
		float Rate;
		float Depth;
		float Feedback;
		float Delay;
	};

	struct sFrequencyShiftParameters
	{
		enum ShiftDirection
		{
			ESD_DOWN,
			ESD_UP,
			ESD_OFF,
			ESD_COUNT
		};
		sFrequencyShiftParameters(
			float frequency = 0.0f,
			ShiftDirection left = ESD_DOWN,
			ShiftDirection right = ESD_DOWN) : 
			Frequency(frequency), Left(left), Right(right) { }

		float Frequency;
		ShiftDirection Left;
		ShiftDirection Right;
	};

	struct sVocalMorpherParameters
	{
		enum MorpherPhoneme
		{
			EMP_A,
			EMP_E,
			EMP_I,
			EMP_O,
			EMP_U,
			EMP_AA,
			EMP_AE,
			EMP_AH,
			EMP_AO,
			EMP_EH,
			EMP_ER,
			EMP_IH,
			EMP_IY,
			EMP_UH,
			EMP_UW,
			EMP_B,
			EMP_D,
			EMP_F,
			EMP_G,
			EMP_J,
			EMP_K,
			EMP_L,
			EMP_M,
			EMP_N,
			EMP_P,
			EMP_R,
			EMP_S,
			EMP_T,
			EMP_V,
			EMP_Z,
			ESD_COUNT
		};

		enum MorpherWaveform
		{
			EMW_SINUSOID,
			EMW_TRIANGLE,
			EMW_SAW,
			EMW_COUNT
		};
		sVocalMorpherParameters(
			MorpherPhoneme phonemeA = EMP_A,
			MorpherPhoneme phonemeB = EMP_ER,
			int phonemeACoarseTune = 0,
			int phonemeBCoarseTune = 0,
			MorpherWaveform waveform = EMW_SINUSOID,
			float rate = 1.41f) : 
			PhonemeA(phonemeA), PhonemeB(phonemeB), PhonemeACoarseTune(phonemeACoarseTune),
			PhonemeBCoarseTune(phonemeBCoarseTune), Waveform(waveform), Rate(rate) { }

		MorpherPhoneme PhonemeA;
		MorpherPhoneme PhonemeB;
		int PhonemeACoarseTune;
		int PhonemeBCoarseTune;
		MorpherWaveform Waveform;
		float Rate;
	};

	struct sPitchShifterParameters
	{
		sPitchShifterParameters(
			int coarseTune = 12,
			int fineTune = 0) : 
			CoarseTune(coarseTune), FineTune(fineTune) { }

		int CoarseTune;
		int FineTune;
	};

	struct sRingModulatorParameters
	{
		enum ModulatorWaveform
		{
			EMW_SINUSOID,
			EMW_SAW,
			EMW_SQUARE,
			EMW_COUNT
		};
		sRingModulatorParameters(
			float frequency = 440.0f,
			float highPassCutoff = 800.0f,
			ModulatorWaveform waveform = EMW_SINUSOID) : 
			Frequency(frequency), HighPassCutoff(highPassCutoff), Waveform(waveform) { }

		float Frequency;
		float HighPassCutoff;
		ModulatorWaveform Waveform;
	};

	struct sAutowahParameters
	{
		sAutowahParameters(
			float attackTime = 0.06f,
			float releaseTime = 0.06f, 
			float resonance = 1000.0f,
			float peakGain = 11.22f) : 
			AttackTime(attackTime), ReleaseTime(releaseTime), Resonance(resonance), 
			PeakGain(peakGain) { }

		float AttackTime;
		float ReleaseTime;
		float Resonance;
		float PeakGain;
	};

	struct sCompressorParameters
	{
		sCompressorParameters(
			bool active = true) : 
			Active(active) { }

		bool Active;
	};

	struct sEqualizerParameters
	{
		sEqualizerParameters(
			float lowGain = 1.0f,
			float lowCutoff = 200.0f,
			float mid1Gain = 1.0f,
			float mid1Center = 500.0f,
			float mid1Width = 1.0f,
			float mid2Gain = 1.0f,
			float mid2Center = 3000.0f,
			float mid2Width = 1.0f,
			float highGain = 1.0f,
			float highCutoff = 6000.0f) : 
			LowGain(lowGain), LowCutoff(lowCutoff), Mid1Gain(mid1Gain), 
			Mid1Center(mid1Center), Mid1Width(mid1Width), Mid2Gain(mid2Gain), 
			Mid2Center(mid2Center), Mid2Width(mid2Width), HighGain(highGain),
			HighCutoff(highCutoff) { }

		float LowGain;
		float LowCutoff;
		float Mid1Gain;
		float Mid1Center;
		float Mid1Width;
		float Mid2Gain;
		float Mid2Center;
		float Mid2Width;
		float HighGain;
		float HighCutoff;
	};
};

#endif

#endif //! IEFFECTPARAMETERS_H