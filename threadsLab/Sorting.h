#ifndef __SORTING
#define __SORTING

template <typename T>
void swap(T *p_val1, T *p_val2)
{
	// input check
	if (p_val1 == nullptr || p_val2 == nullptr)
	{
		return;
	} // end if

	T p_temp = *p_val1;
	*p_val1 = *p_val2;
	*p_val2 = p_temp;
} // end method swap

template <typename T>
void quicksort(T* p_arr, const int i_left, const int i_right)
{
	// Sort left, center, and right
	if (!(p_arr[i_left] <= p_arr[(i_right + i_left) / 2])) // check if left and center must be swapped
	{
		swap(p_arr[i_left], p_arr[(i_right + i_left) / 2]);
	} // end if

	  // center is now greater than left

	if (!(p_arr[(i_right + i_left) / 2] <= p_arr[i_right])) // check if center and right must be swapped
	{
		swap(p_arr[i_right], p_arr[(i_right + i_left) / 2]);
	} // end if

	  // center is now less than right

	if (!(p_arr[i_left] <= p_arr[(i_right + i_left) / 2]))  // check if left and center must be swapped again
	{													    // as center could have a new value after previous swaps
		swap(p_arr[i_left], p_arr[(i_right + i_left) / 2]);
	} // end if


	  // Variables
	int i = i_left,
		j = i_right; // indeces for iterating array

	T	p_pivot = p_arr[(i_left + i_right) / 2]; // set pivot to center


	while (i <= j)
	{
		while (p_arr[i] < p_pivot)
		{
			i++;
		} // end while (arr[i] < pivot)

		while (p_arr[j] > p_pivot)
		{
			j--;
		} // end while (arr[j] > pivot)

		if (i <= j) // make swap 
		{
			swap(p_arr[i], p_arr[j]);
			i++;
			j--;
		} // end if (i <= j)
	} // end while (i <= j)

	if (i_left < j)
	{
		quicksort(p_arr, i_left, j);  // sort left half
	} // end if (left < j)
	if (i < i_right)
	{
		quicksort(p_arr, i, i_right); // sort right half
	} // end if (i < right)

} // end method quicksort

template <typename T>
void bubbleSort(T* p_arr, const int i_left, const int i_right)
{
	for (int i = i_left; i < i_right - 1; i++)
	{
		for (int j = i_left; j < i_right - i - 1; j++)
		{
			if (p_arr[j] > p_arr[j + 1])
			{
				swap(&p_arr[j], &p_arr[j + 1]);
			} // end if
		} // end for j
	} // end for i
} // end bubbleSort

template <typename T>
void insertionSort(T* p_arr, const int i_left, const int i_right)
{
	T key;
	int j;
	for (int i = i_left; i < i_right; i++)
	{
		key = p_arr[i];
		j = i - 1;

		while (j >= i_left && p_arr[j] > key)
		{
			p_arr[j + 1] = p_arr[j];
			j = j - 1;
		} // end while

		p_arr[j + 1] = key;
	} // end for
} // end method insertionSort

#endif