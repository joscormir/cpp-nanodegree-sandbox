#include <cassert>
#include <cstring>
#include <iostream>
#include <string>
#define LOG(x) std::cout << x << std::endl
#define DATE(dd, mm, yy) std::cout << dd << "/" << mm << "/" << yy << std::endl

struct Date {
  int Day() { return day; }
  int Month() { return month; }
  int Year() { return year; }
  void setDay(int _day) { day = _day; }
  void setMonth(int _month) { month = _month; }
  void setYear(int _year) { year = _year; }

private:
  int day{1};
  int month{1};
  int year{0};
};

class dateClass {
private:
  int day{1};
  int month{1};
  int year{1};

public:
  dateClass();
  dateClass(int _day, int _month, int _year) {
    day = _day;
    month = _month;
    year = _year;
  };
  int Day() { return day; }
  int Month() { return month; }
  int Year() { return year; }
};

class Car {
private:
  int horsepower;
  float weight;
  char brand[10];

public:
  Car();
  ~Car();
  void setbrand(std::string _brand);
  std::string getBrand() const;
};

Car::Car() {}
Car::~Car() {}
void Car::setbrand(std::string _brand) {
  char *cstr = new char[_brand.length() + 1];
  std::strcpy(cstr, _brand.c_str());
    int i=0;
    while(*cstr != '\0'){
        brand[i] = *cstr;
        cstr++;
        i++;
    }
};
std::string Car::getBrand() const {
  std::string parsedBrand(brand);
  return parsedBrand;
};



int main() {

  dateClass date(10, 10, 1990);
  DATE(date.Day(), date.Month(), date.Year());
  Car coche;
  coche.setbrand("");
  std::cout << coche.getBrand();
}
