/** 
  
  Copyright (c) 2012 PSJDC.
  
  @file pa.h
  
  @date 2012-8-7클래스  
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   단순한 PortAudio를 리용한 오디오 록음/출력기능
*/

#pragma once

#include "PortAudio.h"

// PortAudio 콜백함수타입
/**
  @breif PortAudio 콜백함수타입
  
  @param input - 마이크로 들어오는 입력자료(스테레오의 경우 L/R/L/R... 와 같은 순서.)
  @param output - 스피커로 출력할 자료. 콜백함수에서 출력 한다(스테레오의 경우 L/R/L/R... 와 같은 순서로.)
  @param frameCount - 프레임의 표본점개수
  @param timeInfo - ...
  @param statusFlags - ...
  @param userData - 오픈함수로 넘겨준 사용자 자료
  @return 0을 귀환.
  @warning 
*/
typedef int FN_PaStreamCallback( 
	const void *input,
	void *output,
	unsigned long frameCount,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData ) ;

// 콜백함수 사용실례
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
  @breif PortAudio를 리용하여 암시적인 오디오장치를 오픈한다.
  
  콜백함수로 넘어오는 데이터 타입은 flaot 이다.
  
  @param p_callbackFunc - 오디오스트림 콜백함수.
  @param p_UserData - 사용자 자료.(스트림 콜백함수시 넘어온다)
  @param p_SampleRate - 표본화주파수
  @param p_FrameBufSize - 콜백함수로 넘어오는 입/출력 스트림 샘플점수
  @param p_SampleFormat - 매 샘플점의 데이터 타입.(paFloat32/paInt32/paInt24/paInt16/paInt8/paUInt8/paCustomFormat)
  @param p_inChannels - 입력채널수
  @param p_outChannels - 출력채널수
  @return 성공 - PortAudio 스트림객체, 실패 - NULL
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
  @breif PortAudio 스트림객체를 닫는다.
  
  @param p_pStream - PortAudio 스트림객체
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
