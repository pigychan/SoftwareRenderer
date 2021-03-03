# PigyChan 软件渲染器

[English](README.md)|中文

这是一个在windows平台上用c++从头开始编写的光栅化渲染器，具有最小的依赖性。编写这个渲染器的目的主要是为了更好地理解计算机图形学的基础知识。

## 特性

- 原生Win32接口，无依赖

- 可编程着色器

- Z缓存测试

- 透视校正

- 重心插值校正

- 可交互的渲染场景

- 可移动相机

## 总结

  该渲染器是我在学习了闫令琪老师的《Games101》计算机图形学课程后，着手的第一个实践项目。

  在最开始，我在https://github.com/ssloy/tinyrenderer/wiki 这个开源项目中学习，完成了渲染器基本的渲染功能，以下是其渲染出来的图片展示：


![image](/image/Africa1.jpg) ![image](/image/Diablo.png)
![image](/image/Africa2.png) ![image](/image/AfricaZ.png) 

  接下来，我在https://github.com/SunXLei/SRender 这个开源项目中学习，为渲染器逐步添加了可移动相机，win32交互接口等功能。另外，我也采用了该项目中的模型资源。

  这是最终实现的渲染器效果：

![image](/image/fuhua.PNG) ![image](/image/gun.PNG) ![image](/image/qiyana.PNG) ![image](/image/xier.PNG) ![image](/image/yayi.PNG)

## 引用
### 《games101》--闫令琪
### https://github.com/SunXLei/SRender
