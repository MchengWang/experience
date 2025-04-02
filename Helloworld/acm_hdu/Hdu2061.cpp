#include <iostream>
#include <vector>
using namespace std;
 
class student {
public:
  char name[31];
  float num;
  float score;
};
 
int main() {
  int N;
  cin >> N;
  while (N--) {
    int n;
    cin >> n;
    vector<student> sum(n);
    bool flag = false;
    for (int i = 0; i < n; i++) {
      scanf("%s %f %f", sum[i].name, &sum[i].num, &sum[i].score);
      if (sum[i].score < 60)
        flag = true;
    }
    if (flag)
      printf("Sorry!\n");
    else {
      float a = 0, b = 0;
      for (auto m : sum) {
        a += m.score * m.num;
        b += m.num;
      }
      printf("%.2f\n", a / b);
    }
  }
  return 0;
}