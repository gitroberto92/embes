/** 
  
  Copyright (c) 2012 PSJDC.
  
  @file pa.h
  
  @date 2012-8-7Ŭ����  
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   �ܼ��� PortAudio�� ������ ����� ����/��±��
*/

#pragma once

#include "PortAudio.h"

// PortAudio �ݹ��Լ�Ÿ��
/**
  @breif PortAudio �ݹ��Լ�Ÿ��
  
  @param input - ����ũ�� ������ �Է��ڷ�(���׷����� ��� L/R/L/R... �� ���� ����.)
  @param output - ����Ŀ�� ����� �ڷ�. �ݹ��Լ����� ��� �Ѵ�(���׷����� ��� L/R/L/R... �� ���� ������.)
  @param frameCount - �������� ǥ��������
  @param timeInfo - ...
  @param statusFlags - ...
  @param userData - �����Լ��� �Ѱ��� ����� �ڷ�
  @return 0�� ��ȯ.
  @warning 
*/
typedef int FN_PaStreamCallback( 
	const void *input,
	void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData ) ;

// �ݹ��Լ� ���Ƿ�
// typedef struct
// {
//     float left_phase;
//     float right_phase;
// }   
// paTestData;
// 
// * This routine will be called by the PortAudio engine when audio is needed.
//  It may called at interrupt level on some machines so don't do anything
//  that could mess up the system like calling malloc() or free().
// */ 
// static int patestCallback( const void *inputBuffer, void *outputBuffer,
//                            unsigned long framesPerBuffer,
//                            const PaStreamCallbackTimeInfo* timeInfo,
//                            PaStreamCallbackFlags statusFlags,
//                            void *userData )
// {
//     /* Cast data passed through stream to our structure. */
//     paTestData *data = (paTestData*)userData; 
//     float *out = (float*)outputBuffer;
//     unsigned int i;
//     (void) inputBuffer; /* Prevent unused variable warning. */
//     
//     for( i=0; i<framesPerBuffer; i++ )
//     {
//         *out++ = data->left_phase;  /* left */
//         *out++ = data->right_phase;  /* right */
//         /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
//         data->left_phase += 0.01f;
//         /* When signal reaches top, drop back down. */
//         if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
//         /* higher pitch so we can distinguish left and right. */
//         data->right_phase += 0.03f;
//         if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
//     }
//     return 0;
// }


/**
  @breif PortAudio�� �����Ͽ� �Ͻ����� �������ġ�� �����Ѵ�.
  
  �ݹ��Լ��� �Ѿ���� ������ Ÿ���� flaot �̴�.
  
  @param p_callbackFunc - �������Ʈ�� �ݹ��Լ�.
  @param p_UserData - ����� �ڷ�.(��Ʈ�� �ݹ��Լ��� �Ѿ�´�)
  @param p_SampleRate - ǥ��ȭ���ļ�
  @param p_FrameBufSize - �ݹ��Լ��� �Ѿ���� ��/��� ��Ʈ�� ��������
  @param p_SampleFormat - �� �������� ������ Ÿ��.(paFloat32/paInt32/paInt24/paInt16/paInt8/paUInt8/paCustomFormat)
  @param p_inChannels - �Է�ä�μ�
  @param p_outChannels - ���ä�μ�
  @return ���� - PortAudio ��Ʈ����ü, ���� - NULL
  @warning 
*/
static PaStream* PA_OpenDefAudioStream(FN_PaStreamCallback p_callbackFunc, void *p_UserData, double p_SampleRate, unsigned long p_FrameBufSize, PaSampleFormat p_SampleForamt = paFloat32, int p_inChannels = 1, int p_outChannels = 2)
{
	PaStreamParameters inputParameters, outputParameters;
	PaStream *stream;
	PaError err;

	err = Pa_Initialize();
	if( err != paNoError ) goto error;

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default input device.\n");
		goto error;
	}
	inputParameters.channelCount = p_inChannels;
	inputParameters.sampleFormat = p_SampleForamt;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		goto error;
	}
	outputParameters.channelCount = p_outChannels;
	outputParameters.sampleFormat = p_SampleForamt;
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		&outputParameters,
		p_SampleRate,
		p_FrameBufSize,
		0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
		p_callbackFunc,
		p_UserData );
	if( err != paNoError ) goto error;

	err = Pa_StartStream( stream );
	if( err != paNoError ) goto error;

	return stream;

	printf("Hit ENTER to stop program.\n");
	getchar();
	err = Pa_CloseStream( stream );
	if( err != paNoError ) goto error;

	Pa_Terminate();
	return 0;

error:
	Pa_Terminate();
	fprintf( stderr, "An error occured while using the portaudio stream\n" );
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return NULL;
}

/**
  @breif PortAudio ��Ʈ����ü�� �ݴ´�.
  
  @param p_pStream - PortAudio ��Ʈ����ü
  @warning 
*/
static void PA_CloseAudioStream(PaStream* p_pStream)
{
	if(p_pStream)
	{
		Pa_CloseStream( p_pStream );
		Pa_Terminate();
	}
}
