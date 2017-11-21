// vsUTF8Template.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "curl/curl.h"
//#include "include/curl/curl.h"
#include "cJSON.h"

#pragma once
#pragma execution_character_set("utf-8") // 设置执行的编码为utf-8 


typedef struct{
	char  data[81920];
	int   dataIdx;
}curlParam;

//static int curlGet(char *url, curlParam *userInfoData);
//static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
	curlParam* param = (curlParam*)userp;

	memcpy(param->data + param->dataIdx, buffer, size*nmemb);
	param->dataIdx += size*nmemb;
	//((char*)userp)[size*nmemb]=0;
	//printf("---%d\n", size*nmemb);

	return size*nmemb;
}
static int curlGet(char *url, curlParam *userInfoData)
{
	CURL *curl;
	CURLcode res;

	userInfoData->dataIdx = 0;
	curl = curl_easy_init();
	if (curl) {
		//
		//sprintf(url, "%sAppV2/AppGlobal/login?userPhoneNumber=%s&code=%s&codeType=%s&appID=%s&appToken=%s",PREURL_JOYCLASS, userPhoneNumber, code, codeType, appID, appToken);

#ifdef MY_DEBUG 
		printf("%s\r\n", url);
#endif

		curl_easy_setopt(curl, CURLOPT_URL, url);
		//res = curl_easy_perform(curl);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, userInfoData);

		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			return -1;
		}

		/* always cleanup */
		curl_easy_cleanup(curl);

		userInfoData->data[userInfoData->dataIdx] = 0;

		return 0;
	}
	return -1;
}


int GBK2UTF8(char *szGbk, char *szUtf8, int Len)
{
	// 先将多字节GBK（CP_ACP或ANSI）转换成宽字符UTF-16  
	// 得到转换后，所需要的内存字符数  
	int n = MultiByteToWideChar(CP_ACP, 0, szGbk, -1, NULL, 0);
	// 字符数乘以 sizeof(WCHAR) 得到字节数  
	WCHAR *str1 = new WCHAR[n];
	// 转换  
	MultiByteToWideChar(CP_ACP,  // MultiByte的代码页Code Page  
		0,            //附加标志，与音标有关  
		szGbk,        // 输入的GBK字符串  
		-1,           // 输入字符串长度，-1表示由函数内部计算  
		str1,         // 输出  
		n             // 输出所需分配的内存  
		);

	// 再将宽字符（UTF-16）转换多字节（UTF-8）  
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]str1;
		str1 = NULL;
		return -1;
	}
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, szUtf8, n, NULL, NULL);
	delete[]str1;
	str1 = NULL;

	return 0;
}
int UTF82GBK(char *szUtf8, char *szGbk, int Len)
{
	int n = MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, NULL, 0);
	WCHAR * wszGBK = new WCHAR[n];
	memset(wszGBK, 0, sizeof(WCHAR) * n);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, -1, wszGBK, n);

	n = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	if (n > Len)
	{
		delete[]wszGBK;
		wszGBK = NULL;
		return -1;
	}

	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGbk, n, NULL, NULL);

	delete[]wszGBK;
	wszGBK = NULL;

	return 0;
}

//#define BUG()    fprintf("Bug in function: %s (file: %s), @line: %d. It is compiled on %s  %s, %s ANSI C standard.\n", __FUNCTION__, __FILE__, __LINE__, __TIME__, __DATE__, __STDC__? "with" : "without");
class DEBUG {
public:
	DEBUG();
	~DEBUG();
	int printf(char *pszFormat, ...);

private:
	FILE *m_fd;
};
DEBUG::DEBUG(){
	m_fd = fopen("debug.log", "a+");
}
DEBUG::~DEBUG(){
	if (NULL != m_fd){
		fclose(m_fd);
		m_fd = NULL;
	}
}
int DEBUG::printf(char *pszFormat, ...)
{
	va_list   pArgList;

	va_start(pArgList, pszFormat);
	int nByteWrite = vfprintf(m_fd, pszFormat, pArgList);
	//if (nByteWrite != -1)
	//	putchar('\n'); //注2
	va_end(pArgList);

	return (nByteWrite == -1 ? -1 : nByteWrite + 1);
}
DEBUG log;
int _tmain(int argc, _TCHAR* argv[])
{
	// file open 
	//FILE *logFile = fopen("debug.log","wb+");
	
	curlParam curlStr;
	//fprintf(logFile, "UFT 汉子");
	log.printf("UFT 汉子");
	//fwrite("UFT 汉子 \n", 1, strlen("UFT 汉子"), logFile);
	curlGet("http://joyclassproduction.51joyshow.com/Home/Login/login", &curlStr);
	cJSON *p = cJSON_Parse(curlStr.data);
	char *str = cJSON_Print(p);
	//fprintf(logFile, "%s\n", str);
	log.printf("%s\n", str);
	//char gbkStr[1024];
	//UTF82GBK(str, gbkStr, 1024);
	//fprintf(logFile,"%s\n", gbkStr);
	//fclose(logFile);
	getchar();
	return 0;
}

