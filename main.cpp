#include <iostream>
#include <fstream>
#define NUMOFVAR 50
#define NUMOFSLACK 50
#define ROWSIZE (NUMOFSLACK+1)
#define COLSIZE (NUMOFSLACK+NUMOFVAR+1)

using namespace std;

void print(float wv[ROWSIZE][COLSIZE])
{
    for(int j=0;j<ROWSIZE;j++)
        {
            for(int i=0;i<COLSIZE;i++)
            {
                cout<<wv[j][i]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl<<endl;
}


int main()
{
    //MakeMatrix
    float wv[ROWSIZE][COLSIZE];
	for(int j=0;j<ROWSIZE; j++)
	{
		for(int i =0;i<COLSIZE;i++)
		{
			wv[j][i]=0;
		}
	}

	fstream myFile;
    myFile.open("baza.txt",ios::in); //otvaram fajl u read modu
	if(myFile.is_open())
    {
        for(int j = 0; j < ROWSIZE; j++)
        {
            for(int i = 0; i< NUMOFVAR; i++)
            {
              myFile >> wv[j][i];
            }
        }
		for(int j = 0;j< NUMOFSLACK;j++)
		{
			myFile >> wv[j][COLSIZE-1];
		}
    }
    myFile.close();

	for(int j=0;j<ROWSIZE-1;j++)
	{
		{
			wv[j][NUMOFVAR+j]=1;
		}
	}
    //CalculateSimplex
    int pivotRow;
    int pivotCol;
    bool unbounded=false;
    bool optimality=false;
    float pivot;

    while(!optimality)
    {
        //checkOptimality(wv)
        optimality=true;
        for(int i=0;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<0)//min> max<
                optimality=false;
        }
        //findPivotCol(wv);
        float minnegval=wv[ROWSIZE-1][0];
        int loc=0;
        for(int i=1;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<minnegval)
            {
                minnegval=wv[ROWSIZE-1][i];
                loc=i;
            }
        }
        pivotCol=loc;
        //isUnbounded(wv,pivotCol)
        unbounded=true;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
                unbounded=false;
        }
        if(unbounded)
        {
            break;
        }
        //findPivotRow(wv,pivotCol);
        float rat[ROWSIZE-1];
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
            {
                rat[j]=wv[j][COLSIZE-1]/wv[j][pivotCol];
            }
            else
            {
                rat[j]=0;
            }
        }

        float minpozval=99999999;
        loc=0;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(rat[j]>0)
            {
                if(rat[j]<minpozval)
                {
                    minpozval=rat[j];
                    loc=j;
                }
            }
        }
        pivotRow=loc;

        pivot=wv[pivotRow][pivotCol];

        //doPivoting(wv,pivotRow,pivotCol,pivot);
        float newRow[COLSIZE];
        float pivotColVal[ROWSIZE];
        for(int i=0;i<COLSIZE;i++)
        {
            newRow[i]=wv[pivotRow][i]/pivot;
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            pivotColVal[j]=wv[j][pivotCol];
        }

        for(int j=0;j<ROWSIZE;j++)
        {
            if(j==pivotRow)
            {
                for(int i=0;i<COLSIZE;i++)
                {
                    wv[j][i]=newRow[i];
                }
            }
            else
            {
                for(int i=0;i<COLSIZE;i++)
                {
                    wv[j][i]=wv[j][i]-newRow[i]*pivotColVal[j];
                }
            }
        }
    }
    //Ispisivanje rezultata
    if(unbounded)
    {
        cout<<"Unbounded"<<endl;
    }
    else
    {
        //solutions(wv);
        for(int i=0;i<NUMOFVAR; i++)  //every basic column has the values, get it form B array
        {
            int count0 = 0;
            int index = 0;
            for(int j=0; j<ROWSIZE-1; j++)
            {
                if(wv[j][i]==0.0)
                {
                    count0 = count0+1;
                }
                else if(wv[j][i]==1)
                {
                    index = j;
                }
            }
            if(count0 == ROWSIZE - 2 )
            {
                cout<<"variable"<<i+1<<": "<<wv[index][COLSIZE-1]<<endl;  //every basic column has the values, get it form B array
            }
            else
            {
                cout<<"variable"<<i+1<<": "<<0<<endl;
            }
        }

        cout<<""<<endl;
        cout<<endl<<"Optimal solution is "<<wv[ROWSIZE-1][COLSIZE-1]<<endl;
    }

    return 0;
}
