public class SelectionSort extends SortAlgorithm {

	public SelectionSort(int input_array[]) {
		super(input_array);
	}

    @Override
    public void sort() {
        // fill this method
	    int i, j, min, size = arr.length;

		//iterating through each element
		for (i = 0; i < size-1; i++) {
			min = i;    //assuming the current element is the minimum
			//iterating through the unsorted elements
			for (j = i+1; j < size; j++) {
				++comparison_counter;
				//finding the minimum
				if (arr[j] < arr[min]) {
					min = j;
				}
			}
			//swap the found minimum with the assumed one
			swap(i, min);
		}
    }

    @Override
    public void print() {
    	System.out.print("Selection Sort\t=>\t");
    	super.print();
    }
}
