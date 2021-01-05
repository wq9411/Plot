#include "image.h"

void Image::init(QLabel *canvas){
    m_canvas = canvas;
    m_canvas->setScaledContents(true);
    m_canvas->setMouseTracking(true);
}

void Image::imread(QString &imgname){
    QString imgfile = m_file_root + "/" + imgname;
    m_image.load(imgfile);
    m_width = m_image.width();
    m_height = m_image.height();
}

void Image::display(QPainter &painter){
    //canvas->setFixedSize(m_width, m_height);
    m_scale[0] = float(m_canvas->width()) / m_width;
    m_scale[1] = float(m_canvas->height()) / m_height;
    painter.drawPixmap(0, 0, m_canvas->width(), m_canvas->height(), m_image);
}
