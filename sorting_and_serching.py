def insertion_sort(array):
    size = len(array)
    for i in range(1, size):
        temp = array[i]
        j = i-1
        while j>=0 and array[j]>temp:
            array[j+1] = array[j]
            j-=1
        array[j+1] = temp
    return array

def bubble_sort(array):
    size = len(array)
    for i in range(size-1):
        for j in range(size-1):
            if array[j] > array[j+1]:
                array[j], array[j+1] = array[j+1], array[j]
    return array
    
def selection_sort(array):
    size = len(array)
    for i in range(size - 1):
        min_idx = i
        for j in range(i+1, size):
            if array[j] < array[min_idx]:
                min_idx = j
        array[i], array[min_idx] = array[min_idx], array[i]
    return array
    
def quick_sort(array, left, right):
    if left < right:
        pi = partition(array, left, right)
        quick_sort(array, left, pi-1)
        quick_sort(array, pi+1, right)

def partition(array, left, right):
    pivot = array[right]
    i = left - 1
    for j in range(left, right):
        if array[j] < pivot:
            i+=1
            array[i], array[j] = array[j], array[i]
    array[i+1], array[right] = array[right], array[i+1]
    return i+1
    
def shell_sort(array):
    n = len(array)
    gap = n//2
    while gap > 0:
        for i in range(gap, n):
            temp = array[i]
            j = i
            while j >= gap and array[j-gap] > temp:
                array[j] = array[j - gap]
                j -= gap
            array[j] = temp
        gap //=2
    return array
    
def linear_search(array, target):
    for i in range(len(array)):
        if array[i] == target:
            return i
    return -1
    
def binary_search(array, target):
    low = 0
    high = len(array) - 1
    while low<= high:
        mid = (low+high)//2
        if array[mid] == target:
            return mid
        elif (array[mid] > target):
            high = mid -1
        else:
            low = mid + 1
    return -1
    
def sentinel_search(array, target):
    last = len(array) -1
    temp = array[last]
    array[last] = target
    i = 0
    while i<len(array) and array[i] != target:
        i += 1
    array[last] = temp
    if i<last or temp == target:
        return i
    else:
        return -1

def fibonacci_search(array, target):
    size = len(array)
    fib2 = 0
    fib1 = 1
    fibm = fib2 + fib1
    while fibm < size:
        fib2 = fib1
        fib1 = fibm
        fibm = fib2 + fib1
    offset = -1
    while fibm>1:
        i = min(offset+fib2, size-1)
        if array[i] < target:
            fibm = fib1
            fib1 = fib2
            fib2 = fibm - fib1
            offset = i
        elif array[i] > target:
            fibm = fib2
            fib1 = fib1 - fib2
            fib2 = fibm - fib1
        else: 
            return i
    if fib1 ==1 and array[size-1] == target:
        return size - 1
    return -1
