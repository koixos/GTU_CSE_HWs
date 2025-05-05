public class QuickSort extends SortAlgorithm {

	public QuickSort(int input_array[]) {
		super(input_array);
	}
	
    private int partition(int left, int right){
        // fill this method
	    int pivot = arr[right];
		int j = left - 1, i;

		//iterate through all elements except the pivot
		for (i = left; i < right; i++) {
			++comparison_counter;
			//if current element is smaller than pivot, swap it
			if (arr[i] < pivot) {
				j++;
				swap(i,j);
			}
		}

		//move the pivot to its correct position
		swap(j+1, right);
		return j+1; //returning index of pivot
    }

    private void sort(int left, int right){
        // fill this method
	    if (left < right) {
			int ind = partition(left, right);
			sort(left, ind-1);
			sort(ind+1, right);
	    }
    }

    @Override
    public void sort() {
    	sort(0, arr.length-1);
    }

    @Override
    public void print() {
    	System.out.print("Quick Sort\t=>\t");
    	super.print();
    }
}
