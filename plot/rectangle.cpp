#include "rectangle.h"
#include <QFileInfo>
#include <QDir>

QString rectinf2string(const RectInf &rect){
    QString str = rect.label + "," + QString::number(rect.minPoint.x())
            + "," + QString::number(rect.minPoint.y()) + ","
            + QString::number(rect.maxPoint.x()) + "," +
            QString::number(rect.maxPoint.y());
    return str;
}


void Rectangle::append(RectInf& rect){
    m_rects.push_back(rect);
}

void Rectangle::clear(){
    m_rects.clear();
}

void Rectangle::deleteRect(int id){
    if(id >=0 && id<m_rects.size()){
        m_rects.remove(id);
    }
}

void Rectangle::setFileroot(const QString &fileroot){
    m_fileroot = fileroot;
}

void Rectangle::save(const QString &imgname){
    QStringList saveinf;
    for (int i=0; i<m_rects.size(); ++i){
        QString str = rectinf2string(m_rects[i]);
        saveinf.append(str);
    }
    QString savepath = m_fileroot + "/" + imgname.split(".")[0] + ".csv";
    m_csv_op.writer(savepath, saveinf);
}

void Rectangle::recover(const QString &imgname, QListWidget *rectWidget){
    QString filepath = m_fileroot + "/" + imgname.split(".")[0] + ".csv";
    m_rects.clear();
    rectWidget->clear();
    QFileInfo file(filepath);
    if (file.exists()){
        QStringList recoverinf;
        RectInf rect;
        m_csv_op.reader(filepath, recoverinf);
        for (int i=1; i<recoverinf.size(); ++i){
            rectWidget->insertItem(i-1, recoverinf[i]);
            QStringList s_rectinf = recoverinf[i].split(",");
            rect.label = s_rectinf[0];
            rect.minPoint.setX(s_rectinf[1].toInt());
            rect.minPoint.setY(s_rectinf[2].toInt());
            rect.maxPoint.setX(s_rectinf[3].toInt());
            rect.maxPoint.setY(s_rectinf[4].toInt());
            m_rects.push_back(rect);
        }
    }
}

void Rectangle::drawRects(QPainter &painter){
    if(!m_rects.empty()){
        for(int i=0; i<m_rects.size(); ++i){
            int x = m_rects[i].minPoint.x();
            int y = m_rects[i].minPoint.y();
            int w = m_rects[i].maxPoint.x() - x;
            int h = m_rects[i].maxPoint.y() - y;
            painter.setPen(QPen(Qt::red,2,Qt::SolidLine));//设置画笔形式
            painter.drawRect(x,y,w,h);
            painter.setPen(QPen(Qt::black,1,Qt::SolidLine));//设置画笔形式
            painter.drawText(x,y,m_rects[i].label);
        }
    }
}

RectInf& Rectangle::selectRect(int id, QPainter &painter){
    if(id>=0 && id<m_rects.size()){
        int x = m_rects[id].minPoint.x();
        int y = m_rects[id].minPoint.y();
        int w = m_rects[id].maxPoint.x() - x;
        int h = m_rects[id].maxPoint.y() - y;
        painter.fillRect(QRectF(x,y,w,h),QBrush(QColor(0,0,255,50)));
        return m_rects[id];
    }
}

void Labels::init(QListWidget *labelWidget){
    QString currentpath = QDir::currentPath();
    //qDebug() << currentpath;
    currentpath += "/labelmap.csv";
    m_csv_op.reader(currentpath, m_labels);
    labelWidget->clear();
    for(int i=0; i<m_labels.size(); ++i){
        labelWidget->insertItem(i,m_labels[i]);
    }
    labelWidget->setCurrentRow(0);
}

void Labels::addId(int &labelid){
    if(labelid>=0&&labelid<m_labels.count()-1){
        ++labelid;
    }else{
       labelid = 0;
    }
}


