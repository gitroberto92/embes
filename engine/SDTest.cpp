#include "engine_main.h"
#include "WaveFile.h"
#include "SDFileIO.h"
#include <string.h>
#include "SDTest.h"

struct  Event
{
	int time;
	int key;
	KeyEvent event;
	Event* next;
	Event(int time, int key, KeyEvent event): time(time), key(key), event(event), next(NULL){}
};

#define WRITE_BUFF_SIZE 1

short writeBuff[AUDIO_FRAME_LEN * WRITE_BUFF_SIZE * AUDIO_CHANNELS];

Event* top;
Event* last;

LPFile input;

void addEvent(Event* event) {
	last->next = event;
	last = event;
}

int getint() {
	char c;
	int ret = 0;
	while ((c = _getchar_(input)) > 0) {
		if (c >= '0' && c <= '9')
			ret = ret * 10 + (c - '0');
		else
			break;
	}
	if (c == 0)
		return -1;
	else
		return ret;
}

Event* readEvent() {
	int time = getint();
	int key = getint();
	int state = getint();
	if (key < 0)
		return NULL;
	if (time < 0)
		return NULL;
	if (state < 0)
		return NULL;

	KeyEvent event;
	if (state != 0)
		event = NOTE_OFF;
	else
		event = NOTE_ON;

	return new Event(time, key, event);
}

void readEvents() {
	for(int i = 0; i < 20; i++){
		Event* ev = readEvent();
		if (ev == NULL)
			break;
		addEvent(ev);
	}	
}

void SDTestStart()
{
#ifdef __DEBUG
	printf("SD Test Start\r\n");
#endif
	WaveFileWriter writer;
	
	writer.Create("sd_rec.wav", AUDIO_SAMPLING_RATE, 16, AUDIO_CHANNELS);
	
	int time = 0;

	top = new Event(0, -1, NOTE_ON);
	last = top;
	input =  _fopen_("input.dat", FT_READ);

	readEvents();

	main_init();

	int currentBuffPos = 0;

	while (top != NULL) {
		while (top != NULL && time > top->time) {
			raiseKeyEvent(top->key, top->event);
			top = top->next;
		}

		main_loop();

		memcpy(writeBuff + AUDIO_FRAME_LEN * AUDIO_CHANNELS * currentBuffPos, g_outBuff, sizeof(short) * AUDIO_FRAME_LEN * AUDIO_CHANNELS);
		currentBuffPos++;
		if (currentBuffPos == WRITE_BUFF_SIZE) {
			writer.AppendData(writeBuff, AUDIO_CHANNELS * AUDIO_FRAME_LEN * WRITE_BUFF_SIZE * sizeof(short));
			currentBuffPos = 0;
		}
		time += 23;
	}
	writer.Finish();
	main_finish();
	_fclose_(input);
}

