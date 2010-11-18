#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct CPoint
{
	double x, y;
}Point;
const int MAXSIZE = 10000;
const double EPS = 1E-6;
int  stack[MAXSIZE]; //数组模拟堆栈，存放凸包的逆序顶点
int  top;            //凸包顶点数p[stack[0]...stack[top]]
Point p[MAXSIZE];

void   GrahamScan(const int &n); //Graham扫描法求凸包
double Dist(const Point &arg1, const Point &arg2); //两点距离
double Length(const int &n);   //多边形周长
double Area(const int &n);     //多边形面积
void   Swap(const int &i, const int &j); //交换两个点
double Multi(const Point &p0, const Point &p1, const Point &p2); //叉积运算
int    cmp(const void *arg1, const void *arg2); //自定义排序:按照极角

int main(void)
{
	int  n;
	// freopen("Input.txt", "r", stdin);
	while( scanf("%d", &n), n )
	{
		int  i;
		for( i=0; i < n; ++i )
		{
			scanf("%lf %lf", &p[i].x, &p[i].y);
		}
		if( n == 1 )
		{
			printf("周长: 0.00\t面积: 0.00\n");
		}
		else if( n == 2 ) 
		{
			printf("周长: %.2lf\t面积: 0.00\n", Dist(p[0], p[1]));
		}
		else 
		{
			GrahamScan(n); 
			for( int k=0; k <= top; ++k )
			{
				printf("%lf  %lf\n", p[stack[k]].x, p[stack[k]].y);
			}
			printf("周长: %.2lf\t面积: %.2lf\n", Length(top+1), Area(top+1));
		}
	}
	
	return 0;
}

double Length(const int &n)
{
	double  sum = Dist( p[stack[n-1]], p[stack[0]] );
	for( int i = 0; i < n-1; ++i )
	{
		sum += Dist( p[stack[i]], p[stack[i+1]] );
	}
	return sum;
}

double Area(const int &n)
{
	double area = 0;
	for( int i=0; i < n-1; ++i )
	{
		area += (p[stack[i]].x*p[stack[i+1]].y) - (p[stack[i+1]].x*p[stack[i]].y);
	}
	area = fabs(area) / 2;
	return area;
}

double Dist(const Point &arg1, const Point &arg2)
{
	return sqrt( (arg1.x - arg2.x)*(arg1.x - arg2.x) + (arg1.y - arg2.y)*(arg1.y - arg2.y) );
}

void Swap(const int &i, const int &j)
{
	Point temp = p[i];
	p[i] = p[j];
	p[j] = temp;
}

//通过矢量叉积求极角关系(p0p1)(p0p2)
//P*Q > 0,P在Q的顺时针方向... ...
double Multi(const Point &p0, const Point &p1, const Point &p2)
{
	return (p1.x-p0.x)*(p2.y-p0.y) - (p2.x-p0.x)*(p1.y-p0.y);
}

int cmp(const void *arg1, const void *arg2)
{
	Point a = *(Point*)arg1;
	Point b = *(Point*)arg2;
	
	double k = Multi(p[0], a, b);
	if( k < -EPS ) return 1;
	else if( fabs(k) < EPS && (Dist(a, p[0]) - Dist(b, p[0])) > EPS ) return 1;
	else return -1;
}

void GrahamScan(const int &n)
{
	int  i, u = 0;
	for( i = 1; i < n; ++i )
	{
		//找到最左下的顶点, 令其为p[0]
		if( ( p[i].y < p[u].y ) || ( p[i].y == p[u].y && p[i].x < p[u].x ) ) 
			u = i;
	}

	i = 0;
	Swap(i, u); //只交换一次
	
	//将点按照相对p[0]的极角从小到大排序, 用qsort
	qsort( p+1, n-1, sizeof(p[0]), cmp );
	//将每一个点逐一检验, 是凸包点放入栈中, 不是则弹出栈。
	//注意: 排序后, p[0]和p[1]一定是凸包的顶点
	stack[0] = 0; stack[1] = 1; stack[2] = 2;
    top = 2;
	for( int j = 3; j < n; ++j )
	{
		while( Multi( p[j], p[stack[top]], p[stack[top-1]] ) > 0 )
		{
			--top;
			if( top == 0 ) break;
		}
		stack[++top] = j;
	}
}
//本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/Fandywang_jlu/archive/2008/03/20/2201350.aspx