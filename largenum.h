#include <iostream>
#include <vector>

using namespace std;

class LargeNum {
  // output number with a comma after ever 3 digits,
  // e.g. 1234567890 -> 1,234,567,890
  friend ostream &operator<<(ostream &out, const LargeNum &num);

private:
  // Define private data members and methods here
  vector<int> digits; // the digits of the number, stored in reverse order
  bool isPositive;    // true if the number is positive, false if it is negative

  // remove leading zeros from the digits
  void removeLeadingZeros();

  // addition of two vectors of digits
  static vector<int> addDigits(const vector<int> &lhs, const vector<int> &rhs);

  // subtraction of two vectors of digits
  vector<int> subDigits(const vector<int> &lhs, const vector<int> &rhs) const;

public:
  // getter for isPositive
  bool getIsPositive() const;
  
  // setter for isPositive
  void setIsPositive(bool value);

  // getters for digits
  vector<int> getDigits() const;

  // setter for digits
  void setDigits(const vector<int> &newDigits);

  // returns absolute value
  LargeNum abs() const;

  // constructor from string
  explicit LargeNum(const string &str = "0");

  // constructor from int
  explicit LargeNum(int anInteger);

  // use the default copy constructor
  LargeNum(const LargeNum &other) = default;

  // use the default copy assignment operator
  LargeNum &operator=(const LargeNum &other) = default;

  // use the default destructor
  ~LargeNum() = default;

  // returns true if the number is zero
  bool isZero() const;

  // negates the number, positive becomes negative, negative becomes positive
  // Zero is always positive
  LargeNum &negate();

  // add two numbers
  LargeNum operator+(const LargeNum &rhs) const;

  // subtract two numbers
  LargeNum operator-(const LargeNum &rhs) const;

  // multiply two numbers
  LargeNum operator*(const LargeNum &rhs) const;

  // divide two numbers. rhs is the divisor
  // similar to integer division, ignore remainder
  LargeNum operator/(const LargeNum &rhs) const;

  // return true if the numbers are equal
  bool operator==(const LargeNum &rhs) const;

  // return true if the numbers are not equal
  bool operator!=(const LargeNum &rhs) const;

  // return true if the left-hand-side number is less than the
  // right-hand-side number
  bool operator<(const LargeNum &rhs) const;

  // return true if the left-hand-side number is greater than the
  // right-hand-side number
  bool operator>(const LargeNum &rhs) const;

  // return true if the left-hand-side number is less than or equal to the
  // right-hand-side number
  bool operator<=(const LargeNum &rhs) const;

  // return true if the left-hand-side number is greater than or equal to the
  // right-hand-side number
  bool operator>=(const LargeNum &rhs) const;

  // prefix increment
  LargeNum &operator++();

  // postfix increment
  LargeNum operator++(int);

  // prefix decrement
  LargeNum &operator--();

  // postfix decrement
  LargeNum operator--(int);
};