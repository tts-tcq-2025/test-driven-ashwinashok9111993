#include <gtest/gtest.h>
#include <string>
#include "StringCalculator.h"
#include <stdexcept>
#include <tuple>

class StringCalculatorTest : public ::testing::Test {
 protected:
    StringCalculator calculator;  // Used in test methods below
};

// Parameterized test data structures
struct BasicAdditionData {
    std::string input;
    int expected;
    std::string description;
};

struct CustomDelimiterData {
    std::string input;
    int expected;
    std::string description;
};

struct InvalidInputData {
    std::string input;
    std::string expectedMessage;
    std::string description;
};

TEST_F(StringCalculatorTest, EmptyStringReturnsZero) {
    EXPECT_EQ(0, calculator.Add(""));
}

// Base template for parameterized tests with common test pattern
template<typename TestDataType>
class ParameterizedCalculatorTest : public ::testing::TestWithParam<TestDataType> {
 protected:
    StringCalculator calculator;

    void TestCalculatorExpectation(const TestDataType& data) {
        EXPECT_EQ(data.expected, calculator.Add(data.input)) << "Failed for: " << data.description;
    }
};

// Parameterized test for basic addition scenarios
class BasicAdditionTest : public ParameterizedCalculatorTest<BasicAdditionData> {};

TEST_P(BasicAdditionTest, HandlesVariousInputs) {
    TestCalculatorExpectation(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    SingleAndMultipleNumbers,
    BasicAdditionTest,
    ::testing::Values(
        BasicAdditionData{"1", 1, "single number"},
        BasicAdditionData{"5", 5, "single digit"},
        BasicAdditionData{"42", 42, "double digit"},
        BasicAdditionData{"1,2", 3, "two numbers with comma"},
        BasicAdditionData{"3,4", 7, "different two numbers"},
        BasicAdditionData{"8,7", 15, "larger two numbers"},
        BasicAdditionData{"1,2,3", 6, "three numbers"},
        BasicAdditionData{"1,2,3,4", 10, "four numbers"},
        BasicAdditionData{"1,2,3,4,5", 15, "five numbers"}
    )
);

// Parameterized test for delimiter scenarios
class DelimiterTest : public ParameterizedCalculatorTest<CustomDelimiterData> {};

TEST_P(DelimiterTest, HandlesVariousDelimiters) {
    TestCalculatorExpectation(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    AllDelimiterTypes,
    DelimiterTest,
    ::testing::Values(
        // Newline delimiters
        CustomDelimiterData{"1\n2,3", 6, "mixed newline and comma"},
        CustomDelimiterData{"1\n2\n3,4", 10, "multiple newlines with comma"},

        // Custom single-character delimiters
        CustomDelimiterData{"//;\n1;2", 3, "semicolon delimiter"},
        CustomDelimiterData{"//*\n1*2*3", 6, "asterisk delimiter"},
        CustomDelimiterData{"//|\n1|2|3|4", 10, "pipe delimiter"},

        // Custom multi-character delimiters
        CustomDelimiterData{"//[***]\n1***2***3", 6, "triple asterisk delimiter"},
        CustomDelimiterData{"//[abc]\n1abc2abc3abc4", 10, "text delimiter"},
        CustomDelimiterData{"//[::]\n1::2::3::4::5", 15, "double colon delimiter"}
    )
);

// Parameterized test for large number filtering
class LargeNumberFilterTest : public ParameterizedCalculatorTest<BasicAdditionData> {};

TEST_P(LargeNumberFilterTest, FiltersLargeNumbers) {
    TestCalculatorExpectation(GetParam());
}

INSTANTIATE_TEST_SUITE_P(
    NumberFiltering,
    LargeNumberFilterTest,
    ::testing::Values(
        BasicAdditionData{"2,1001", 2, "number over 1000 ignored"},
        BasicAdditionData{"1000,2", 1002, "exactly 1000 included"},
        BasicAdditionData{"1,2,1001,9999", 3, "multiple large numbers ignored"},
        BasicAdditionData{"1000", 1000, "exactly 1000 alone"},
        BasicAdditionData{"1001", 0, "exactly 1001 ignored"},
        BasicAdditionData{"0,5,0", 5, "zero values included"}
    )
);

// Parameterized test for negative number validation
class NegativeNumberTest : public ParameterizedCalculatorTest<InvalidInputData> {};

TEST_P(NegativeNumberTest, ThrowsOnNegativeNumbers) {
    const auto& data = GetParam();

    try {
        calculator.Add(data.input);
        FAIL() << "Expected std::invalid_argument for: " << data.description;
    } catch (const std::invalid_argument& e) {
        std::string message = e.what();
        EXPECT_TRUE(message.find("negatives not allowed") != std::string::npos)
            << "Message should contain 'negatives not allowed' for: " << data.description;
        EXPECT_TRUE(message.find(data.expectedMessage) != std::string::npos)
            << "Message should contain '" << data.expectedMessage << "' for: " << data.description;
    }
}

INSTANTIATE_TEST_SUITE_P(
    NegativeValidation,
    NegativeNumberTest,
    ::testing::Values(
        InvalidInputData{"-1", "-1", "single negative number"},
        InvalidInputData{"1,-2", "-2", "negative in middle"},
        InvalidInputData{"1,-2,-3,4", "-2", "multiple negatives (check first)"},
        InvalidInputData{"-5,-10", "-5", "multiple negatives only"}
    )
);

// Integration and edge case tests (keep as regular tests for specific scenarios)
TEST_F(StringCalculatorTest, ComplexScenarioIntegrationTest) {
    EXPECT_EQ(6, calculator.Add("//[***]\n1***2***3***1001"));
    EXPECT_EQ(15, calculator.Add("1\n2,3\n4,5"));
}
