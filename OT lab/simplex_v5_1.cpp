#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<set>
#include <algorithm>

#define FLT_MAX 3.402823466e+9F /* max value */
#define FLT_MIN -3.402823466e+9F /* min value */
using namespace std;

map<string,int> name_to_idx;
map<int,string> idx_to_name;
vector<string> all_var;
vector<double> cost;

vector<string> basic_var;
vector<double> basic_var_cost;
vector<string> non_basic_var;
vector<double> non_basic_var_cost; 
vector<double> delta;  

vector<vector<double>> constraints;  // n+2 -> 1 for b and 1 for slack variable
int main(){
    cout<<"enter the type of problem"<<endl;
    cout<<"1. maximization"<<endl;
    cout<<"2. minimization"<<endl;
    int type;
    cin>>type;

    cout<<"enter no. of independent variable in cost function"<<endl;
    int n1,n2,n3,n,constrain_no;  // n1 -> default variables, n2 -> slack variables, n -> total variables
    cin>>n1;

    cout<<"enter bound for each variable, 1 for positive, 2 for unrestricted"<<endl;

    int cur_var=0;

    for(int i=0;i<n1;i++){
        cout<<"enter x"<<i+1<<" sign : "<<endl;
        int temp;
        cin>>temp;
        double c;
        cout<<"enter x"<<i+1<<" cost value : "<<endl;
        cin>>c;
        
        if(type==1){
            if(temp==1){
                all_var.push_back("x"+to_string(i+1));
                cost.push_back(c);
                name_to_idx["x"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="x"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
            }
            else if(temp==2){
                all_var.push_back("xa"+to_string(i+1));
                name_to_idx["xa"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="xa'"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
                cost.push_back(c);

                all_var.push_back("xb"+to_string(i+1));
                name_to_idx["xb"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="xb"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
                cost.push_back(-1*c);
            }
        }
        else if(type==2){
            if(temp==1){
                all_var.push_back("x"+to_string(i+1));
                cost.push_back(-1*c);
                name_to_idx["x"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="x"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
            }
            else if(temp==2){
                all_var.push_back("xa"+to_string(i+1));
                name_to_idx["xa"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="xa'"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
                cost.push_back(-1*c);

                all_var.push_back("xb"+to_string(i+1));
                name_to_idx["xb"+to_string(i+1)]=cur_var;
                idx_to_name[cur_var+1]="xb"+to_string(i+1);
                cur_var++; //cout<<endl<<" cv: "<<cur_var<<endl;
                cost.push_back(1*c);
            }
        }
    }

    for(auto i:all_var){
        cout<<i<<" ";
    }
    cout<<endl;
    for(auto i:cost){
        cout<<i<<" ";
    }

    cout<<endl<<"enter no. of constrains"<<endl;
    cin>>constrain_no;

    // z + n1(default variables) + n2(slack variables) + 1(b) = n+2
    // cout<<"enter cost function"<<endl;
    // cost.push_back(1);                   // for z cofficient is 1

    // for(int i=0;i<n1;i++){
    //     double temp;
    //     cin>>temp;
    //     if(type==1)
    //         cost.push_back(-1*temp);
    //     else if(type==2)
    //         cost.push_back(temp);
    // }
    // for(int i=0;i<n2+1;i++){       // added (n2+1) zeroes for slack variables and b
    //     cost.push_back(0);
    // }

    // constraints.push_back(cost);  // added cost function to constraints

    cout<<"enter constraints"<<endl;

    int slack_no=0;
    vector<string> slack_var;
    int surplus_no=0;
    vector<string> surplus_var;
    int artificial_no=0;
    vector<string> artificial_var;
    int total_extra_var=0;
    vector<string> real_variables;

    vector<double> b;
    vector<char> sign;
    vector<vector<double>> A;

    for(int i=0;i<constrain_no;i++){
        vector<double> temp;
        //temp.push_back(0);              // for z(1)
        cout<<"constraint "<<i+1<<": "<<endl;
        for(int j=0;j<n1;j++){
            double t;
            cin>>t;
            temp.push_back(t);
            if(name_to_idx.find("xa"+to_string(j+1))!=name_to_idx.end()){
                temp.push_back(-1*t);
            }
        }
        cout<<"enter the sign of constraint (<,=,>)"<<endl;
        char c;
        cin>>c;            //check for <= or >= or ==
        sign.push_back(c);

        cout<<"enter b"<<endl;
        double t;
        cin>>t;
        b.push_back(t);

        A.push_back(temp);
    }

    cout<<"input taken"<<endl;

    for(int i=0;i<constrain_no;i++){
        for(auto j:A[i]){
            cout<<j<<" ";
        }
        cout<<sign[i]<<" "<<b[i]<<endl;
    }

    for(int i=0;i<constrain_no;i++){                 // making all cofficient (b) positive
        if(b[i]<0){
            for(int j=0;j<A[i].size();j++){
                A[i][j]=-1*A[i][j];
            }
            if(sign[i]=='<'){
                sign[i]='>';
            }
            else if(sign[i]=='>'){
                sign[i]='<';
            }
            b[i]=-1*b[i];
        }
    }

    for(int i=0;i<constrain_no;i++){

        for(int j=0;j<total_extra_var;j++){
                A[i].push_back(0);
            }

        if(sign[i]=='<'){
            
            slack_no++;
            total_extra_var++;

            A[i].push_back(1);
            all_var.push_back("sl"+to_string(slack_no));
            slack_var.push_back("sl"+to_string(slack_no));
            name_to_idx["sl"+to_string(slack_no)]=cur_var;
            idx_to_name[cur_var+1]="sl"+to_string(slack_no);
            cur_var++; cout<<endl<<" cv: "<<cur_var<<endl;
            cost.push_back(0);
        }
        else if(sign[i]=='>'){
            surplus_no++;
            artificial_no++;
            total_extra_var = total_extra_var + 2;

            A[i].push_back(-1);
            all_var.push_back("sr"+to_string(surplus_no));
            surplus_var.push_back("sr"+to_string(surplus_no));
            name_to_idx["sr"+to_string(surplus_no)]=cur_var;
            idx_to_name[cur_var+1]="sr"+to_string(surplus_no);
            cur_var++; cout<<endl<<" cv: "<<cur_var<<endl;
            cost.push_back(0);

            A[i].push_back(1);
            all_var.push_back("a"+to_string(artificial_no));
            artificial_var.push_back("a"+to_string(artificial_no));
            name_to_idx["a"+to_string(artificial_no)]=cur_var;
            idx_to_name[cur_var+1]="a"+to_string(artificial_no);
            cur_var++; cout<<endl<<" cv: "<<cur_var<<endl;
            cost.push_back(-1e6);
        }
        else if(sign[i]=='='){
            artificial_no++;
            total_extra_var++;

            A[i].push_back(1);
            all_var.push_back("a"+to_string(artificial_no));
            artificial_var.push_back("a"+to_string(artificial_no));
            name_to_idx["a"+to_string(artificial_no)]=cur_var;
            idx_to_name[cur_var+1]="a"+to_string(artificial_no);
            cur_var++; cout<<endl<<" cv: "<<cur_var<<endl;
            cost.push_back(-1e6);
        }
    }

    // for(auto i:all_var){
    //     cout<<i<<" ";
    // }
    // cout<<endl;

    cout<<"output : "<<endl;

    for(int i=0;i<constrain_no;i++){
        int j=0;
        for(j=0;j<A[i].size();j++){
            cout<<A[i][j]<<"("<<all_var[j]<<") ";
        }
        while(j<all_var.size()){
            A[i].push_back(0);
            cout<<0<<"("<<all_var[j]<<") ";
            j++;
        }
        cout<<"= "<<b[i]<<endl;
    }

    cout<<"all variables :"<<endl;
    for(auto a:all_var){
        cout<<a<<" ,";
    }
    cout<<">= 0"<<endl;

    cout<<"costs of all variables :"<<endl;
    for(auto a:cost){
        cout<<a<<" ,";
    }
    cout<<endl;

    cout<<"slack variables : ";
    for(auto a:slack_var){
        cout<<a<<" ,";
    }
    cout<<endl;

    cout<<endl<<"surplus variables : ";
    for(auto a:surplus_var){
        cout<<a<<" ,";
    }
    cout<<endl;

    cout<<endl<<"artificial variables : ";
    for(auto a:artificial_var){
        cout<<a<<" ,";
    }
    cout<<endl;

    for(auto a:all_var){
        if(a[0]=='x'){
            real_variables.push_back(a);
        }
    }

    for(auto a:all_var){
        if(a[0]=='x'){
            non_basic_var.push_back(a);
        }
        else if(a[1]=='l'){
            basic_var.push_back(a);
        }
        else if(a[0]=='a'){
            basic_var.push_back(a);
        }
        else if(a[1]=='r'){
            non_basic_var.push_back(a);
        }
    }

    cout<<"basic variables :"<<endl;
    for(auto a:basic_var){
        cout<<a<<" ";
    }
    cout<<endl;

    cout<<"non basic variables :"<<endl;
    for(auto a:non_basic_var){
        cout<<a<<" ";
    }
    cout<<endl;

    for(int i=0;i<all_var.size();i++){
        cout<<all_var[i]<<"("<<name_to_idx[all_var[i]]<<")"<<" ";
    }
    cout<<endl;

    for(int i=0;i<basic_var.size();i++){
        cout<<basic_var[i]<<name_to_idx[basic_var[i]]<<" ";
        basic_var_cost.push_back(cost[name_to_idx[basic_var[i]]]);
    }

    cout<<"basic variables cost :"<<endl;
    for(auto a:basic_var_cost){
        cout<<a<<" ";
    }
    cout<<endl;

    for(int i=0;i<all_var.size();i++){
        double sum=0;
        for(int j=0;j<basic_var.size();j++){
            sum+=A[j][i]*basic_var_cost[j];
        }
        delta.push_back(sum-cost[i]);
    }

    cout<<"delta :"<<endl;
    for(auto a:delta){
        cout<<a<<" ";
    }
    cout<<endl;

    double total_cost=0;

    //making the table :
    int pivot_column;
    int pivot_row;
    double pivot;

    set<pair<int,int>> tie_rows;     //for tie breaking ( value,row)
    

    int n_ter =7;
    while(--n_ter){

        //get entering variable
        double min_delta=FLT_MAX;
        int min_delta_index=-1;
        int check_delta_flag=0;            //if all delta are positive then all solutions found
        bool min_ratio_tie_flag=0;

        for(int i=0;i<delta.size();i++){
            if(delta[i]<min_delta){
                min_delta=delta[i];
                min_delta_index=i;
            }
            if(delta[i]<0){
                check_delta_flag=1;         //if any delta is negative then all solutions not found
            }
        }

        if(check_delta_flag==0){
            cout<<"all solutions found"<<endl;
            break;
        }

        pivot_column=min_delta_index;

        //get leaving variable
        double min_ratio=FLT_MAX;
        int min_ratio_index=-1;
        int check_column_flag=0;            //if all ratios are negative then unbounded
        int prev_min_raio_index=-1;
        double prev_min_ratio=FLT_MAX;


        for(int i=0;i<basic_var.size();i++){
            if(A[i][pivot_column]>0){
                double temp=b[i]/A[i][pivot_column];

                if(temp==min_ratio){
                    tie_rows.insert(make_pair(A[prev_min_raio_index][pivot_column],prev_min_raio_index));       //vector of tie rows
                    tie_rows.insert(make_pair(A[i][pivot_column],i));       //vector of tie rows
                    min_ratio_tie_flag=1;
                }
                if(temp<=min_ratio){
                    min_ratio=temp;
                    min_ratio_index=i;
                    prev_min_raio_index=i;
                    prev_min_ratio=temp;
                }
                
            }
            if(A[i][pivot_column]>0){
                check_column_flag=1;
            }
        }

        cout<<"tie rows : "<<endl;
        for(auto tr:tie_rows){
            cout<<tr.second<<" ";
        }
        cout<<endl;

        cout<<"unit matrix idx : "<<endl;

        vector<int> temp_basic_var_idx;
        for(int i=0;i<basic_var.size();i++){
            temp_basic_var_idx.push_back(name_to_idx[basic_var[i]]);      //vector of basic variable index
            cout<<name_to_idx[basic_var[i]]<<" ";
        }
        cout<<endl;

        int final_row=-1;
        bool zero_found_flag=0;
        bool unit_matrix_tie_flag=0;

        for(int i=0;i<temp_basic_var_idx.size();i++){       //check all tie rows for unit matrix
            int column_idx = temp_basic_var_idx[i];

            unit_matrix_tie_flag=0;
            zero_found_flag=0;
            final_row=-1;

            for(auto tr:tie_rows){
                int row_idx = tr.second;
                if(A[row_idx][column_idx]==0){
                    if(zero_found_flag){
                        unit_matrix_tie_flag=1;
                    }
                    zero_found_flag=1;
                    final_row=row_idx;
                }
            }
            if(!unit_matrix_tie_flag && final_row!=-1){
                break;
            }
        }




        if(check_column_flag==0){
            cout<<"unbounded"<<endl;      //if all ratios are negative then unbounded
            return 0;
        }

        cout<<"min ratio : "<<min_ratio<<endl;      //print min ratio

        pivot_row=min_ratio_index;
        if(min_ratio_tie_flag){
            pivot_row=final_row;
            cout<<"there is degeneracy"<<endl;
        }
        pivot = A[pivot_row][pivot_column];    //found pivot

        cout<<"pivot : "<<pivot<<endl;
        cout<<"pivot row : "<<pivot_row<<endl;
        cout<<"pivot column : "<<pivot_column<<endl;
        // found pivot row and column

        cout<<"incoming variable : "<<all_var[pivot_column]<<endl;  //print incoming variable
        cout<<"outgoing variable : "<<basic_var[pivot_row]<<endl;   //print outgoing variable

        std::vector<string>::iterator non_basic_var_itr = find(non_basic_var.begin(),non_basic_var.end(),all_var[pivot_column]);
        if(non_basic_var_itr!=non_basic_var.end()){
            non_basic_var.erase(non_basic_var_itr);       //removed previous non basic variable
        }
        non_basic_var.push_back(basic_var[pivot_row]);   //added non basic variable

        basic_var[pivot_row]=all_var[pivot_column];      //changed basic variable
        // non_basic_var[pivot_column]=basic_var[pivot_row];   

        for(int i=0;i<all_var.size();i++){
            A[pivot_row][i]=A[pivot_row][i]/pivot;      //divided pivot row by pivot
        }
        b[pivot_row]=b[pivot_row]/pivot;                //divided b by pivot

        for(int i=0;i<basic_var.size();i++){
            if(i!=pivot_row && A[i][pivot_column]!=0){
                double temp=A[i][pivot_column];
                for(int j=0;j<all_var.size();j++){
                    A[i][j]=A[i][j]-temp*A[pivot_row][j];      //made pivot column cofficients zero
                }
                b[i]=b[i]-temp*b[pivot_row];
            }
        }

        for(int i=0;i<basic_var.size();i++){
            basic_var_cost[i] = cost[name_to_idx[basic_var[i]]];    //calculating basic variables cost
        }

        for(int i=0;i<all_var.size();i++){
            double sum=0;
            for(int j=0;j<basic_var.size();j++){                   //calculating delta
                sum+=A[j][i]*basic_var_cost[j];
            }
            delta[i]=sum-cost[i];
        }

        //printing table

        //print all var cost
        cout<<"all variables :"<<endl;
        for(auto a:all_var){
            cout<<a<<" ,";
        }
        cout<<">= 0"<<endl;

        cout<<"costs of all variables :"<<endl;
        for(auto a:cost){
            cout<<a<<" ,";
        }
        cout<<endl;

        cout<<"  ";
        for(auto a:all_var){
            cout<<a<<"  ";
        }
        cout<<endl;

        for(int i=0;i<basic_var.size();i++){
            cout<<basic_var[i]<<" ";
            for(int j=0;j<all_var.size();j++){
                cout<<A[i][j]<<" ";
            }
            cout<<b[i]<<endl;
        }

        cout<<"deltas :"<<endl;
        for(auto a:delta){
            cout<<a<<"  ";
        }
        cout<<endl;

        cout<<"basic variables :"<<endl;
        for(auto a:basic_var){
            cout<<a<<" ";
        }
        cout<<endl;
        cout<<"non basic variables :"<<endl;
        for(auto a:non_basic_var){
            cout<<a<<" ";
        }
        cout<<endl;

        cout<<"basic variables cost :"<<endl;
        for(auto a:basic_var_cost){
            cout<<a<<" ";
        }
        cout<<endl;

        //find total cost
        total_cost=0;
        for(int i=0;i<basic_var.size();i++){
            if(basic_var[i][0]=='x'){
                total_cost+=basic_var_cost[i]*b[i];
            }
        }
        cout<<"total cost : "<<total_cost<<endl;

        cout<<endl<<endl<<"------------------------"<<endl<<endl;
    }

    int infeasible_flag=0;
    int allover_degeneracy_flag=0;

    for(int i=0;i<basic_var.size();i++){
        cout<<basic_var[i]<<" = "<<b[i]<<endl;
        if(basic_var[i][0]=='a' && b[i]>0){
            infeasible_flag=1;
        }
        if(b[i]==0){
            allover_degeneracy_flag=1;
        }
    }
    if(infeasible_flag==1){
        cout<<"infeasible"<<endl;
    }
    else{
        cout<<"feasible"<<endl;
    }
    if(allover_degeneracy_flag==1){
        cout<<"there is degeneracy"<<endl;
    }
    

    int alternative_flag=0;
    int alternate_incoming_var_idx;
    int alternate_outgoing_var_idx;

    for(int i=0;i<non_basic_var.size();i++){
        if(delta[name_to_idx[non_basic_var[i]]]==0){  //if there is any zero delta for non basic variable then alternative solution might exist
            alternative_flag=1;
            alternate_incoming_var_idx=name_to_idx[non_basic_var[i]];
        }
    }

    if(alternative_flag){
        cout<<"there is alternative solution"<<endl;
    }


    // int alt_iter =5;
    // if(alternative_flag && !infeasible_flag && --alt_iter){

    //     pivot_column=alternate_incoming_var_idx;

    //     //get leaving variable
    //     double min_ratio=FLT_MAX;
    //     int min_ratio_index=-1;
    //     int check_column_flag=0;            //if all ratios are negative then unbounded

    //     for(int i=0;i<basic_var.size();i++){
    //         if(A[i][pivot_column]>0){
    //             double temp=b[i]/A[i][pivot_column];
    //             if(temp<min_ratio){
    //                 min_ratio=temp;
    //                 min_ratio_index=i;
    //             }
    //         }
    //         if(A[i][pivot_column]>0){
    //             check_column_flag=1;
    //         }
    //     }

    //     if(check_column_flag==0){
    //         cout<<"unbounded"<<endl;      //if all ratios are negative then unbounded
    //         return 0;
    //     }

    //     cout<<"min ratio : "<<min_ratio<<endl;      //print min ratio

    //     pivot_row=min_ratio_index;
    //     pivot = A[pivot_row][pivot_column];    //found pivot

    //     cout<<"pivot : "<<pivot<<endl;
    //     cout<<"pivot row : "<<pivot_row<<endl;
    //     cout<<"pivot column : "<<pivot_column<<endl;
    //     // found pivot row and column

    //     cout<<"incoming variable : "<<all_var[pivot_column]<<endl;  //print incoming variable
    //     cout<<"outgoing variable : "<<basic_var[pivot_row]<<endl;   //print outgoing variable

    //     basic_var[pivot_row]=all_var[pivot_column];      //changed basic variable
    //     non_basic_var[pivot_column]=basic_var[pivot_row];   //changed non basic variable

    //     for(int i=0;i<all_var.size();i++){
    //         A[pivot_row][i]=A[pivot_row][i]/pivot;      //divided pivot row by pivot
    //     }
    //     b[pivot_row]=b[pivot_row]/pivot;                //divided b by pivot

    //     for(int i=0;i<basic_var.size();i++){
    //         if(i!=pivot_row && A[i][pivot_column]!=0){
    //             double temp=A[i][pivot_column];
    //             for(int j=0;j<all_var.size();j++){
    //                 A[i][j]=A[i][j]-temp*A[pivot_row][j];      //made pivot column cofficients zero
    //             }
    //             b[i]=b[i]-temp*b[pivot_row];
    //         }
    //     }

    //     for(int i=0;i<basic_var.size();i++){
    //         basic_var_cost[i] = cost[name_to_idx[basic_var[i]]];    //calculating basic variables cost
    //     }

    //     for(int i=0;i<all_var.size();i++){
    //         double sum=0;
    //         for(int j=0;j<basic_var.size();j++){                   //calculating delta
    //             sum+=A[j][i]*basic_var_cost[j];
    //         }
    //         delta[i]=sum-cost[i];
    //     }

    //     //printing table

    //     //print all var cost
    //     cout<<"all variables :"<<endl;
    //     for(auto a:all_var){
    //         cout<<a<<" ,";
    //     }
    //     cout<<">= 0"<<endl;

    //     cout<<"costs of all variables :"<<endl;
    //     for(auto a:cost){
    //         cout<<a<<" ,";
    //     }
    //     cout<<endl;

    //     cout<<"  ";
    //     for(auto a:all_var){
    //         cout<<a<<"  ";
    //     }
    //     cout<<endl;

    //     for(int i=0;i<basic_var.size();i++){
    //         cout<<basic_var[i]<<" ";
    //         for(int j=0;j<all_var.size();j++){
    //             cout<<A[i][j]<<" ";
    //         }
    //         cout<<b[i]<<endl;
    //     }

    //     cout<<"deltas :"<<endl;
    //     for(auto a:delta){
    //         cout<<a<<"  ";
    //     }
    //     cout<<endl;

    //     cout<<"basic variables :"<<endl;
    //     for(auto a:basic_var){
    //         cout<<a<<" ";
    //     }
    //     cout<<endl;
    //     cout<<"non basic variables :"<<endl;
    //     for(auto a:non_basic_var){
    //         cout<<a<<" ";
    //     }
    //     cout<<endl;

    //     cout<<"basic variables cost :"<<endl;
    //     for(auto a:basic_var_cost){
    //         cout<<a<<" ";
    //     }
    //     cout<<endl;

    //     //find total cost
    //     for(int i=0;i<basic_var.size();i++){
    //         total_cost+=basic_var_cost[i]*b[i];
    //     }
    //     cout<<"total cost : "<<total_cost<<endl;

    //     cout<<endl<<endl<<"------------------------"<<endl<<endl;
    // }

}