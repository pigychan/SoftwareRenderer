#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>


#pragma pack(push,1)    //将信息压入编译器堆栈顶部，将当前封装对齐值设置为1

//TGA文件头 ADT
struct TGA_Header {
    char idlength;          //图像信息字段长度，规定了Image ID字段的长度
    char colormaptype;      //颜色表类型
    char datatypecode;      //图像类型
    short colormaporigin;   //颜色表首地址
    short colormaplength;   //颜色表长度
    char colormapdepth;     //颜色表表项大小
    short x_origin;         //图像x坐标起始位置
    short y_origin;         //图像y坐标起始位置
    short width;            //图像宽度  
    short height;           //图像高度
    char  bitsperpixel;     //每个像素包含的位数
    char  imagedescriptor;  //图像描述符
};
#pragma pack(pop)       //弹出编译器堆栈顶部记录

//TGAColor ADT
struct TGAColor {
    unsigned char bgra[4];  //RGBA四个值
    unsigned char bytespp;  //bits per pixel

    //构造函数部分
    TGAColor() : bgra(), bytespp(1) {
        for (int i = 0; i < 4; i++) bgra[i] = 0;
    }
    TGAColor(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) : bgra(), bytespp(4) {
        bgra[0] = B;
        bgra[1] = G;
        bgra[2] = R;
        bgra[3] = A;
    }
    TGAColor(unsigned char v) : bgra(), bytespp(1) {
        for (int i = 0; i < 4; i++) bgra[i] = 0;
        bgra[0] = v;
    }
    TGAColor(const unsigned char* p, unsigned char bpp) : bgra(), bytespp(bpp) {
        for (int i = 0; i < (int)bpp; i++) {
            bgra[i] = p[i];
        }
        for (int i = bpp; i < 4; i++) {
            bgra[i] = 0;
        }
    }

    unsigned char& operator[](const int i) { return bgra[i]; }

    //重载 * 运算符，将颜色与光照强度相乘
    TGAColor operator *(float intensity) const {
        TGAColor res = *this;
        intensity = (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
        for (int i = 0; i < 4; i++) res.bgra[i] = bgra[i] * intensity;
        return res;
    }
};

class TGAImage {
protected:
    unsigned char* data;    //图像数据
    int width;              //宽度
    int height;             //高度    
    int bytespp;            //bits per pixel

    bool   load_rle_data(std::ifstream& in);
    bool unload_rle_data(std::ofstream& out);
public:
    //三种图像格式
    enum Format {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    //构造函数与析构函数
    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage& img);
    ~TGAImage();

    //运算符重载
    TGAImage& operator =(const TGAImage& img);

    //其他成员函数
    bool read_tga_file(const char* filename);
    bool write_tga_file(const char* filename, bool rle = true);
    bool flip_horizontally();
    bool flip_vertically();
    bool scale(int w, int h);
    TGAColor get(int x, int y);
    bool set(int x, int y, TGAColor& c);
    bool set(int x, int y, const TGAColor& c);


    int get_width();
    int get_height();
    int get_bytespp();
    unsigned char* buffer();
    void clear();
};

#endif //__IMAGE_H__
