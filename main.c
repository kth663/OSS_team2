#incldue<stdio.h>

#define SIZE 5

void inputArray(int arr[]){
 printf("정수 5개를 입력하세요:\n");
 for(int i=0;i<SIZE; i++){
  printf("%d번째 숫자:",i+1);
  scanf_s("%d",&arr[i]);
  }
}

int findMax(int arr[]) {
    int max = arr[0];
    for (int i = 1; i < SIZE; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

int main(void){
 int numbers[SIZE];
 int maxResult,minResult;
 int sum;
 float average;

 inputArray(numbers);

 maxResult=findMax(numbers);

 minResult=findMin(numbers);

 sum=findSum(numbers);
 average=(float)sum/SIZE;

printfResults(numbers,maxResult,minResult,sum,average);
 
  return 0;
}

int findSum(int arr[]) {
    int sum = 0;

    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    return sum;
}
