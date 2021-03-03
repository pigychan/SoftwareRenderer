#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <fstream>


#pragma pack(push,1)    //����Ϣѹ���������ջ����������ǰ��װ����ֵ����Ϊ1

//TGA�ļ�ͷ ADT
struct TGA_Header {
    char idlength;          //ͼ����Ϣ�ֶγ��ȣ��涨��Image ID�ֶεĳ���
    char colormaptype;      //��ɫ������
    char datatypecode;      //ͼ������
    short colormaporigin;   //��ɫ���׵�ַ
    short colormaplength;   //��ɫ����
    char colormapdepth;     //��ɫ������С
    short x_origin;         //ͼ��x������ʼλ��
    short y_origin;         //ͼ��y������ʼλ��
    short width;            //ͼ����  
    short height;           //ͼ��߶�
    char  bitsperpixel;     //ÿ�����ذ�����λ��
    char  imagedescriptor;  //ͼ��������
};
#pragma pack(pop)       //������������ջ������¼

//TGAColor ADT
struct TGAColor {
    unsigned char bgra[4];  //RGBA�ĸ�ֵ
    unsigned char bytespp;  //bits per pixel

    //���캯������
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

    //���� * �����������ɫ�����ǿ�����
    TGAColor operator *(float intensity) const {
        TGAColor res = *this;
        intensity = (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
        for (int i = 0; i < 4; i++) res.bgra[i] = bgra[i] * intensity;
        return res;
    }
};

class TGAImage {
protected:
    unsigned char* data;    //ͼ������
    int width;              //���
    int height;             //�߶�    
    int bytespp;            //bits per pixel

    bool   load_rle_data(std::ifstream& in);
    bool unload_rle_data(std::ofstream& out);
public:
    //����ͼ���ʽ
    enum Format {
        GRAYSCALE = 1, RGB = 3, RGBA = 4
    };

    //���캯������������
    TGAImage();
    TGAImage(int w, int h, int bpp);
    TGAImage(const TGAImage& img);
    ~TGAImage();

    //���������
    TGAImage& operator =(const TGAImage& img);

    //������Ա����
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
