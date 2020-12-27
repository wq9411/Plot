#ifndef PLOT_H
#define PLOT_H

#include <QMap>
#include <QSet>
#include <QMainWindow>
#include <QString>
#include <QObject>
#include <QMouseEvent>
#include <Qvector>
#include <QPainter>
#include "rectangle.h"

namespace Ui {
class plot;
}

class plot : public QMainWindow
{
    Q_OBJECT


public:
    explicit plot(QWidget *parent = nullptr);
    ~plot();
public slots:
    void openFloder();
    void save();
    void preImg();
    void nextImg();
    void deleteRect();
    void skipImg();
    void windowClose();

protected:
   bool eventFilter(QObject *watched,QEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   virtual void keyPressEvent(QKeyEvent *event);

signals:
   void pressPoint(QPoint s);
   void mobilePoint(QPoint m);
   void releasePoint(QPoint e);

private:
    Ui::plot *ui;
    QPainter m_painter; //画布
    QString m_filepath;//图片根目录
    Labels m_labels;   //标签信息
    RectInf m_rectinf; //当前标注信息
    Rectangle m_rects; //标注信息
    QStringList m_imgnamelists; //文件夹中图片名
    int m_imgwidth,m_imgheight,m_imgid;//图像宽、高、当前图片计数
    int m_numrects;//当前画矩阵数
    QPoint m_movepoint,m_point,m_moverectpoint;
    QPolygon m_pairpoint;//存储最小点和最大点

    void displayImg();//显示图片
    void draw();//画矩形
    void recover(); //恢复标注状态
    void updateInf();//更新信息
    void moveRectLine(QPoint &point1, QPoint &point2);//修改矩形框

};

#endif // PLOT_H
