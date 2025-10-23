#incldue<stdio.h>

#define SIZE 5

int main(void){


  return 0;
};

int findSum(int arr[]) {
    int sum = 0;

    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    return sum;
}
