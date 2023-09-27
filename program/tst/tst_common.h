#ifndef TST_COMMON_H
#define TST_COMMON_H

#include <QtTest>

class Common : public QObject
{
    Q_OBJECT

public:
    Common();
    ~Common();

private slots:
    void test_case1();
};

#endif // TST_COMMON_H
