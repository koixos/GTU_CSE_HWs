public class MergeSort extends SortAlgorithm {
	
	public MergeSort(int input_array[]) {
		super(input_array);
	}
	
	private void merge(int left, int right, int mid){
        // fill this method
		int i ,j, k = left;

		int size1 = mid-left+1;
		int size2 = right-mid;

		//temp arrays for sub-arrays
		int[] leftArr = new int[size1];
		int[] rightArr = new int[size2];

		//copying data to the temp arrays
		for (i = 0; i < size1; i++) {
			leftArr[i] = arr[left+i];
		}

		for (j = 0; j < size2; j++) {
			rightArr[j] = arr[mid+j+1];
		}

		//merging the temp arrays
		i = 0;
		j = 0;
		while (i < size1 && j < size2) {
			++comparison_counter;
			if (leftArr[i] <= rightArr[j]) {
				arr[k] = leftArr[i];
				++i;
			} else {
				arr[k] = rightArr[j];
				++j;
			}
			++k;
		}

		//copying remaining elements of the left-side array
		while (i < size1) {
			arr[k] = leftArr[i];
			++i;
			++k;
		}

		//copying remaining elements of the right-side array
		while (j < size2) {
			arr[k] = rightArr[j];
			++j;
			++k;
		}
    }

    private void sort(int left, int right){
        // fill this method
	    if (left < right) {
			int mid = left + (right-left) / 2;

			//sort right and left halves
		    sort(left, mid);
			sort(mid+1, right);

			merge(left, right, mid);
	    }
    }
    
    @Override
    public void sort() {
    	sort(0, arr.length-1);
    }
    
    @Override
    public void print() {
    	System.out.print("Merge Sort\t=>\t");
    	super.print();
    }
}
