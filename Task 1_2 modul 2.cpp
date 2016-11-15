#include  <cstdio>
#include <iostream>

using namespace std;

struct Point
{
	int x;
	int y;
};

void insertion_sort(Point* arr, int n)
{
	int place = 0;
	//bool flag = 0;
	for (int i = 0; i < n; i++)
	{
		place = i;
		Point temp = arr[i];
		for (int j = i; j > 0 && (temp.x < arr[j - 1].x || (temp.x == arr[j - 1].x && temp.y < arr[j - 1].y)); j--)
		{
			arr[j] = arr[j - 1];
			place = j-1;
		}
		arr[place] = temp;
	}
}

int main()
{
	int n;
	scanf("%d", &n);
	Point* arr = new Point[n];
	for (int i = 0; i < n; i++)
	{
		scanf("%d%d", &arr[i].x, &arr[i].y);
	}
	insertion_sort(arr, n);
	for (int i = 0; i < n; i++)
	{
		printf("%d %d\n", arr[i].x, arr[i].y);
	}
	system("pause");

	return 0;
}
