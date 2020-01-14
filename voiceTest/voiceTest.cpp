// voiceTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h> 
#include <windows.h>
#include <string>
#include <time.h>
#include<mmsystem.h>
#include<stdio.h>
#include<queue>
using std::queue;

using namespace std;
#pragma comment(lib, "winmm.lib")  


std::string timeToStr(time_t timeStamp);


int recoedSoundPCM(int seconds)
{
	const int freq = 8000;
	const int bitWidth = 16;
	HWAVEIN hWaveIn;  //输入设备
	WAVEFORMATEX waveform; //采集音频的格式，结构体
	BYTE* pBuffer1;//采集音频时的数据缓存
	WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体
	FILE* pf;

	HANDLE          wait;
	waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
	waveform.nSamplesPerSec = freq;//采样率，次/秒
	waveform.wBitsPerSample = bitWidth;//采样比特，16bits/次
	waveform.nChannels = 1;//采样声道数，2声道
	waveform.nAvgBytesPerSec = 2 * waveform.nSamplesPerSec;//每秒的数据率，就是每秒能采集多少字节的数据
	waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数
	waveform.cbSize = 0;//一般为0

	wait = CreateEvent(NULL, 0, 0, NULL);
	//使用waveInOpen函数开启音频采集
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//建立两个数组（这里可以建立多个数组）用来缓冲音频数据
	DWORD bufsize = 1024 * 100;//每次开辟100k的缓存存储录音数据
	string fileName = "record_" + timeToStr(time(nullptr)) + "_" + to_string(seconds) + "s_"\
		+ to_string(freq) + "Hz_" + to_string(bitWidth) + "bit.pcm";


	fopen_s(&pf, fileName.c_str(), "wb");
	if (pf == nullptr)
	{
		cout << "File [" << fileName << "] create fail.";
		return (-1);
	}

	pBuffer1 = new BYTE[bufsize];
	if (pBuffer1 == nullptr)
	{
		cout << "Memory malloc fail!" << endl;
		return (-1);
	}
	
	int sec = seconds;

	while (sec--)
	{
		wHdr1.lpData = (LPSTR)pBuffer1;
		wHdr1.dwBufferLength = bufsize;
		wHdr1.dwBytesRecorded = 0;
		wHdr1.dwUser = 0;
		wHdr1.dwFlags = 0;
		wHdr1.dwLoops = 1;
		waveInPrepareHeader(hWaveIn, &wHdr1, sizeof(WAVEHDR));//准备一个波形数据块头用于录音
		waveInAddBuffer(hWaveIn, &wHdr1, sizeof(WAVEHDR));//指定波形数据块为录音输入缓存
		waveInStart(hWaveIn);//开始录音
		Sleep(1000);//等待声音录制1s
		waveInReset(hWaveIn);//停止录音
		fwrite(pBuffer1, 1, wHdr1.dwBytesRecorded, pf);
		cout << "Time remain: " << sec << 's' << endl;
	}

	delete[] pBuffer1;

	fclose(pf);
	cout << "File:[ " << fileName << " ] saved." << endl;
	waveInClose(hWaveIn);
	return 0;
}


std::string timeToStr(time_t timeStamp) {
	tm ltm;
	localtime_s(&ltm, &timeStamp);

	std::string res = "0000-00-00_000000";
	int year = 1900 + ltm.tm_year;

	res[0] += year / 1000;
	res[1] += (year / 100) % 10;
	res[2] += (year / 10) % 10;
	res[3] += year % 10;

	res[5] += (1 + ltm.tm_mon) / 10;
	res[6] += (1 + ltm.tm_mon) % 10;

	res[8] += ltm.tm_mday / 10;
	res[9] += ltm.tm_mday % 10;

	res[11] += ltm.tm_hour / 10;
	res[12] += ltm.tm_hour % 10;
	res[13] += ltm.tm_min / 10;
	res[14] += ltm.tm_min % 10;
	res[15] += ltm.tm_sec / 10;
	res[16] += ltm.tm_sec % 10;

	return res;
}





