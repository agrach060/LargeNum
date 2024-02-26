#include "largenum.h"
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

// Your code goes here

// getter for isPositive
bool LargeNum::getIsPositive() const { return isPositive; }

// getter for digits
vector<int> LargeNum::getDigits() const { return digits; }

// output number with a comma after ever 3 digits,
// e.g. 1234567890 -> 1,234,567,890
ostream &operator<<(ostream &out, const LargeNum &num) {
  const vector<int> &digits = num.getDigits();
  int size = static_cast<int>(digits.size());
  if (digits[size - 1] == -1) {
    out << '-';
    out << digits[size - 2];
    for (int i = size - 3; i >= 0; --i) {
      if ((i + 1) % 3 == 0) {
        out << ',';
      }
      out << digits[i];
    }

  } else {
    out << digits[size - 1];
    for (int i = size - 2; i >= 0; --i) {
      if ((i + 1) % 3 == 0) {
        out << ',';
      }
      out << digits[i];
    }
  }
  return out;
}

// default constructor from string
LargeNum::LargeNum(const string &str) {
  isPositive = (str[0] != '-');
  // Parse the number into digits and store them in reverse order
  for (size_t i = str.length(); i-- > 0;) {
    char digit = str[i];
    if (digit == '-') {
      digits.push_back(-1); // negative sign
      isPositive = false;
    } else if (digit >= '0' && digit <= '9') {
      digits.push_back(digit - '0');
    } else {
      throw invalid_argument("Invalid character in number string.");
    }
  }
}

LargeNum::LargeNum(int anInteger) {
  // Convert the integer to a string and call the string constructor
  string str = to_string(anInteger);
  *this = LargeNum(str);
}

// returns true if the number is zero
bool LargeNum::isZero() const {
  for (int i = 0; i < digits.size(); i++) {
    if (digits[i] != 0) {
      return false;
    }
  }
  return true;
}

// negates the number, positive becomes negative, negative becomes positive
// Zero is always positive
LargeNum &LargeNum::negate() {
  if (digits.size() == 1 && digits[0] == 0) {
    isPositive = true;
    return *this;
  }
  digits.push_back(-1);
  isPositive = !isPositive;
  return *this;
}

// setter for digits
void LargeNum::setDigits(const vector<int> &newDigits) { digits = newDigits; }

// returns absolute value
LargeNum LargeNum::abs() const {
  LargeNum result = *this;
  result.setIsPositive(true); // Make the number positive

  if (!isPositive) {
    result.digits.pop_back(); // Remove the last digit
  }
  return result;
}

// setter for isPositive
void LargeNum::setIsPositive(bool value) { isPositive = value; }

// add two numbers
LargeNum LargeNum::operator+(const LargeNum &rhs) const {
  LargeNum result;
  if (isPositive && rhs.isPositive) {
    result.setDigits(addDigits(digits, rhs.digits));
    result.setIsPositive(true);
  } else if (isPositive && !rhs.isPositive) {
    if (abs() >= rhs.abs()) {
      result.setDigits(subDigits(digits, rhs.digits));
      result.setIsPositive(true);
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(false);
    }
  } else if (!getIsPositive() && rhs.getIsPositive()) {
    if (abs() >= rhs.abs()) {
      result.setDigits(subDigits(digits, rhs.digits));
      result.setIsPositive(false);
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(true);
    }
  } else {
    result.setDigits(addDigits(digits, rhs.digits));
    result.setIsPositive(false);
    result.negate();
  }
  return result;
}

