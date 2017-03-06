#ifndef TESTPARENT_H
#define TESTPARENT_H

#include <QObject>

class TestParent : public QObject
{
    Q_OBJECT
public:
    explicit TestParent(QObject *parent = 0);

signals:

public slots:
};

#endif // TESTPARENT_H