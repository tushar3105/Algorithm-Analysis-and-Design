#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int n, a[200005];
struct lr
{
	int l;
	int r;
};
void merge(int l, int r)
{
	int x, sz1, sz2;
	x=(l+r)/2;
	sz1=(x+1-l);
	sz2=(r-x);
	int *left=(int*)malloc((sz1)*sizeof(int));
	int *right=(int*)malloc((sz2)*sizeof(int));
	for(int i=l;i<=x;++i)
		left[i-l]=a[i];
	for(int i=x+1;i<=r;++i)
		right[i-(x+1)]=a[i];
	int i=0;
	int j=0;
	int y=r-(x+1);
	int tm=0;
	while(x-l>=i && j<=(r-x-1))
	{
		if(left[i]>right[j])
		{
			a[l+tm]=right[j];
			j+=1;
			tm+=1;
		}
		else
		{
			a[l+tm]=left[i];
			i+=1;
			tm+=1;
		}
	}
	while(i<=x-l)
	{
		a[l+tm]=left[i];
		i+=1;
		tm+=1;
	}
	while(j<=y)
	{
		a[l+tm]=right[j];
		j+=1;
		tm+=1;
	}
	free(left);
	free(right);
}
void sele(int l, int r)
{
	int sm, flag, tmp;
	for(int i=l;i<=r;i++)
	{
		sm=i;
		flag=0;
		for(int j=i+1;j<=r;j++)
		{
			if(a[sm]>a[j])
			{
				sm=j;
				flag++;
			}
		}

		if (flag)
		{
			tmp=a[sm];
			a[sm]=a[i];
			a[i]=tmp;
			flag=0;
		}
	}
}
void *merge_sort(void* data)
{
	int l, r;
	struct lr *dt=(struct lr*)data;
	l=dt->l;
	r=dt->r;
	if(l<r && r+1-l<=5)
	{
		sele(l,r);
                return 0;
	}
	int mid;
	struct lr *st=(struct lr*)malloc(sizeof(struct lr));
	struct lr *st2=(struct lr*)malloc(sizeof(struct lr));
	mid=(l+r)/2;
	st->l=l;
	st->r=mid;
	st2->l=mid+1;
	st2->r=r;
	if(l>=r)
		return 0;
	else
	{
		pthread_t stid1;
		pthread_create(&stid1, NULL, merge_sort, st);
		pthread_t stid2;
		pthread_create(&stid2, NULL, merge_sort, st2);
		pthread_join(stid1, NULL);
		pthread_join(stid2, NULL);
		merge(l,r);
	}
	free(dt);
	return 0;
}
int main()
{
	clock_t begin = clock();
	int n;
	scanf("%d",&n);
	for(int i=0;i<n;++i)
	{
		scanf("%d",&a[i]);
	}
	struct lr *st=(struct lr*)malloc(sizeof(struct lr));
	st->r=n-1;
	st->l=0;
	pthread_t stid;
	pthread_create(&stid, NULL, merge_sort, st);
	pthread_join(stid, NULL);
	for(int i=0;i<n;i++)
		printf("%d ",a[i]);
	clock_t end = clock();
    double time_spent = (double)(end - begin)/ (double) CLOCKS_PER_SEC;
	printf("\n%lf\n",time_spent);
}
