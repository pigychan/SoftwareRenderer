# PigyChan 软件渲染器

[English](README.md)|中文

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


![image](/images/merge2.jpg)

  接下来，我在https://github.com/SunXLei/SRender 这个开源项目中学习，为渲染器逐步添加了可移动相机，win32交互接口等功能。另外，我也采用了该项目中的模型资源。

  将tinyrender移植到win32程序过程中，请小心左手右手坐标系对应的透视矩阵。

  tinyrender少了一个透视插值过程，请使用呈线性的 1/w 来插值UV，光照，顶点等属性。

  这是正确插值形成的Phong着色渲染。

![image](/images/merge1.jpg)

  这是最终实现的渲染器效果：

![image](/images/miku.jpg)
