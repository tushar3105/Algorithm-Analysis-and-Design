#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
int *a;
int n;
struct lr
{
	int l,r;
};
void merge(int l, int r)
{
	int x, sz1, sz2;
	x=(l+r)/2;
	sz1=x-l+1;
	sz2=r-x;

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
	int i, sm, tmp, flag;
	
	for(i=l;i<=r;i++)
	{
		flag=0;
		sm=i;

		for(int j=i+1;j<=r;j++)
		{
			if(a[j]<a[sm])
			{
				flag++;
				sm=j;
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
	if(l<r && r-l<=4)
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
	st2->l=1+mid;
	st2->r=r;
	if(l>=r)
		return 0;
	int x, y;
	x=fork();
	if(x==0)
	{
		merge_sort(st);
		exit(0);
	}
	y=fork();
	if(y==0)
	{
		merge_sort(st2);
		exit(0);
	}
	int wtid=0;
	while(waitpid(x,NULL,0)>=wtid || waitpid(y,NULL,0)>=wtid);
	merge(l,r);
	free(dt);
	return 0;
}
int main()
{
	int n, errid=1;
	clock_t begin = clock();
	scanf("%d",&n);int shmid; 
    key_t key = IPC_PRIVATE; 
    int length;
    length = n; 
    size_t SHM_SIZE = length*sizeof(int); 
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) 
    { 
        perror("shmget"); 
        _exit(errid); 
    } 
    if ((a = (int *)shmat(shmid, NULL, 0)) == (int *) -1) 
    { 
        perror("shmat"); 
        _exit(errid); 
    } 
	for(int i=0;i<n;i+=1)
	{
		scanf("%d",&a[i]);
	}
	struct lr *st=(struct lr*)malloc(sizeof(struct lr));
	st->r=n-1;
	st->l=0;
	merge_sort(st);
	clock_t end = clock();
	for(int i=0;i<n;i+=1)
		printf("%d ",a[i]);
	if (shmdt(a) == -1) 
    { 
        perror("shmdt"); 
        _exit(errid); 
    } 
  
    if (shmctl(shmid, IPC_RMID, NULL) == -1) 
    { 
        perror("shmctl"); 
        _exit(errid); 
    }

    double time_spent = (double)(end - begin) / (double)CLOCKS_PER_SEC;
	printf("\n%lf\n",time_spent);
}