// subtract two numbers
LargeNum LargeNum::operator-(const LargeNum &rhs) const {
  LargeNum result;
  if (isPositive && rhs.isPositive) {
    if (abs() >= rhs.abs()) {
      result.setDigits(subDigits(digits, rhs.digits));
      result.setIsPositive(true);
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(false);
      result.negate();
      result.isPositive = false;
    }
  } else if (isPositive && !rhs.isPositive) {
    if (abs() >= rhs.abs()) {
      result.setDigits(addDigits(digits, rhs.digits));
      result.setIsPositive(true);
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(false);
    }
  } else if (!isPositive && rhs.isPositive) {
    if (abs() >= rhs.abs()) {
      result.setDigits(addDigits(digits, rhs.digits));
      result.setIsPositive(false);
      result.negate();
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(true);
    }
  } else {
    if (abs() >= rhs.abs()) {
      result.setDigits(subDigits(digits, rhs.digits));
      result.setIsPositive(false);
    } else {
      result.setDigits(subDigits(rhs.digits, digits));
      result.setIsPositive(true);
    }
  }
  result.removeLeadingZeros(); // Remove leading zeros
  return result;
}

// multiply two numbers
LargeNum LargeNum::operator*(const LargeNum &rhs) const {
  LargeNum result;
  // If either number is zero, the result is zero
  if (isZero() || rhs.isZero()) {
    return result;
  }
  // Multiply using repeated addition
  LargeNum multiplicand = abs();
  LargeNum multiplier = rhs.abs();
  while (multiplier > LargeNum(0)) {
    result = result + multiplicand;
    multiplier = multiplier - LargeNum(1);
  }
  // Set the sign of the result
  if (!isPositive && !rhs.isPositive) {
    return result;
  }
  if (!isPositive || !rhs.isPositive) {
    return result.negate();
    ;
  }
  return result;
}

// divide two numbers. rhs is the divisor
// similar to integer division, ignore remainder
LargeNum LargeNum::operator/(const LargeNum &rhs) const {
  LargeNum quotient;
  if (rhs.isZero()) {
    // division by zero
    throw runtime_error("Division by zero");
  }
  if (isZero()) {
    // dividend is zero, quotient is zero
    return quotient;
  }
  // compute quotient as repeated subtraction
  LargeNum dividend = abs();
  LargeNum divisor = rhs.abs();
  while (dividend >= divisor) {
    dividend = dividend - divisor;
    quotient = quotient + LargeNum(1);
  }
  // set sign of quotient
  if (!isPositive && !rhs.isPositive) {
    return quotient;
  }
  if (!isPositive || !rhs.isPositive) {
    return quotient.negate();
  }
  return quotient;
}

// return true if the numbers are equal
bool LargeNum::operator==(const LargeNum &rhs) const {
  // if the sizes are different, the numbers can't be equal

  if (digits.size() != rhs.digits.size()) {
    return false;
  }

  // iterate over the digits and compare them
  for (int i = 0; i < digits.size(); i++) {
    if (digits[i] != rhs.digits[i]) {
      return false;
    }
  }

  // if we made it this far, the numbers must be equal
  return true;
}

// return true if the numbers are not equal
bool LargeNum::operator!=(const LargeNum &rhs) const { return !(*this == rhs); }

// return true if the left-hand-side number is less than the
// right-hand-side number
bool LargeNum::operator<(const LargeNum &rhs) const {
  if (isPositive != rhs.isPositive) {
    return !isPositive;
  }
  if (digits.size() != rhs.digits.size()) {
    return (digits.size() < rhs.digits.size()) ^ !isPositive;
  }
  for (size_t i = digits.size(); i-- > 0;) {
    if (digits[i] != rhs.digits[i]) {
      return (digits[i] < rhs.digits[i]) ^ !isPositive;
    }
  }
  return false;
}

// return true if the left-hand-side number is greater than the
// right-hand-side number
bool LargeNum::operator>(const LargeNum &rhs) const { return rhs < *this; }

// return true if the left-hand-side number is less than or equal to the
// right-hand-side number
bool LargeNum::operator<=(const LargeNum &rhs) const {
  if (*this == rhs) {
    return true;
  }
  return (*this < rhs);
}

