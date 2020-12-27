#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QPoint>
#include <QVector>
#include <QString>
#include <QPainter>
#include <QListWidget>
#include "csv.h"

struct RectInf
{
    QPoint minPoint;
    QPoint maxPoint;
    QString label;
};

QString rectinf2string(const RectInf& rect);

class Rectangle
{
public:
    Rectangle(){}
    void append(RectInf& rect);
    void clear();
    void deleteRect(int id);
    void setFileroot(const QString& fileroot);
    void save(const QString& imgname);
    void recover(const QString& imgname, QListWidget *rectWidget);
    void drawRects(QPainter& painter);
    RectInf& selectRect(int id, QPainter& painter);

private:
    QVector<RectInf> m_rects;
    QString m_fileroot;
    CSV_OP m_csv_op;
};

class Labels
{
public:
    Labels(){}
    void init(QListWidget *labelWidget);
    void addId(int& labelid);

private:
    QStringList m_labels;
    CSV_OP m_csv_op;
};

#endif // RECTANGLE_H
