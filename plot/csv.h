#ifndef CSV_H
#define CSV_H
#include <QString>
#include <QStringList>

class CSV_OP
{
public:
    CSV_OP();
    void reader(const QString& filename, QStringList& lists);
    void writer(const QString& filename, const QStringList& lists, bool wr_header=true);

private:
    QString m_header;

};




#endif // IMAGE_H
