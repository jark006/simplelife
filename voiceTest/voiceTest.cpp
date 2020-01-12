// voiceTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h> 
#include <windows.h>
#include <string>
#include <time.h>

using namespace std;
#pragma comment(lib, "winmm.lib")  


std::string timeToStr(time_t timeStamp);

HWAVEIN hWaveIn;  //输入设备
WAVEFORMATEX waveform; //采集音频的格式，结构体
BYTE* pBuffer1;//采集音频时的数据缓存
WAVEHDR wHdr1; //采集音频时包含数据缓存的结构体
FILE* pf;
int main(int argc, char* argv[])
{
	HANDLE          wait;
	waveform.wFormatTag = WAVE_FORMAT_PCM;//声音格式为PCM
	waveform.nSamplesPerSec = 8192;//采样率，次/秒
	waveform.wBitsPerSample = 16;//采样比特，16bits/次
	waveform.nChannels = 1;//采样声道数，2声道
	waveform.nAvgBytesPerSec = 2 * waveform.nSamplesPerSec;//每秒的数据率，就是每秒能采集多少字节的数据
	waveform.nBlockAlign = 2;//一个块的大小，采样bit的字节数乘以声道数
	waveform.cbSize = 0;//一般为0

	wait = CreateEvent(NULL, 0, 0, NULL);
	//使用waveInOpen函数开启音频采集
	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD_PTR)wait, 0L, CALLBACK_EVENT);

	//建立两个数组（这里可以建立多个数组）用来缓冲音频数据
	DWORD bufsize = 1024 * 100;//每次开辟10k的缓存存储录音数据
	int i = 10;
	string fileName = "录音" + timeToStr(time(nullptr)) + ".pcm";
	fopen_s(&pf, fileName.c_str(), "wb");
	if (pf == nullptr)
	{
		cout << "File [" << fileName << "] create fail.";
		exit(-1);
	}

	pBuffer1 = new BYTE[bufsize];
	if (pBuffer1 == nullptr)
	{
		exit(-1);
	}
	while (i--)//录制20左右秒声音，结合音频解码和网络传输可以修改为实时录音播放的机制以实现对讲功能
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
		printf("%ds  ", i);
	}
	delete[] pBuffer1;

	fclose(pf);

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

