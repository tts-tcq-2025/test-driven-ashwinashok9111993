#include "StringCalculator.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <numeric>

int StringCalculator::Add(const std::string& numbers) {
    if (numbers.empty()) {
        return 0;
    }

    std::string delimiter = parseDelimiter(numbers);
    std::string numbersPart = extractNumbers(numbers);
    std::string normalizedNumbers = normalizeDelimiters(numbersPart, delimiter);
    std::vector<int> nums = parseNumbers(normalizedNumbers);

    validateNumbers(nums);

    return calculateSum(nums);
}

std::string StringCalculator::parseDelimiter(const std::string& numbers) {
    if (!hasCustomDelimiterFormat(numbers)) {
        return ",";
    }

    size_t newlinePos = numbers.find('\n');
    if (newlinePos == std::string::npos) {
        return ",";
    }

    return extractDelimiterFromHeader(numbers.substr(2, newlinePos - 2));
}

bool StringCalculator::hasCustomDelimiterFormat(const std::string& numbers) {
    return numbers.length() >= 4 && numbers.substr(0, 2) == "//";
}

std::string StringCalculator::extractDelimiterFromHeader(const std::string& delimiterPart) {
    if (hasBracketFormat(delimiterPart)) {
        return delimiterPart.substr(1, delimiterPart.length() - 2);
    }

    return delimiterPart;
}

bool StringCalculator::hasBracketFormat(const std::string& delimiterPart) {
    return delimiterPart.length() >= 2 &&
           delimiterPart[0] == '[' &&
           delimiterPart[delimiterPart.length() - 1] == ']';
}

std::string StringCalculator::extractNumbers(const std::string& numbers) {
    if (!hasCustomDelimiterFormat(numbers)) {
        return numbers;
    }

    return extractNumbersAfterDelimiterHeader(numbers);
}

std::string StringCalculator::extractNumbersAfterDelimiterHeader(const std::string& numbers) {
    size_t newlinePos = numbers.find('\n');
    if (newlinePos == std::string::npos || newlinePos + 1 >= numbers.length()) {
        return numbers;
    }

    return numbers.substr(newlinePos + 1);
}

std::string StringCalculator::normalizeDelimiters(const std::string& numbersPart,
                                                   const std::string& delimiter) {
    std::string result = numbersPart;

    if (delimiter != ",") {
        result = replaceDelimiterWithComma(result, delimiter);
    }

    std::replace(result.begin(), result.end(), '\n', ',');

    return result;
}

std::string StringCalculator::replaceDelimiterWithComma(const std::string& text,
                                                        const std::string& delimiter) {
    std::string escapedDelimiter = delimiter;
    std::regex specialChars(R"([-[\]{}()*+?.,\^$|#\s])");
    escapedDelimiter = std::regex_replace(escapedDelimiter, specialChars, R"(\$&)");

    std::regex delimiterRegex(escapedDelimiter);
    return std::regex_replace(text, delimiterRegex, ",");
}

std::vector<int> StringCalculator::parseNumbers(const std::string& normalizedNumbers) {
    std::vector<int> nums;
    std::stringstream ss(normalizedNumbers);
    std::string num;

    while (std::getline(ss, num, ',')) {
        if (!num.empty()) {
            nums.push_back(std::stoi(num));
        }
    }

    return nums;
}

int StringCalculator::calculateSum(const std::vector<int>& nums) {
    return std::accumulate(nums.begin(), nums.end(), 0, [](int sum, int n) {
        return n <= 1000 ? sum + n : sum;
    });
}

void StringCalculator::validateNumbers(const std::vector<int>& nums) {
    std::vector<int> negatives = findNegativeNumbers(nums);

    if (!negatives.empty()) {
        std::string message = buildNegativeNumbersMessage(negatives);
        throw std::invalid_argument(message);
    }
}

std::vector<int> StringCalculator::findNegativeNumbers(const std::vector<int>& nums) {
    std::vector<int> negatives;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(negatives),
                 [](int num) { return num < 0; });
    return negatives;
}

std::string StringCalculator::buildNegativeNumbersMessage(const std::vector<int>& negatives) {
    std::string message = "negatives not allowed: ";
    for (size_t i = 0; i < negatives.size(); ++i) {
        if (i > 0) message += ", ";
        message += std::to_string(negatives[i]);
    }
    return message;
}
