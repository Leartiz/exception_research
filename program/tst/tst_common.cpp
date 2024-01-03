#include <string>
#include <algorithm>

#include <QDebug>

#include "tst_common.h"
#include "params.h"
#include "utils.h"

Common::Common(){}
Common::~Common(){}

// Params
// -----------------------------------------------------------------------

void Common::test_Params_buildExceptionCaseIndexs()
{
    struct TestCase {
        size_t equation_count;
        size_t exception_count;
        std::vector<int> expected;
    } const testCases[] = {
        { 10, 3, { 1, 4, 7 } },
        { 15, 3, { 2, 7, 12 } },
        { 25, 4, { 3, 9, 15, 21 } },
        { 100, 5, { 10, 30, 50, 70, 90 } },
        //...
    };

    for (const auto& testCase : testCases) {
        Params::equation_count = testCase.equation_count;
        Params::exception_count = testCase.exception_count;

        const auto actual = Params::build_exception_case_indexs();
        QCOMPARE_EQ(actual, testCase.expected);
    }
}

void Common::test_Params_normalizeReadTimeUnit()
{
    struct TestCase {
        std::string input;
        std::string expected;
    } const testCases[] = {
        { "s", "s" },
        { "ms", "ms" },
        { "us", "us" },
        { "123", "us" },
        { "___", "us" },
    };

    for (const auto& testCase : testCases) {
        const auto actual = Params::normalize_read_time_unit(
            testCase.input);
        QCOMPARE_EQ(actual, testCase.expected);
    }
}

void Common::test_Params_normalizedTimeUnitToRatio()
{
    struct TestCase {
        std::string input;
        double expected;
    } const testCases[] = {
        { "s", 1'000'000 },
        { "ms", 1'000 }, { "us", 1 },
        { "123", 1 }, { "___", 1 },
    };

    for (const auto& testCase : testCases) {
        const auto actual = Params::normalized_time_unit_to_ratio(
            testCase.input);
        QCOMPARE_EQ(actual, testCase.expected);
    }
}

void Common::test_Params_isExceptionCaseIndex()
{
    struct TestCase {
        size_t equation_count;
        size_t exception_count;
        std::vector<int> exception_case_indexs;

    } const testCases[] = {
        { 10, 3, { 1, 4, 7 } },
        { 15, 3, { 2, 7, 12 } },
        { 25, 4, { 3, 9, 15, 21 } },
        { 100, 5, { 10, 30, 50, 70, 90 } },
        //...
    };

    for (const auto& testCase : testCases) {
        Params::equation_count = testCase.equation_count;
        Params::exception_count = testCase.exception_count;
        Params::exception_case_indexs = Params::build_exception_case_indexs();

        // ***

        for (size_t i = 0; i < testCase.equation_count; ++i) {
            const auto indx = static_cast<int>(i);

            bool expected = false;
            const auto& indexs = testCase.exception_case_indexs;
            if (std::find(indexs.cbegin(), indexs.cend(), indx)
                != indexs.cend()) {
                expected = true;
            }

            QCOMPARE_EQ(Params::is_exception_case_index(indx), expected);
        }
    }
}

void Common::test_Params_generateCoeffs()
{
    struct TestCase {
        size_t equation_count;
        size_t exception_count;
        std::vector<int> exception_case_indexs;

    } const testCases[] = {
        { 10, 3, { 1, 4, 7 } },
        { 15, 3, { 2, 7, 12 } },
        { 25, 4, { 3, 9, 15, 21 } },
        { 100, 5, { 10, 30, 50, 70, 90 } },
        //...
    };

    for (const auto& testCase : testCases) {
        Params::equation_count = testCase.equation_count;
        Params::exception_count = testCase.exception_count;
        Params::exception_case_indexs = Params::build_exception_case_indexs();

        // ***

        for (size_t i = 0; i < testCase.equation_count; ++i) {
            const auto indx = static_cast<int>(i);

            const auto& indexs = testCase.exception_case_indexs;
            std::tuple<double, double, double> expected = { 0, 0, 0 };
            if (std::find(indexs.cbegin(), indexs.cend(), indx)
                == indexs.cend()) {

                // copied code from implementation!
                expected = {
                    ((indx % 2000) - 1000) / 33.0,
                    ((indx % 200) - 100) / 22.0,
                    ((indx % 20) - 10) / 11.0
                };
            }

            QCOMPARE_EQ(Params::generate_coeffs(indx), expected);
        }
    }
}

void Common::test_Params_generateCoeffs_1()
{
    // TODO:
}

// Utils
// -----------------------------------------------------------------------

void Common::test_Utils_sumVec()
{
    struct TestCase {
        std::vector<double> input;
        double expected;
    } const testCases[] = {
        { {}, 0 },
        { { 1, 2, 3 }, 6 },
        { std::vector<double>(), 0 },
    };

    QCOMPARE_EQ(std::vector<double>(0), std::vector<double>({}));
    for (const auto& testCase : testCases) {
        const auto actual = Utils::sum_vec(testCase.input);
        QCOMPARE_EQ(actual, testCase.expected);
    }

}

QTEST_APPLESS_MAIN(Common)
