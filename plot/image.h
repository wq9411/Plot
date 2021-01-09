#ifndef IMAGE_H
#define IMAGE_H
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QPainter>


class Image
{
public:
    Image():m_canvas(nullptr){}
    ~Image() {}
    void init(QLabel* canvas);
    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
    const float* getScale() {return m_scale;}
    void setFileRoot(QString &fileroot) {m_file_root = fileroot;}
    void imread(QString &imgname);
    void display(QPainter &painter);
    void adjustBrightness(int brightness);
    void adjustContrast(int contrast);


private:
    QLabel* m_canvas;
    QImage m_image_show;
    QImage m_image_orig;
    QString m_file_root;
    int m_width;
    int m_height;
    float m_scale[2];   
};

#endif // IMAGE_H