// return true if the left-hand-side number is greater than or equal to
// the right-hand-side number
bool LargeNum::operator>=(const LargeNum &rhs) const { return !(*this < rhs); }

// prefix increment
LargeNum &LargeNum::operator++() {
  if (isPositive) {
    // increment the number represented by digits
    int carry = 1;
    for (int i = 0; i < digits.size(); ++i) {
      digits[i] += carry;
      carry = digits[i] / 10;
      digits[i] %= 10;
      if (carry == 0) {
        break;
      }
    }
    if (carry != 0) {
      digits.push_back(carry);
    }
  } else {
    // decrement the number represented by digits
    int carry = 1;
    for (int i = 0; i < digits.size(); ++i) {
      digits[i] -= carry;
      if (digits[i] < 0) {
        digits[i] += 10;
        carry = 1;
      } else {
        carry = 0;
        break;
      }
    }
    if (carry != 0) {
      isPositive = true;
      digits[0] = -digits[0];
      for (int i = 1; i < digits.size(); ++i) {
        digits[i] = 9 - digits[i];
      }
      digits.push_back(1);
    }
  }

  if (digits.size() == 2 && digits[1] == -1 && digits[0] == 0) {
    isPositive = true;
    *this = LargeNum(0);
  }
  // remove leading zeros
  while (digits.size() > 1 && digits.back() == 0) {
    digits.pop_back();
  }

  return *this;
}

// postfix increment
LargeNum LargeNum::operator++(int) {
  LargeNum temp(*this);
  ++(*this);
  return temp;
}

// prefix decrement
LargeNum &LargeNum::operator--() {
  *this = *this - LargeNum(1);
  return *this;
}

// postfix decrement
LargeNum LargeNum::operator--(int) {
  LargeNum temp(*this);
  --(*this);
  return temp;
}

//  remove leading zeros from the digits
void LargeNum::removeLeadingZeros() {
  while (digits.size() > 1 && digits.back() == 0) {
    digits.pop_back();
  }
}

// perform addition on two vectors of digits, ignoring signs
vector<int> LargeNum::addDigits(const vector<int> &lhs,
                                const vector<int> &rhs) {
  vector<int> result;
  int carry = 0;
  int index = 0;

  // add digits until we run out of digits in one of the numbers
  while (index < lhs.size() && index < rhs.size() && lhs[index] >= 0 &&
         rhs[index] >= 0) {
    int sum = lhs[index] + rhs[index] + carry;
    result.push_back(sum % 10);
    carry = sum / 10;
    index++;
  }

  // add any remaining digits from lhs

  while (index < lhs.size() && lhs[index] >= 0) {
    int sum = lhs[index] + carry;
    result.push_back(sum % 10);
    carry = sum / 10;
    index++;
  }

  // add any remaining digits from rhs
  while (index < rhs.size() && rhs[index] >= 0) {
    int sum = rhs[index] + carry;
    result.push_back(sum % 10);
    carry = sum / 10;
    index++;
  }

  // if there is a final carry, add it to the result
  if (carry > 0) {
    result.push_back(carry);
  }

  return result;
}

// perform subtraction on two vectors of digits, ignoring signs
// lhs must be greater than or equal to rhs
vector<int> LargeNum::subDigits(const vector<int> &lhs,
                                const vector<int> &rhs) const {
  vector<int> result;
  int borrow = 0;
  int index = 0;

  // subtract digits until we run out of digits in one of the numbers
  while (index < lhs.size() && index < rhs.size()) {
    int diff = lhs[index] - rhs[index] - borrow;
    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result.push_back(diff);
    index++;
  }

  // subtract any remaining digits from lhs
  while (index < lhs.size() && lhs[index] >= 0) {
    int diff = lhs[index] - borrow;
    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }
    result.push_back(diff);
    index++;
  }

  // remove any leading zeros from the result
  LargeNum copy(*this);
  copy.digits = result;
  copy.removeLeadingZeros();

  return copy.digits;
}
