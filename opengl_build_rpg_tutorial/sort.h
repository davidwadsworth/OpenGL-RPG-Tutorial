#pragma once

/*
TODO

@author David Wadsworth
*/

template<typename T>
class merge_sort
{
	static void merge(std::vector<T>& vec, int l, int m, int r)
	{
		int i, j, k;
		int n1 = m - l + 1;
		int n2 = r - m;

		/* create temp arrays */
		std::vector<T> L, R;

		/* Copy data to temp arrays L[] and R[] */
		for (i = 0; i < n1; i++)
			L.push_back(vec[l + i]);
		for (j = 0; j < n2; j++)
			R.push_back(vec[m + 1 + j]);

		/* Merge the temp arrays back into arr[l..r]*/
		i = 0; // Initial index of first subarray 
		j = 0; // Initial index of second subarray 
		k = l; // Initial index of merged subarray 
		while (i < n1 && j < n2)
		{
			if (L[i] <= R[j])
			{
				vec[k] = L[i];
				i++;
			}
			else
			{
				vec[k] = R[j];
				j++;
			}
			k++;
		}

		/* Copy the remaining elements of L[], if there
		   are any */
		while (i < n1)
		{
			vec[k] = L[i];
			i++;
			k++;
		}

		/* Copy the remaining elements of R[], if there
		   are any */
		while (j < n2)
		{
			vec[k] = R[j];
			j++;
			k++;
		}
	}
	static void sort(std::vector<T>& vec, int l, int r)
	{
		if (l < r)
		{
			// Same as (l+r)/2, but avoids overflow for 
			// large l and h 
			int m = l + (r - l) / 2;

			// Sort first and second halves 
			sort(vec, l, m);
			sort(vec, m + 1, r);

			merge(vec, l, m, r);
		}
	}
public:
	static void sort(std::vector<T>& vec)
	{
		sort(vec, 0, vec.size() - 1);
	}
};

template <class T>
static void insertion_sort(std::vector<T>& vec)
{
	int i, j;
	T key;
	for (i = 1; i < vec.size(); i++)
	{
		key = vec[i];
		j = i - 1;

		while (j >= 0 && vec[j] > key)
		{
			vec[j + 1] = vec[j];
			j = j - 1;
		}
		vec[j + 1] = key;
	}
}