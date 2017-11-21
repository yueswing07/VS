// cJsonTest.cpp : 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

//#include "stdafx.h"
extern "C"  {
#include "cJSON.h"
}
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>
#include <thread>  
using namespace std;


#include "crtdbg.h" 
//添加该头文件
//#pragma comment( linker, "/subsystem:console" ) 
//在头文件下面添加下面宏定义
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC 
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

class DisplayThread {  
 public:  
  void operator ()() {  
    for (int i = 0; i < 100; i++)  
      std::cout << "Display Thread Excecuting" << std::endl;  
  }  
};  

// return string value  user need free memeroy.
// string return string;false,true return "0","1"; number return string; 
//others retrun NULL include failed.
char * cJSON_GetStringValue(cJSON *item)
{
	char *pStringValue = NULL;
	switch ((item->type) & 0xFF)
	{
	case cJSON_NULL:

		return NULL;

	case cJSON_False:{
		int len = strlen("0") + 1;
		pStringValue = (char *)malloc(len);
		strncpy(pStringValue, "0", len);
		return pStringValue;
	}

	case cJSON_True:{
		int len = strlen("1") + 1;
		pStringValue = (char *)malloc(len);
		strncpy(pStringValue, "1", len);
		return pStringValue;
	}

	case cJSON_Number:{
		pStringValue = (char *)malloc(64);
		if (!pStringValue){
			return NULL;
		}
		/* use saturation in case of overflow */
		/*if (item->valueint >= INT_MAX || item->valueint <= INT_MIN)
		{
			sprintf(pStringValue, "%d", item->valueint);
		}
		else
		{
			sprintf(pStringValue, "%1.15g", item->valuedouble);
		}*/
		// %g print 
		sprintf(pStringValue, "%1.15g", item->valuedouble);
		return pStringValue;
	}

	case cJSON_Raw:
		return NULL;

	case cJSON_String:{
		int len = strlen(item->valuestring) + 1;
		pStringValue = (char *)malloc(len);
		strncpy(pStringValue, item->valuestring, len);
		return pStringValue;
	}

	case cJSON_Array:
		return NULL;

	case cJSON_Object:
		return NULL;

	default:
		return NULL;
	}

}
//void main(){
//	return;
//}
//int _tmain(int argc, _TCHAR* argv[])
int main()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	//运行时添加该语句，在内存泄露前添加，一般加到程序的入口函数中。
	//_CrtSetBreakAlloc(223);
	char *s = "{\"list\":[{\"name\":\"22xiao hong\",\"age\":10},{\"name\":\"33hua hua\",\"age\":\"11\"}]}";  
    cJSON *root = cJSON_Parse(s);  
    if(!root) {  
        printf("get root faild !\n");  
        return -1;  
    }
	int size = cJSON_GetArraySize(root);
	printf("%d\n",size);
    cJSON *js_list = cJSON_GetObjectItem(root, "list");  
    if(!js_list){  
        printf("no list!\n");  
        return -1;  
    }  
    int array_size = cJSON_GetArraySize(js_list);  
    printf("array size is %d\n",array_size);  
    int i = 0;  
    cJSON *item,*it, *js_name, *js_age;  
    char *p  = NULL;  
    for(i=0; i< array_size; i++) {  
        item = cJSON_GetArrayItem(js_list, i);  
  //      if(!item) {  
  //          //TODO...  
  //      }  
  //      p = cJSON_PrintUnformatted(item);  
  //      it = cJSON_Parse(p);  
  //      if(!it)  
  //          continue ;  
  //      js_name = cJSON_GetObjectItem(it, "name");  
  //      printf("name is %s\n",js_name->valuestring);  
  //      js_age = cJSON_GetObjectItem(it, "age");  
  //      printf("age is %d\n",js_age->valueint);
		//free(p);
		//free(it);

		/*js_name = cJSON_GetObjectItem(item, "name");  
		printf("name is %s\n", js_name->valuestring);
		printf("name is %s\n", item->valuestring);
        js_age = cJSON_GetObjectItem(item, "age");  
		printf("age is %d\n", js_age->valueint);*/

		js_name = cJSON_GetObjectItem(item, "name");
		js_age = cJSON_GetObjectItem(item, "age");
		char *tmp = cJSON_GetStringValue(js_name);
		printf("name is %s\n", tmp);
		free(tmp);
		tmp = cJSON_GetStringValue(js_age);
		printf("age is %s\n", tmp);
		free(tmp);
      
    }


	// 创建JSON Object  
    cJSON *obj = cJSON_CreateObject();
    // 加入节点（键值对），节点名称为value，节点值为123.4  
    //cJSON_AddNumberToObject(obj,"value",123.4);  
    // 打印JSON数据包  
    char *out = cJSON_Print(obj);  
    printf("%s\n",out);
	if (out) {
		free(out);
		out = NULL;
	}
    out = cJSON_PrintUnformatted(obj);  
    printf("%s\n",out);  
    // 释放内存  
    cJSON_Delete(obj);
    free(out);
	
	// 数组实例
	cJSON *jsonArray = cJSON_CreateArray();
	char *jsonStr = cJSON_PrintUnformatted(jsonArray);
	printf("%s\n", jsonStr);
	if (jsonArray) {
		cJSON_Delete(jsonArray);
		jsonArray = NULL;
	}
	if (jsonStr) {
		free(jsonStr);
		jsonStr = NULL;
	}

    // map 使用
	map<int, string> mapStudent;  
    mapStudent.insert(pair<int, string>(1, "student_one"));  
    mapStudent.insert(pair<int, string>(2, "student_two"));  
    mapStudent.insert(pair<int, string>(3, "student_three"));  
    map<int, string>::iterator iter;  
    for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)  
       cout<<iter->first<<' '<<iter->second<<endl;  


	long long a = 0x7fffffffffffffff;
	printf(" long long %lld \n ",a);

	// c++ 多线程   
	/*std::thread threadObj((DisplayThread()));  
	for (int i = 0; i < 100; i++)  
	std::cout << "Display From Main Thread " << std::endl;  
	std::cout << "Waiting For Thread to complete" << std::endl;  
	threadObj.join();
	std::cout << "Exiting from Main Thread" << std::endl; */

	//  auto 使用 Lambda函数（匿名函数）
	auto f = [](int x, int y) { return x + y; };
	cout << f(21, 12) << std::endl;

	//a = 10000000;
	//std::thread thread1([a](){for (int i = 0; i < a; i++){} });
	//auto fun = [](int b)->int{for (int i = 0; i < b; i++){}return 0; };
	////std::thread thread2(fun(a));
	////如果没有在std::thread对象上调用join或detach，其析构函数将会终止程序  
	//thread1.join();
	////thread2.join();

	// memery leak 
	//char * pp = (char*)malloc(1042);

	getchar();
      
    if(root)  
        cJSON_Delete(root);  
        return 0;  
}

