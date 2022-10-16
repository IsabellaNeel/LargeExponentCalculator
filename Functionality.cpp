#include <array>
#include <iostream>
#include <fstream>
#include <emscripten/bind.h>
using namespace emscripten;


/*return a number in reverse order of num*/
int reverse(int num) {
  int revNum = 0;
  while (num != 0) {
    revNum = revNum * 10 + (num % 10);
    num = num / 10;
  }
  return revNum;
}



std::string mainFunction(int num) {
  int temp_ans_size = 0, num_length = 0, i = 0, j = 0, itr = 0,
           ans_size = 0, num_temp = 0, revNum, carry = 0, num_arr[5],
           ans_arr[500000], temp_ans_arr[500000];

  std::string str;

  revNum = reverse(num);

  /* acounts for zero and digits to the power of 1*/
  if (revNum == 1) {
    str.append(std::to_string(num));
    return 0;
  } else if (num == 0) {
    str.append("1");
    return 0;
  }

  /*turning num into an array of its digits, it's last digit is at num_arr[0]*/
  while (num != 0) {
    num_arr[num_length] = num % 10;
    temp_ans_arr[num_length] = num % 10;
    num = num / 10;
    num_length++;
  }
  ans_size = num_length;
  temp_ans_size = num_length;

  /*outer for loop for exponential mutiplication*/
  for (i = 0; i < revNum - 1; i++) {
    if (i % 2 == 0) { /*alternates between multiplying num by one array
                       * and then multiplying num by the previous result*/
      carry = 0;
      ans_size = 0;
      /*inner loop multiplies of each digit of num
       * by each digit of the current ans_arr */
      for (itr = 0; itr < num_length; itr++) {
        carry = 0;
        /*inner inner loop for each digit of curent ans_arr*/
        for (j = 0; j < temp_ans_size; j++) {

          /*accounts for possible junk in new array spots*/
          if (itr + j >= ans_size) {
            num_temp = (num_arr[itr] * temp_ans_arr[j]);
            carry = num_temp / 10;
            ans_arr[itr + j + 1] = carry;
            ans_arr[itr + j] = (num_temp % 10);

            /*correcting the size*/
            if (carry != 0 && j + itr + 2 > ans_size) {
              ans_size = j + itr + 2;
            } else if (j + itr + 1 > ans_size) {
              ans_size = j + itr + 1;
            }

          } else if (itr + j < ans_size) {
            num_temp = (num_arr[itr] * temp_ans_arr[j]) + ans_arr[j + itr];
            carry = num_temp / 10;
            ans_arr[j + itr] = (num_temp % 10);
            if (itr + j + 1 < ans_size) {
              ans_arr[itr + j + 1] = carry + ans_arr[itr + j + 1];
            } else {
              ans_arr[itr + j + 1] = carry;
            }

            /*correcting the size*/
            if (carry != 0 && j + itr + 2 > ans_size) {
              ans_size = j + itr + 2;
            } else if (j + itr + 1 > ans_size) {
              ans_size = j + itr + 1;
            }
          }
        }
      }
    } else {
      carry = 0;
      temp_ans_size = 0;
      /*inner loop multiplies each digit of num
       * by each digit of the current ans_arr */
      for (itr = 0; itr < num_length; itr++) {
        carry = 0;
        /*inner inner loop for each digit of curent ans_arr*/
        for (j = 0; j < ans_size; j++) {

          /*accounts for possible junk in new array spots*/
          if (itr + j >= temp_ans_size) {
            num_temp = (num_arr[itr] * ans_arr[j]);
            carry = num_temp / 10;
            temp_ans_arr[itr + j + 1] = carry;
            temp_ans_arr[itr + j] = (num_temp % 10);

            /*correcting the size*/
            if (carry != 0 && j + itr + 2 > temp_ans_size) {
              temp_ans_size = j + itr + 2;
            } else if (j + itr + 1 > temp_ans_size) {
              temp_ans_size = j + itr + 1;
            }

          } else if (itr + j < temp_ans_size) {
            num_temp = (num_arr[itr] * ans_arr[j]) + temp_ans_arr[j + itr];
            carry = num_temp / 10;
            temp_ans_arr[j + itr] = (num_temp % 10);
            if (itr + j + 1 < temp_ans_size) {
              temp_ans_arr[itr + j + 1] = carry + temp_ans_arr[itr + j + 1];
            } else {
              temp_ans_arr[itr + j + 1] = carry;
            }

            /*correcting the size*/
            if (carry != 0 && j + itr + 2 > temp_ans_size) {
              temp_ans_size = j + itr + 2;
            } else if (j + itr + 1 > temp_ans_size) {
              temp_ans_size = j + itr + 1;
            }
          }
        }
      }
    }
  }

  /*prints out number*/

  if (i % 2 == 0) {
    for (i = temp_ans_size - 1; i >= 0; i--) {
       str.append(std::to_string(temp_ans_arr[i]));
    }
  } else {
    for (i = ans_size - 1; i >= 0; i--) {
      str.append(std::to_string(ans_arr[i]));
    }
  }
 
  return str;
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("mainFunction", &mainFunction);
}

int main(){
 std::cout << mainFunction(24);
 std::cout << "\n";
}