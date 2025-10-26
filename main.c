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

int findMin(int arr[]) {
    int min = arr[0];
    for (int i = 1; i < SIZE; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

void printResults(int arr[], int max, int min,int sum, float average) {
    printf("배열 분석 결과\n");

    // 원래 배열 출력
    printf("1. 입력된 배열: [ ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arr[i]);
    }
    printf("]\n");

    // 분석 결과 출력
    printf("2. 최댓값 (팀원 2): %d\n", max);
    printf("3. 최솟값 (팀원 3): %d\n", min);
    printf("4. 합계   (팀원 4): %d\n", sum);
    printf("5. 평균   (팀원 4): %.2f\n", average);
}

int findSum(int arr[]) {
    int sum = 0;

    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    return sum;
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

