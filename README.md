这是我自写的一个地震数据格式（segy数据和txt数据）的读写小工具，
主要完成segy以及txt格式存储的地震数据的读写与显示，以及segy到txt
的转换。

之所以要写这个工具，是因为使用matlab仿真时，matlab不支持segy数据
的读取，而一般的地震数据处理软件又不支持segy到txt格式的转换，为了
方便使用，我就自己写了个segy解析程序，在此基础上，实现了地震数据的
显示（支持多道和单道），显示形式支持填充和非填充、平滑和非平滑以及
子波数据的显示等。

反演功能暂时不可用，界面用的是QT库，我用的版本是QT4.8.6，QT5.x不能
直接编译通过。