/*
const uint32_t DATASIZE = (1024 * 10); //分次截取数据大小  
FILE* pcmfile;  //音频文件  
HWAVEOUT        hwo;

void CALLBACK WaveCallback(HWAVEOUT hWave, UINT uMsg, DWORD dwInstance, DWORD dw1, DWORD dw2)//回调函数  
{
	switch (uMsg)
	{
	case WOM_DONE://上次缓存播放完成,触发该事件  
	{
		LPWAVEHDR pWaveHeader = (LPWAVEHDR)dw1;
		pWaveHeader->dwBufferLength = fread(pWaveHeader->lpData, 1, DATASIZE, pcmfile);;
		waveOutPrepareHeader(hwo, pWaveHeader, sizeof(WAVEHDR));
		waveOutWrite(hwo, pWaveHeader, sizeof(WAVEHDR));
		break;
	}
	}
}

//C++ 播放音频流(PCM裸流)
// https://blog.csdn.net/weixinhum/article/details/29943973

int main()
{
	int             cnt;
	WAVEHDR         wh1;
	//WAVEHDR         wh2;
	WAVEFORMATEX    wfx;

	char* p1 = new char[DATASIZE];
	char* p2 = new char[DATASIZE];
	cout << sizeof(wh1) << endl;

	fopen_s(&pcmfile, "soundRecord2020-01-14_233716.pcm", "rb");//打开文件  
	if (pcmfile == nullptr)
	{
		exit(-1);
	}
	wfx.wFormatTag = WAVE_FORMAT_PCM;//设置波形声音的格式  
	wfx.nChannels = 1;//设置音频文件的通道数量  
	wfx.nSamplesPerSec = 8000;//设置每个声道播放和记录时的样本频率  
	wfx.nAvgBytesPerSec = 16000;//设置请求的平均数据传输率,单位byte/s。这个值对于创建缓冲大小是很有用的  
	wfx.nBlockAlign = 2;//以字节为单位设置块对齐  
	wfx.wBitsPerSample = 16;
	wfx.cbSize = 0;//额外信息的大小  
	wh1.dwLoops = 5;
	waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD)WaveCallback, 0L, CALLBACK_FUNCTION);
	//打开一个给定的波形音频输出装置来进行声音播放，方式为回调函数方式。如果是对话框程序，可以将第五个参数改为(DWORD)this，操作跟本Demo程序相似  

	wh1.dwLoops = 1;//播放区一  
	wh1.lpData = p1;
	wh1.dwBufferLength = DATASIZE;
	fread(wh1.lpData, 1, DATASIZE, pcmfile);
	wh1.dwFlags = 0L;
	waveOutPrepareHeader(hwo, &wh1, sizeof(WAVEHDR));//准备一个波形数据块用于播放  
	waveOutWrite(hwo, &wh1, sizeof(WAVEHDR));//在音频媒体中播放第二个参数指定的数据，也相当于开启一个播放区的意思  

	//wh2.dwLoops = 0L;//播放区二，基本同上  
	//wh2.lpData = p2;// new char[DATASIZE];
	//wh2.dwBufferLength = DATASIZE;
	//fread(wh2.lpData, 1, DATASIZE, pcmfile);
	//wh2.dwFlags = 0L;
	//waveOutPrepareHeader(hwo, &wh2, sizeof(WAVEHDR));
	//waveOutWrite(hwo, &wh2, sizeof(WAVEHDR));

	while (wh1.dwBufferLength != 0 )//|| wh2.dwBufferLength != 0)//如果文件还在没播放完则等待500ms  
	{
		Sleep(500);
	}
	waveOutUnprepareHeader(hwo, &wh1, sizeof(WAVEHDR));//清理数据  
	 //waveOutUnprepareHeader(hwo, &wh2, sizeof(WAVEHDR));

	delete[] wh1.lpData;
	//delete[] wh2.lpData;
	fclose(pcmfile);//关闭文件  
	return 0;
}
*/


/*
* some good values for block size and count
*/
#define BLOCK_SIZE 4096
#define BLOCK_COUNT 3
/*
*waveoutprocdecodethread share vars
*/
static CRITICAL_SECTION waveCriticalSection;
static HANDLE freeBufferSema;
static queue<WAVEHDR*> qWHDR;

static DWORD decodeThreadProc();
static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

HWAVEOUT hWaveOut;
MMRESULT ret;
WAVEFORMATEX wfx;

DWORD decodeThreadId;
HANDLE hDecodeThread;
char filename[] = "soundRecord2020-01-14_233716.pcm";

