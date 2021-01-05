#ifndef IMAGE_H
#define IMAGE_H
#include <QPixmap>
#include <QLabel>
#include <QString>
#include <QPainter>


class Image
{
public:
    Image():m_canvas(nullptr), m_scale({1.0, 1.0}) {}
    ~Image() {}
    void init(QLabel* canvas);
    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
    const float* getScale() {return m_scale;}
    void setFileRoot(QString &fileroot) {m_file_root = fileroot;}
    void imread(QString &imgname);
    void display(QPainter &painter);



private:
    QLabel* m_canvas;
    QPixmap m_image;
    QString m_file_root;
    int m_width;
    int m_height;
    float m_scale[2];   
};

#endif // IMAGE_H
