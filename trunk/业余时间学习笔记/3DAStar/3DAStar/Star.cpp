#include "StdAfx.h"
#include "Star.h"
#include <cmath>

#define  xDepth  100
#define  yDepth  100
#define  zDepth	 20

Node3D  nodes[20][100][100] ;
short	array[20][100][100] = { 0 };

Star::Star(void)
{
}

Star::~Star(void)
{
}


bool Star::Creatgraph()
{
	Node3D node;
	int w2 = Node3D::SIZE * xDepth /2 ;
	int h2 = Node3D::SIZE * yDepth /2 ;
	int d2 = Node3D::SIZE * zDepth /2 ;

	memset(array,1,sizeof(array));

	/// 随机生成10个阻塞的点,可能有重复的点
	for ( int i = 0 ; i < 50 ; i++ )
	{
		int  x  = rand() % 20;
		int  y  = rand() % 100;
		int  z  = rand() % 100;

		array[x][y][z] = 0 ;
	}
	array[0][0][0] = 1;
	array[15][10][35] = 1;
	for ( int i = 0 ; i < zDepth ; i++ )
	{
		for ( int j  = 0; j < yDepth ; j++ )
		{
			for ( int k =0 ; k < xDepth ; k++ )
			{
				Point pi;
				pi.xPos = k ;
				pi.yPos = j ;
				pi.zPos = i ;
				bool isWalkable = array[i][j][k] >= 1 ? true : false;
				node.setWalkable( isWalkable );
				node.setPoint( pi );
				nodes[i][j][k] = node; 
			}
		}
	}
	
	startp = nodes[0][0][0];			/// 设置开始节点
	endp   = nodes[15][10][35];			/// 设置结束节点
	Find_path( &nodes[0][0][0] );
	return true ;
}

bool Star::Find_path(Node3D * node)
{
	
	if ( *node != endp  && endp.getWalkable() )
	{
		/*
		*   Brief :  采用BFS避免DFS递归溢出问题。
		*   Author:  Expter 
		*	Creat Date:  [2009/10/27]
		*/
		do 
		{
			close[node] =  node->f;
			int	sz = max(node->point.zPos -1 , 0 );
			int	ez = min(node->point.zPos +1 ,zDepth-1);

			int	sy = max(node->point.yPos -1 , 0 );
			int	ey = min(node->point.yPos +1 ,yDepth-1);

			int	sx = max(node->point.xPos -1 , 0 );
			int	ex = min(node->point.xPos +1 ,xDepth-1);

			for ( int i = sz ; i <= ez ; i++ )
			{
				for ( int j  = sy ; j <= ey ; j++ )
				{
					for ( int k = sx ; k <= ex ; k++ )
					{
						if ( !( i == node->point.zPos && j== node->point.yPos && k == node->point.xPos ))
						{
							//adjNode  = nodes[i][j][k];
							searchchNode( &nodes[i][j][k], node );
						}
					}
				}
			}
	
			Node3D * best = NULL;
			if ( open.size() >= 1 )
			{
				best = open.begin()->first;
				open.erase( open.begin() );
			}
			node = best;
		} while ( node != NULL  && (*node != endp)) ;
	}

	if ( node != NULL  && endp.getWalkable() && (*node == endp))
	{
		std::cout << " 找到路径 " << std::endl;
		int s2 = Node3D::SIZE / 2; 
		Node3D *adjNode = node;

		path.clear();
		while ( *adjNode != startp )
		{
			if ( *adjNode != startp )
			{
				adjNode->setPath( true );
				path.push_back( adjNode->point );
			}
			if ( adjNode->parent == NULL )
			{
				break ;
			}
			adjNode = (Node3D*)(adjNode->parent);
		}
		
		path.push_back( adjNode->point );
		for ( int i = path.size()-1 ; i >=0  ; --i )
		{
			char str[100];
			path[i].tostring(str);
			std::cout << str << std::endl;
		}
	}
	else
	{
		std::cout << " 不能找到路径 "  << std::endl;
	}
	return true ;
}

void Star::searchchNode(Node3D * adjNode  , Node3D * current)
{
	bool xZDiagonal = ( adjNode->point.xPos != current->point.xPos && adjNode->point.zPos != current->point.zPos );
	bool xYDiagonal = ( adjNode->point.xPos != current->point.xPos && adjNode->point.yPos != current->point.yPos );
	bool yZDiagonal = ( adjNode->point.yPos != current->point.yPos && adjNode->point.zPos != current->point.zPos );

	bool corner = false;

	Node3D * tmp = current;

	///
	///  注释、、 
	///
// 	/// 如果是顶点，可以抄近到  ,不需要此判断。
// 	if( 0 && (xZDiagonal || xYDiagonal || yZDiagonal) ) 
// 	{
// 
// 		int xDirection = (adjNode->point.xPos - current->point.xPos);
// 		int yDirection = (adjNode->point.yPos - current->point.yPos);
// 		int zDirection = (adjNode->point.zPos - current->point.zPos);
// 
// 		/// 如果是2步的走
// 		if ( abs( xDirection) == 2 || abs(xZDiagonal) == 2 || abs(zDirection)== 2 )
// 		{
// 			corner = true;
// 			int x1 = ( current->point.xPos + adjNode->point.xPos) /2 ;
// 			int y1 = ( current->point.yPos + adjNode->point.yPos) /2 ;
// 			int z1 = ( current->point.zPos + adjNode->point.zPos) /2 ;
// 
// 			if ( nodes[z1][y1][x1].getWalkable() &&  !findItem(open,adjNode) && !findItem(close,adjNode) )
// 			{
// 				// corner = false;
// 				// tmp->parent = &nodes[z1][y1][x1];
// 			}
// 		}
// 		
// 		//corner = false;//( xZCorner1 || xZCorner2 || xYCorner1 || xYCorner2 || yZCorner1 || yZCorner2);
// 	}

	if(  adjNode->getWalkable() &&  !findItem(open,adjNode) && !findItem(close,adjNode) && !corner)
	{
		adjNode->parent = tmp;
		adjNode->g = current->g = (xZDiagonal || xYDiagonal || yZDiagonal) ? 14 : 2;

		int difX = endp.point.xPos - adjNode->point.xPos;
		int difY = endp.point.yPos - adjNode->point.yPos;
		int difZ = endp.point.zPos - adjNode->point.zPos;

		if( difX < 0 )		difX = difX * -1 ;
		if( difY < 0 )		difY = difY * -1 ;
		if( difZ < 0 )      difZ = difZ * -1 ;

		adjNode->h = (difX + difY + difZ ) * 10;
		adjNode->f = adjNode->g + adjNode->h ;

		//open[adjNode] = adjNode->f ;
		open.insert(  pair<Node3D*,int>( adjNode , adjNode->f ) );
	}
	else if  ( adjNode->getWalkable() && !findItem(close,adjNode) && !corner ) 
	{
		int g = current->g  + ( xZDiagonal || xYDiagonal || yZDiagonal ) ? 14 : 10;
		if ( g < adjNode->g )
		{
			adjNode->g = g;
			adjNode->parent = current;
		}
	}
}