#ifndef A_H
#define A_H

#include <QObject>

class A : public QObject
{
    Q_OBJECT

public:
    explicit A(QObject *parent = 0);
    ~A();

    void start();
    void startTimer();

signals:
    void signal();
    void signal2();

public slots:
    void onSignal();
    void onSignal2();
    void onTimer();
};

#endif // A_H


