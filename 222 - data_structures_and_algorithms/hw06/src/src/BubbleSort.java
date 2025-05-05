public class BubbleSort extends SortAlgorithm {

	public BubbleSort(int input_array[]) {
		super(input_array);
	}
	
    @Override
    public void sort() {
    	// fill this method
	    int i, j, size = arr.length;
		boolean isOrdered;  //to check if the array is already sorted

	    //iterating through all elements
		for (i = 0; i < size-1; i++) {
			isOrdered = true;
			for (j = 0; j < size-i-1; j++) {
				++comparison_counter;
				//if nex element is smaller than the current one, swap them
				if (arr[j+1] < arr[j]) {
					swap(j, j+1);
					isOrdered = false;  //set the flag false, if a swap happens
				}
			}

			//if the array is already sorted, exit the loop
			if (isOrdered) {
				break;
			}
		}
    }
    
    @Override
    public void print() {
    	System.out.print("Bubble Sort\t=>\t");
    	super.print();
    }
}
