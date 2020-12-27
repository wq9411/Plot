#include "plot.h"
#include "ui_plot.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QPainter>
#include <QMessageBox>
#include <sstream>
#include <string>
#include <QDebug>

plot::plot(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::plot)
{
    ui->setupUi(this);
    ui->display_image->setScaledContents(true);
    ui->display_image->installEventFilter(this);
    setFocusPolicy(Qt::StrongFocus);
    ui->centralWidget->setMouseTracking(true);
    setMouseTracking(true);
    ui->display_image->setMouseTracking(true);
    connect(ui->open_floder,SIGNAL(clicked(bool)),this,SLOT(openFloder()));
    connect(ui->save,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(ui->last_img,SIGNAL(clicked(bool)),this,SLOT(preImg()));
    connect(ui->next_img,SIGNAL(clicked(bool)),this,SLOT(nextImg()));
    connect(ui->delete_button,SIGNAL(clicked(bool)),this,SLOT(deleteRect()));
    connect(ui->skip,SIGNAL(clicked(bool)),this,SLOT(skipImg()));
    connect(ui->exit,SIGNAL(clicked(bool)),this,SLOT(windowClose()));
    m_imgid = 0;
    //counterPoint=0;
    m_numrects = 0;
    m_labels.init(ui->labels);

}
plot::~plot()
{
    delete ui;
}
void plot::openFloder() //打开图片文件夹
{
    m_filepath=QFileDialog::getExistingDirectory(this,tr("图片路径"),".");
    if(m_filepath.isEmpty()){
        return;
    }else{
        m_rects.setFileroot(m_filepath);
        ui->caption->setText("初始化...");
        QDir dir(m_filepath);
        QStringList namefilters;
        namefilters<<"*.jpg"<<"*.png"<<"*.jpeg";
        m_imgnamelists = dir.entryList(namefilters,QDir::Files|QDir::Readable,QDir::Name);
        //qDebug()<<imgNameList[0];
        ui->progressBar->setRange(0,m_imgnamelists.count()-1);
        ui->skip_line->setText("1");
        updateInf();
    }

}

void plot::displayImg()//显示图片
{
   const QString imagename=m_filepath+"/"+m_imgnamelists[m_imgid];
   QPixmap image;
   image.load(imagename);
   ui->display_image->setFixedSize(image.width(),image.height());
   m_imgwidth=image.width();
   m_imgheight=image.height();
   m_painter.drawPixmap(0,0,m_imgwidth,m_imgheight,image);
}
bool plot::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->display_image&&event->type()==QEvent::Paint&&!m_imgnamelists.isEmpty()){
        m_painter.begin(ui->display_image);
        displayImg();
        draw();
        update();
        m_painter.end();
        return false;
    }
    return QWidget::eventFilter(watched,event);
}
void plot::mousePressEvent(QMouseEvent *event)
{
    /*if(!m_imgnamelists.isEmpty()&&event->button()==Qt::LeftButton){
        m_startpoint = event->pos()-(ui->centralWidget->pos()+ui->display_image->pos());
        emit pressPoint(m_startpoint);
    }*/
    if(!m_imgnamelists.isEmpty()&&event->button()==Qt::RightButton){
            int labelid=ui->labels->currentRow();
            m_labels.addId(labelid);
            ui->labels->setCurrentRow(labelid);
    }

}
void plot::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_imgnamelists.isEmpty())
    {
        m_movepoint=event->pos()-(ui->centralWidget->pos()+ui->display_image->pos());
        if(event->buttons()&Qt::LeftButton){
            m_moverectpoint=m_movepoint;
        }else {
            m_moverectpoint.setX(0);
            m_moverectpoint.setY(0);
        }
        emit mobilePoint(m_movepoint);
    }
}
void plot::mouseReleaseEvent(QMouseEvent *event)
{
    if(!m_imgnamelists.isEmpty()&&event->button()==Qt::LeftButton)
    {
        m_point=event->pos()-(ui->centralWidget->pos()+ui->display_image->pos());
        if(m_point.x()>=0&&m_point.x()<=m_imgwidth&&m_point.y()>=0&&m_point.y()<=m_imgheight)//判断点是否在图像内部
        {
            m_pairpoint.append(m_point);
            if(m_pairpoint.count()==2){
                if( m_pairpoint[1].x()<m_pairpoint[0].x()){
                    int x = m_pairpoint[0].x();
                    m_pairpoint[0].setX(m_pairpoint[1].x());
                    m_pairpoint[1].setX(x);
                }
                if( m_pairpoint[1].y()<m_pairpoint[0].y()){
                    int y = m_pairpoint[0].y();
                    m_pairpoint[0].setY(m_pairpoint[1].y());
                    m_pairpoint[1].setY(y);
                }
                int labelid = ui->labels->currentRow();
                QListWidgetItem *item = ui->labels->item(labelid);
                m_rectinf.label = item->text();
                m_rectinf.minPoint = m_pairpoint[0];
                m_rectinf.maxPoint = m_pairpoint[1];
                m_rects.append(m_rectinf);
                QString displayinf = rectinf2string(m_rectinf);
                ui->plot_labels->insertItem(m_numrects, displayinf);
                ++m_numrects;
                m_pairpoint.clear();
            }
        }

        emit releasePoint(m_point);
    }
}
void plot::draw()
{
    m_painter.setPen(QPen(Qt::black,1,Qt::SolidLine));
    m_painter.drawLine(m_movepoint.x(),0,m_movepoint.x(),m_imgheight);
    m_painter.drawLine(0,m_movepoint.y(),m_imgwidth,m_movepoint.y());
    if(!m_pairpoint.isEmpty()){
        m_painter.setPen(QPen(Qt::red,2,Qt::SolidLine));
        m_painter.drawPoints(m_pairpoint);
    }

    m_rects.drawRects(m_painter);
    int id = ui->plot_labels->currentRow();
    if(id>=0 && id<ui->plot_labels->count()){
        RectInf& t_rect = m_rects.selectRect(id, m_painter);
        moveRectLine(t_rect.minPoint, t_rect.maxPoint);
        QString displayinf = rectinf2string(t_rect);
        QListWidgetItem *item=ui->plot_labels->takeItem(id);
        delete item;
        ui->plot_labels->insertItem(id, displayinf);
        ui->plot_labels->setCurrentRow(id);
    }

}
void plot::save()
{
    if(m_imgnamelists.isEmpty()){
        QMessageBox::information(this, tr("提示"),
                                 tr("请打开文件夹!"));
        return;
    }
    ui->caption->setText("保存中...");
    m_rects.save(m_imgnamelists[m_imgid]);
    ui->caption->setText("保存成功！");
}
void plot::recover()
{
  m_rects.recover(m_imgnamelists[m_imgid], ui->plot_labels);
}
void plot::updateInf()
{
    QString ratioDeal=QString("%1").arg(m_imgid+1)+"/"+QString("%1").arg(m_imgnamelists.count());
    ui->lineEdit->setText(ratioDeal);
    ui->progressBar->setValue(m_imgid);
    ui->caption->setText("No."+QString("%1").arg(m_imgid+1)+": "+m_imgnamelists[m_imgid]);
    m_pairpoint.clear();
    recover();
    m_numrects = ui->plot_labels->count();
}
void plot::preImg()
{
    if(m_imgnamelists.isEmpty()){
        QMessageBox::information (this, tr("提示"),
                                  tr("请打开文件夹!"));
        return;
    }
    if(m_imgid==0){
        QMessageBox::information (this,tr("提示"),
                                  tr("这是第一张!"));
         return;

    }else {
        save();
        --m_imgid;
        updateInf();
    }
}
void plot::nextImg()
{
    if(m_imgnamelists.isEmpty()){
        QMessageBox::information (this, tr("提示"),
                                  tr("请打开文件夹!"));
        return;
    }
    if(m_imgid==m_imgnamelists.count()-1)
    {
        QMessageBox::information (this, tr("提示"),
                                  tr("已是最后一张!"));
        return;
    }else {
        save();
        ++m_imgid;
        updateInf();
    }
}
void plot::skipImg()
{
    if(m_imgnamelists.isEmpty()){
        QMessageBox::information (this, tr("提示"),
                                  tr("请打开文件夹!"));
        return;
    }
    m_imgid = ui->skip_line->text().toInt() - 1;
    if(m_imgid<0){
        m_imgid = 0;
    }else if(m_imgid>=m_imgnamelists.count()){
        m_imgid=m_imgnamelists.count()-1;
    }
    updateInf();
}
void plot::windowClose()
{
    this->close();
}
void plot::deleteRect()
{
    int id = ui->plot_labels->currentRow();
    if(id>=0 && id<ui->plot_labels->count()){
        QListWidgetItem *item=ui->plot_labels->takeItem(id);
        delete item;
        m_rects.deleteRect(id);
        --m_numrects;
        m_pairpoint.clear();
    }
}
void plot::keyPressEvent(QKeyEvent *event)
{
//    if(!m_imgnamelists.isEmpty()&&event->key()==Qt::Key_C){
//        m_pairpoint.clear();
//        if(saveInf.find(imgNameList[currentImg])!=saveInf.end()){
//            saveInf[imgNameList[currentImg]].pop_back();
//            ui->plot_labels->clear();
//            if(saveInf[imgNameList[currentImg]].empty()){
//                auto mapIt=saveInf.find(imgNameList[currentImg]);
//                saveInf.erase(mapIt);
//            }else{
//                currentDrawRect=0;
//                for(const auto &plotLabel:saveInf[imgNameList[currentImg]]){
//                    ui->plot_labels->insertItem(currentDrawRect,labelLists[plotLabel.label].split(" ")[1]);
//                    ++currentDrawRect;
//                }
//            }
//        }
//        return;
//    }
    if(!m_imgnamelists.isEmpty()&&event->key()==Qt::Key_S){
        nextImg();
        return;
    }
    if(!m_imgnamelists.isEmpty()&&event->key()==Qt::Key_W){
        preImg();
        return;
    }
    if(!m_imgnamelists.isEmpty()&&event->key()==Qt::Key_Z){
        deleteRect();
        return;
    }
    QWidget::keyPressEvent(event);
}
void plot::moveRectLine(QPoint &point1, QPoint &point2)
{
    int interval = 20;
    if(abs(m_movepoint.x()-point1.x())<interval &&
       abs(m_movepoint.y()-point1.y())<interval){
        this->setCursor(Qt::SizeFDiagCursor);
        if(m_moverectpoint.x()!=0||m_moverectpoint.y()!=0){
            point1.setX(m_moverectpoint.x());
            point1.setY(m_moverectpoint.y());
            m_pairpoint.clear();
        }
    }else if(abs(m_movepoint.x()-point2.x())<interval &&
             abs(m_movepoint.y()-point2.y())<interval){
        this->setCursor(Qt::SizeFDiagCursor);
        if(m_moverectpoint.x()!=0||m_moverectpoint.y()!=0){
            point2.setX(m_moverectpoint.x());
            point2.setY(m_moverectpoint.y());
            m_pairpoint.clear();
        }
    }else if(abs(m_movepoint.x()-point2.x())<interval &&
             abs(m_movepoint.y()-point1.y())<interval){
        this->setCursor(Qt::SizeBDiagCursor);
        if(m_moverectpoint.x()!=0||m_moverectpoint.y()!=0){
            point2.setX(m_moverectpoint.x());
            point1.setY(m_moverectpoint.y());
            m_pairpoint.clear();
        }
    }else if(abs(m_movepoint.x()-point1.x())<interval &&
             abs(m_movepoint.y()-point2.y())<interval){
        this->setCursor(Qt::SizeBDiagCursor);
        if(m_moverectpoint.x()!=0||m_moverectpoint.y()!=0){
            point1.setX(m_moverectpoint.x());
            point2.setY(m_moverectpoint.y());
            m_pairpoint.clear();
        }
    }else {
        this->setCursor(Qt::ArrowCursor);
    }
}
