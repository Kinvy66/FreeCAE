
本项目是基于[freecaeworkbench](https://github.com/Kinvy66/freecaeworkbench)上进行重构。

编译环境： Qt6.8.3+MSVC2022

第三方库下载链接: [https://pan.baidu.com/s/1G6D5cN9_vdIGqD3bcVWRlw?pwd=cybk](https://pan.baidu.com/s/1G6D5cN9_vdIGqD3bcVWRlw?pwd=cybk) 提取码: cybk


第三方库直接解压在工程的根目录。

项目第一次编译，在根目录中CMakeList中的设置`FC_COPY_THIRDPARTY_DLLS`为`ON`, 该选项可以在编译后自动拷贝第三方库dll到编译目录下

```cmake
option(FC_COPY_THIRDPARTY_DLLS
    "This option controls whether to copy third-party DLLs to build directory. If ON, only copy when source DLL is newer than target DLL"
    ON)
```

第三方库版本
|    名称  | 版本 |
|---|----|
|SARibbon    |2.5.7  |
|VTK         |9.4.1  |
|OCC         |7.9.3  |
|spdlog      |1.16.0 |
|qwk         |1.4.0.0|
|ADS         |4.4.1  |
|quazip      |1.5    |
|zlib        |1.4.1.1|
|gmsh        |4.15.0 |
|pybind11    |3.0.2  |

