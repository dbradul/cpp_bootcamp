#ifndef TESTCHILD_H
#define TESTCHILD_H

#include <QObject>

class TestChild : public QObject
{
    Q_OBJECT
public:
    explicit TestChild(QObject *parent = 0);

signals:

public slots:
};

#endif // TESTCHILD_H