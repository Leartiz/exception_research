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
    void test_Params_buildExceptionCaseIndexs();
    void test_Params_normalizeReadTimeUnit();
    void test_Params_normalizedTimeUnitToRatio();
    void test_Params_isExceptionCaseIndex();
    void test_Params_generateCoeffs();
    void test_Params_generateCoeffs_1();

private slots:
    void test_Utils_sumVec();
};

#endif // TST_COMMON_H
