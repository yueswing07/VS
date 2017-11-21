# VS
Visual Studio 的使用， c , c++ 开发  MFC开发积累。
## vsUTF8Template 工程
vsUTF8Template 工程能构建uf8模板的vs开发环境。对于开发纯c,c++程序终于可以
用通用的utf8编码了。当然最完美的方案还是纯ASCII字符就没问题的。
构建流程：
* 用vsUTF8Template目录里hfile.h，newc++file.cpp文件覆盖
  vs 安装目录的模板文件。如vs2013 目录C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcprojectitems，这样以后在vs新建的.h,.cpp文件就是带bom的utf8文件。
* utf8支持，这里需要理解三个地方，文件编码，程序编码，显示编码。虽然文件是utf8编码的，但是vs编译后的二进制流编码是有编译决定的（默认更系统编码有关），这里可以在文件开始加一句
#pragma execution_character_set("utf-8") // 设置执行的编码为utf-8 
使编译的编码为utf8编码。显示编码也系统编码有关，这里我的系统默认是gbk的
所以控制台输出和调试变量常看中文会乱码，没有什么解决方案，将调试信息输出
到文件（注意输出到文件只输出uft8编码的），这样就可以方便的查看了。
备注：工程里封装了一个简单类DEBUG可用于调试信息输出到文件。
在vs中utf8 要使用带bom，不利于跨平台使用，在linux下用脚本转换
uf8bom.sh脚本摘录了网上代码。
linux 下utf bom 去除
https://www.cnblogs.com/Mach-he/p/6683074.html?utm_source=itdadao&utm_medium=referral
http://blog.sina.com.cn/s/blog_50ebb8ad0102vkg3.html

## vs dll lib 构建
目录vsUTF8Template/cJSON项目是cJSON构建lib,dll和调试的结合体。
项目方式：创建项目，选择空项目，导入需要生成lib或者dll的文件，如果程序
没问题可以右键项目名称->属性->配置属性->General->configuration Type 
选择lib（输出.lib文件到debug或release目录）或者dll（输出.lib和.dll文件到debug或release目录）输出。
需要调试工程： 上面选择application(.exe)输出，然后给工程添加一个 main.cpp 文件
添加 void main(){} 调试工程。完成后可以在重复上面的方法输出lib或dll.
注意： 注意release，debug 不同方式下都需要单独配置（包括文件包含，类库，dll类库
引入，输出等）
项目中 我将cJSON编译的dll 添加到了 vsUTF8Template 里面用。
备注：Debug模式下默认定义了_DEBUG宏，建议多使用。
参考：
https://www.cnblogs.com/qinguoyi/p/7257353.html
http://blog.csdn.net/sunylat/article/details/26216797

