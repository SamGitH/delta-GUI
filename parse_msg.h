#ifndef PARSE_MSG_H
#define PARSE_MSG_H

//#include <QMainWindow>
#include <QObject>

class parse_msg : public QObject
{
    Q_OBJECT
public:
    parse_msg();

    void test(QString str);

};

#endif // PARSE_MSG_H
