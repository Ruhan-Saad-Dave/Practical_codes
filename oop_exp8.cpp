#include <iostream>
using namespace std;

template<typename T>
void sort(T* const array, int size){
	for(int i=0; i < size-1; i++){
		int minIndex = i;
		for(int j = 1; j < size; j++){
			if(array[j] < array[minIndex])
				minIndex = j;
		}
		if(minIndex != i){
			T temp = array[i];
			array[i] = array[minIndex];
			array[minIndex] = temp;
		}
	}
}

template <typename T>
void display(T* const array, int size){
	for(int i=0; i<size; i++)
		cout << array[i] << " ";
	cout << endl;
}

int main(){
	int sizeOfint, sizeOffloat;
	cout << "Enter size of Integer array:";
	cin >> sizeOfint;
	cout << "Enter elements of array:" << endl;
	int int_Array[sizeOfint];
	for(int i=0; i<sizeOfint; i++)
		cin >> int_Array[i];

	cout << "Enter size of Floatarray:";
	cin >> sizeOffloat;
	cout << "Enter elements of array:" << endl;
	float float_Array[sizeOffloat];
	for(int i=0; i<sizeOffloat; i++)
		cin >> float_Array[i];

	sort(int_Array, sizeOfint);
	sort(float_Array, sizeOffloat);
	
	display(int_Array, sizeOfint);
	display(float_Array, sizeOffloat);
	return 0;
}

