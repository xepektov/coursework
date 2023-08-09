#include<iostream>
#include<vector>
#include<string>
using namespace std;


    vector<vector<int>> constraints;  // n+2 -> 1 for b and 1 for slack variable



int main(){
    cout<<"enter no. of independent variable in cost function"<<endl;
    int n1,n2,n;  // n1 -> default variables, n2 -> slack variables, n -> total variables
    cin>>n1;
    cout<<"enter no. of constraints"<<endl;
    cin>>n2;
    n=n1+n2;
    vector<int> cost;


    // z + n1(default variables) + n2(slack variables) + 1(b) = n+2


    cout<<"enter cost function"<<endl;
    cost.push_back(1);                   // for z cofficient is 1

    for(int i=0;i<n1;i++){
        int temp;
        cin>>temp;
        cost.push_back(-1*temp);
    }
    for(int i=0;i<n2+1;i++){       // added (n2+1) zeroes for slack variables and b
        cost.push_back(0);
    }

    constraints.push_back(cost);  // added cost function to constraints

    cout<<"enter constraints"<<endl;

    for(int i=0;i<n2;i++){
        vector<int> temp;
        temp.push_back(0);              // for z(1)
        cout<<"constraint "<<i+1<<": "<<endl;
        for(int j=0;j<n1;j++){
            int t;
            cin>>t;
            temp.push_back(t);
        }

        // accepted the default variables (n1)
        cout<<"enter the sign of constraint"<<endl;
        char c;
        cin>>c;            //check for <= or >= 

        if(c=='<'){
            for(int j=0;j<n2;j++){
                if(i==j)
                    temp.push_back(1);
                else
                    temp.push_back(0);
            }
        }
        else if(c=='>'){
            for(int j=0;j<n2;j++){
                if(i==j)
                    temp.push_back(-1);
                else
                    temp.push_back(0);
            }
        }                                    // added slack variables ( n2)
 
        int b;
        cin>>b;
        temp.push_back(b);             //added b value (1)


        constraints.push_back(temp);
    }

    // accepted the constraints (n2)

    vector<string> all_var;   // {-,x1,x2,s1,s2,s3}
    vector<string> basic_var; // {-,s1,s2,s3}

    all_var.push_back("-");
    basic_var.push_back("-");

    for(int i=0;i<n1;i++){
        string s;
        s="x"+to_string(i+1);
        all_var.push_back(s);
    }
    for(int i=0;i<n2;i++){
        string s;
        s="s"+to_string(i+1);
        all_var.push_back(s);
        basic_var.push_back(s);
    }

    // added all variables to all_var and basic variables to basic_var

    cout<<"enter the type of problem"<<endl;
    cout<<"1. maximization"<<endl;
    cout<<"2. minimization"<<endl;
    int type;
    cin>>type;

    // accepted the type of problem
    cout<<constraints.size()<<" "<<constraints[0].size()<<endl;
    cout<<"z x1 x2 s1 s2 s3 b"<<endl;

    for(int i=0;i<n2+1;i++){
        for(int j=0;j<n+2;j++){
            cout<<constraints[i][j]<<" ";
        }
        cout<<endl;
    }
    // printing the constraints

    int pivot_column=0;
    int pivot_row=0;
    int pivot=0;

    while(true){

        int p=0;
        for(int i=1;i<n+1;i++){
            if(constraints[0][i]<p){
                p=constraints[0][i];
                pivot_column=i;
            }
        }
        cout<<"p"<<p<<endl;
        if(p>=0){
            break;
        }
        // if cost function has all non negative cofficients then break, also find pivot row

        bool flag=true;
        for(int i=1;i<n2+1;i++){
            if(constraints[i][pivot_column]>0){
                int tem=constraints[i][n+1]/constraints[i][pivot_column];
                cout<<i<<"tem: "<<tem<<endl;
                if(flag){
                    pivot=tem;
                    pivot_row=i;
                    flag=false;
                }
            
                if(tem<pivot){
                    pivot=tem;
                    pivot_row=i;
                }
            }
        }
        
        if(flag){
            cout<<"unbounded"<<endl;
            return 0;
        }

        cout<<"pivot row: "<<pivot_row<<" pivot column: "<<pivot_column<<endl;

        // found pivot row

        pivot = constraints[pivot_row][pivot_column];    //found pivot

        basic_var[pivot_row]=all_var[pivot_column];      //changed basic variable

        cout<<"pivot: "<<pivot<<endl;

        for(int i=0;i<n+2;i++){
            constraints[pivot_row][i]=constraints[pivot_row][i]/pivot;      //divided pivot row by pivot
        }      

        for(int i=0;i<n2+1;i++){
            if(i!=pivot_row && constraints[i][pivot_column]!=0){
                int temp=constraints[i][pivot_column];
                for(int j=0;j<n+2;j++){
                    constraints[i][j]=constraints[i][j]-temp*constraints[pivot_row][j];      //made pivot column cofficients zero
                }
            }
        }

        cout<<"z x1 x2 s1 s2 s3 b"<<endl;
        for(int i=0;i<n2+1;i++){
            for(int j=0;j<n+2;j++){
                cout<<constraints[i][j]<<" ";
            }
            cout<<endl;
        }
        
    }

    // found the solution
    for(int i=0;i<n2+1;i++){
        if(basic_var[i][0]=='x'){
            cout<<basic_var[i]<<" = "<<constraints[i][n+1]<<endl;
        }
    }
    
}