int pcmAudioPlay()
{
	int i;

	freeBufferSema = CreateSemaphore(0,
		0,
		BLOCK_COUNT,
		(LPCWSTR)"bufferSema"
	);

	if (freeBufferSema == NULL)
	{
		printf("Create Buffer Sema ERROR!\n");
		return 1;
	}

	InitializeCriticalSection(&waveCriticalSection);


	wfx.nSamplesPerSec = 8000;
	wfx.wBitsPerSample = 16;
	wfx.nChannels = 1;
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	ret = waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);
	if (ret != MMSYSERR_NOERROR)
	{
		printf("waveoutopen ERR!\n");
		return 1;
	}
	printf("waveout open successfully!\n");
	//decThreadData.device = &hWaveOut;
	//decThreadData.filename = filename;

	hDecodeThread = CreateThread(NULL, //Choose default security
		0, //Default stack size
		(LPTHREAD_START_ROUTINE)&decodeThreadProc,
		//Routine to execute
		0, //Thread parameter
		0, //Immediately run the thread
		&decodeThreadId //Thread Id
	);

	if (hDecodeThread == NULL)
	{
		printf("Error Creating Decode Thread\n");
		return 1;
	}

	WaitForSingleObject(hDecodeThread, INFINITE);

	Sleep(1000);

	waveOutClose(hWaveOut);
	CloseHandle(freeBufferSema);
	CloseHandle(hDecodeThread);
	DeleteCriticalSection(&waveCriticalSection);

}


static DWORD decodeThreadProc()
{
	WAVEHDR* current = NULL;
	WAVEHDR* waveBlocks = NULL;
	char* audioBuf = NULL;
	FILE* fp = NULL;
	int i;

	audioBuf = (char*)malloc(BLOCK_SIZE * BLOCK_COUNT); if (!audioBuf)return 0;
	memset(audioBuf, 0, BLOCK_SIZE * BLOCK_COUNT);
	waveBlocks = (WAVEHDR*)malloc(BLOCK_COUNT * sizeof(WAVEHDR)); if (!waveBlocks)return 0;
	memset(waveBlocks, 0, BLOCK_COUNT * sizeof(WAVEHDR));
	for (i = 0; i < BLOCK_COUNT; i++)
	{
		waveBlocks[i].lpData = (audioBuf + i * BLOCK_SIZE);
		waveBlocks[i].dwBufferLength = BLOCK_SIZE;
	}

	fopen_s(&fp, filename, "rb");
	if (fp == NULL)
	{
		printf("open file :%s error\n", filename);
		return -1;
	}

	//preread BLOCK_COUNT data to buffer
	for (i = 0; i < BLOCK_COUNT; i++)
	{
		current = &waveBlocks[i];
		fread(current->lpData, 1, BLOCK_SIZE, fp);
		waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));

	}
	for (i = 0; i < BLOCK_COUNT; i++)
	{
		current = &waveBlocks[i];
		printf("i:%x\n", current);
		waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
	}

	while (1)
	{
		int size = 0;
		//wait a free pcm buf
		WaitForSingleObject(
			freeBufferSema, // event handle
			INFINITE);    // indefinite wait

		//read data to pcm buf,then write to waveout
		//
		EnterCriticalSection(&waveCriticalSection);
		current = qWHDR.front();
		qWHDR.pop();
		LeaveCriticalSection(&waveCriticalSection);
		printf("f:%x\n", current);
		waveOutUnprepareHeader(hWaveOut, current, sizeof(WAVEHDR));
		size = fread(current->lpData, 1, BLOCK_SIZE, fp);
		//printf("size:%d\n",size);
		if (size > 0)
		{
			waveOutPrepareHeader(hWaveOut, current, sizeof(WAVEHDR));
			waveOutWrite(hWaveOut, current, sizeof(WAVEHDR));
		}
		else
		{
			printf("play over!%d\n", size);
			break;
		}
	}
	printf("decode finished!\n");
	fclose(fp);
	free(audioBuf);
	free(waveBlocks);
}

static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	WAVEHDR* freeWaveHdr = NULL;
	switch (uMsg)
	{
	case WOM_DONE:
		EnterCriticalSection(&waveCriticalSection);
		freeWaveHdr = (WAVEHDR*)dwParam1;
		printf("o:%x\n", freeWaveHdr);
		qWHDR.push(freeWaveHdr);
		LeaveCriticalSection(&waveCriticalSection);
		ReleaseSemaphore(freeBufferSema, 1, NULL);
		break;
	case WOM_OPEN:
		printf("open waveout\n");
		break;
	case WOM_CLOSE:
		printf("close waveout\n");
		break;
	default:
		printf("ERROR MSG!\n");
	}

}


int main() {

	//recoedSoundPCM(20);
	pcmAudioPlay();

	return 0;
}