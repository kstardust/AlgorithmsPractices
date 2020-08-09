#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef struct range_tuple {
  size_t left;
  size_t right;
} range_tuple;

int max_subarray_sum(int* array, int left, int right, range_tuple* range);
int _max_cross_sum(int* array, int left, int mid, int right, range_tuple* range);

int
main(int argc, char **argv)
{
  int nums[MAX] = {0};
  int *np = nums;
  range_tuple range;
  while (scanf("%d", np++) != EOF);
  int count = np - 1 - nums;
  np = nums;
  int sum = max_subarray_sum(nums, 0, count-1, &range);

  printf("max sum: %d, from index %zu to %zu\n", sum, range.left, range.right);
  exit(0);
}

int
max_subarray_sum(int *array, int left, int right, range_tuple* range)
{
  if ( left == right ) {
    return array[left];
    range->left = left;
    range->right = right;
  }
  int mid = (right + left) / 2;
  range_tuple rl, rr, rc;
  int max_left = max_subarray_sum(array, left, mid, &rl);
  int max_right = max_subarray_sum(array, mid+1, right, &rr);
  int max_cross = _max_cross_sum(array, left, mid, right, &rc);
  int max = 0;
  if (max_right > max_left && max_right > max_cross) {
    max = max_right;
    *range = rr;
  } else if (max_left > max_right && max_left > max_cross) {
    max = max_left;
    *range = rl;
  } else {
    max = max_cross;
    *range = rc;
  }
  return max;
}

int
_max_cross_sum(int *array, int left, int mid, int right, range_tuple* range)
{
  int max_l = 0;
  int max_r = 0;
  int sum = 0;
  int il, ir;
  
  for (int i = mid; i >= left; i--) {
    sum += array[i];
    if (sum > max_l) {
      max_l = sum;
      il = i;
    }
  }

  sum = 0;
  for (int i = mid+1; i <= right; i++) {
    sum += array[i];
    if (sum > max_r) {
      max_r = sum;
      ir = i;
    }
  }
  range->left = il;
  range->right = ir;
  return max_r + max_l;
}
