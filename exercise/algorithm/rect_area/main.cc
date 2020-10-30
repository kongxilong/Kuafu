#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
#define INF 0x3f3f3f3f
#define MAX 110
#define LL rt * 2
#define RR rt * 2 + 1

struct segment {
  double l, r, h;
  int f; //-1: bootm 1:top
} ss[2 * MAX];

struct node //
{
  int l, r;
  int cover;  //
  double len; //
} tt[2 * MAX * 4];
double pos[2 * MAX];
int nums;
int cmp(struct segment a, struct segment b) { return a.h < b.h; }

void build(int a, int b, int rt) {
  tt[rt].l = a;
  tt[rt].r = b;
  tt[rt].cover = 0;
  tt[rt].len = 0;
  if (a == b)
    return;
  int mid = (tt[rt].l + tt[rt].r) / 2;
  build(a, mid, LL);
  build(mid + 1, b, RR);
}

int binary(double key, int low, int high) {
  while (low <= high){
    int mid = (low + high) >> 1;
    if (pos[mid] == key)
      return mid;
    else if (key < pos[mid])
      high = mid - 1;
    else
      low = mid + 1;
  }
  return -1;
}

void pushup(int rt) {
  if (tt[rt].cover) //
    tt[rt].len = pos[tt[rt].r + 1] - pos[tt[rt].l];
  else if (tt[rt].l == tt[rt].r) // not an segment
    tt[rt].len = 0;
  else // is a segment
    tt[rt].len = tt[LL].len + tt[RR].len;
}

void update(int a, int b, int val, int rt) {
  if (tt[rt].l == a && tt[rt].r == b) // target area
  {
    tt[rt].cover += val;
    pushup(rt);
    return;
  }
  int mid = (tt[rt].l + tt[rt].r) / 2;
  if (b <= mid) // only visit left child
    update(a, b, val, LL);
  else if (a > mid) // only visit right child
    update(a, b, val, RR);
  else // visit both
  {
    update(a, mid, val, LL);
    update(mid + 1, b, val, RR);
  }
  pushup(rt); //
}

int main() {
  int Case = 0;
  int n;
  double x1, y1, x2, y2;
  nums = 0;
  vector<vector<int>> input{{0, 0, 2, 2}, {1, 1, 4, 4}};
  for (int i = 0; i < 2; i++) {
    ss[nums].l = input[i][0];
    ss[nums].r = input[i][2];
    ss[nums].h = input[i][3];
    ss[nums].f = 1;
    ss[nums + 1].l = input[i][0]; // record bootm info
    ss[nums + 1].r = input[i][2];
    ss[nums + 1].h = input[i][1];
    ss[nums + 1].f = -1;
    pos[nums] = input[i][0];
    pos[nums + 1] = input[i][2];
    nums += 2;
  }
  sort(ss, ss + nums, cmp); //
  sort(pos, pos + nums);    //
  int m = 1;
  for (int i = 1; i < nums; i++)
    if (pos[i] != pos[i - 1]) // unique for split
      pos[m++] = pos[i];

  build(0, m - 1, 1); // split area [0,m-1]
  double ans = 0;
  for (int i = 0; i < nums; i++) //
  {
    int l = binary(ss[i].l, 0, m - 1);
    int r = binary(ss[i].r, 0, m - 1) - 1;
    update(l, r, ss[i].f, 1);                   //
                                                //
    ans += (ss[i + 1].h - ss[i].h) * tt[1].len; //
  }
  printf("Test case #%d\n", ++Case);
  printf("Total explored area: %.2f\n\n", ans);
  return 0;
}
