#pragma once
#include <string>
#include <vector>

class StringCalculator {
 public:
    int Add(const std::string& numbers);

 private:
    std::string parseDelimiter(const std::string& numbers);
    bool hasCustomDelimiterFormat(const std::string& numbers);
    std::string extractNumbers(const std::string& numbers);
    std::string extractNumbersAfterDelimiterHeader(const std::string& numbers);
    std::string extractDelimiterFromHeader(const std::string& delimiterPart);
    bool hasBracketFormat(const std::string& delimiterPart);
    std::string normalizeDelimiters(const std::string& numbersPart, const std::string& delimiter);
    std::string replaceDelimiterWithComma(const std::string& text, const std::string& delimiter);
    std::vector<int> parseNumbers(const std::string& normalizedNumbers);
    int calculateSum(const std::vector<int>& nums);
    void validateNumbers(const std::vector<int>& nums);
    std::vector<int> findNegativeNumbers(const std::vector<int>& nums);
    std::string buildNegativeNumbersMessage(const std::vector<int>& negatives);
};
