#include "image.h"

void Image::init(QLabel *canvas){
    m_canvas = canvas;
    m_canvas->setScaledContents(true);
    m_canvas->setMouseTracking(true);
}

void Image::imread(QString &imgname){
    QString imgfile = m_file_root + "/" + imgname;
    m_image.load(imgfile);
    m_image_orig.load(imgfile);;
    m_width = m_image.width();
    m_height = m_image.height();
}

void Image::display(QPainter &painter){
    //canvas->setFixedSize(m_width, m_height);
    QPixmap pixmap2show = QPixmap::fromImage(m_image);
    m_scale[0] = float(m_canvas->width()) / m_width;
    m_scale[1] = float(m_canvas->height()) / m_height;
    painter.drawPixmap(0, 0, m_canvas->width(), m_canvas->height(), pixmap2show);
}

void Image::adjustBrightness(int brightness){
    int red, green, blue;
    QImage tmp_image = m_image_orig;
    int pixels = m_width * m_height;

    unsigned int *data = (unsigned int *)tmp_image.bits();
    for (int i = 0; i < pixels; ++i)
    {
        red= qRed(data[i])+ brightness;
        red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
        green= qGreen(data[i])+brightness;
        green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
        blue= qBlue(data[i])+brightness;
        blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
        data[i] = qRgba(red, green, blue, qAlpha(data[i]));
    }
    m_image = tmp_image;
}

void Image::adjustContrast(int contrast){
    QImage tmp_image = m_image_orig;
    int pixels = m_width * m_height;
    unsigned int *data = (unsigned int *)tmp_image.bits();

    int red, green, blue, nRed, nGreen, nBlue;

    if (contrast > 0 && contrast < 100)
    {
        float param = 1 / (1 - contrast / 100.0) - 1;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * param;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * param;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * param;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }else{
        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            red = nRed + (nRed - 127) * contrast / 100.0;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = nGreen + (nGreen - 127) * contrast / 100.0;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = nBlue + (nBlue - 127) * contrast / 100.0;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
    }
    m_image = tmp_image;
}
