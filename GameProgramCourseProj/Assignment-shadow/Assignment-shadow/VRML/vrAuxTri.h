
#define	X	0
#define	Y	1

#define DIM     2               /* Dimension of points */
typedef float     tPointi[DIM];   /* type integer point */
typedef float  tPointd[DIM];   /* type double point */

#define PMAX    100000            /* Max # of pts in polygon */
typedef tPointi tPolygoni[PMAX];/* type integer polygon */

int	vrTriRecurse( int n, tPolygoni P, long labels[],
				    long Indexs[PMAX][3],int *NumTri );

void vrRemoveCollinear(int *n,tPolygoni P,long indices[]);

void vrRemoveDegenTri(tPolygoni	Pt, long Indexs[PMAX][3],int *NumTri,long indices[]);

void vrRemoveOutsideTri(tPolygoni P,long Indexs[PMAX][3],int *NumTri,
					  long indices[],int NumberVertex);
