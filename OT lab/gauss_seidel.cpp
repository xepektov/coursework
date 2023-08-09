#include<iostream>
#include<conio.h>
 
using namespace std;
 
float a[1000][1000];
float x1[1000];
float x[1000];
int n;

bool norm(){
    float sum=0;
    for(int i=0;i<1000;i++){
        sum+=(x1[i]-x[i])*(x1[i]-x[i]);
    }
    if(sum<0.001){
        return true;
    }
    return false;
}

int main(void)
{
    float b[1000], y[1000];
    n=0;
    int m = 0, i = 0, j = 0;
    cout << "Enter no. of eqns : ";
    cin >> n;

    cout << "\nEnter the coeffients:\n\n";
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            cin >> a[i][j];
        }
    }
    cout << "\nEnter RHS values \n";
    for (i = 0; i < n; i++)
    {
        cin >> b[i];
    }

    cout << "Enter initial values\n";
    for (i = 0; i < n; i++)
    {
        cin >> x[i];
    }

    cout << "\nEnter the no. of iteration : ";
    cin >> m;
    while (m > 0)
    {
        
        for (i = 0; i < n; i++)
        {
            x1[i]= x[i];
        }

        for (i = 0; i < n; i++)
        {

            

            y[i] = (b[i] / a[i][i]);
            for (j = 0; j < n; j++)
            {
                if (j == i)
                    continue;
                y[i] = y[i] - ((a[i][j] / a[i][i]) * x[j]);
                x[i] = y[i];
            }
            printf("x%d = %f    ", i + 1, y[i]);
        }

        if(norm()){
            cout<<"Solution found";
            break;
        }
        cout << "\n";
        m--;
    }
    return 0;
}