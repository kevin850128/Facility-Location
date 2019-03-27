/*
   This code can be compiled and run ok.
     利用貪婪演算法找出最佳設施地址

   usage (how to run):
     hw3

   input file:
     data1.txt or data2.txt

   output file:
     none

   compile (how to compile):
     g++ -o hw3 hw3.cpp

   pseudocode:
     每個網格會有專屬的設施負責，不可能重複負責，這是關鍵
     其他部分基本上照著pdf的步驟做都不太會有問題

   coded by 江冠駒, ID: H24031354, email: kevin040208@gmail.com
   date: 2018.04.16
*/



#include<iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;



int main(){
    //---begin--- PART 1: Declaration and read file-----------
    struct Zone{             //此為各網格之struct
        int a;              //儲存某網格之縱軸座標，即由上而下第a列,a=1,...,M
        int b;              //儲存某網格之橫軸座標，即由左而右第b行,b=1,...,N
        double W,           //儲存某網格之自身需求量
               C,           //儲存某網格之設施建置成本
               Wbar,        //儲存某網格可涵蓋之鄰近R範圍內之需求量總和
               Q;           //儲存某網格之設施建置評估值= Wbar/C
        bool isQ,           //儲存某網格是否合格得以建置新設施，合格則為1，否則為0
             isF,           //儲存某網格是否被挑選來建置新設施，是則為1，否則為0
             isB;           //儲存某網格是否已被指派其專屬之設施，是則為1，否則為0
    };
    int M,//區域縱軸座標範圍(1,2,...,M)
        N,//區域橫軸座標範圍(1,2,...,N)
        U=0,//合格之候選網格個數，計算完各網格之設施建置評估值才可得到
        L,//欲選擇設立之設施個數
        R,//設施涵蓋距離範圍，使用曼哈頓距離量測
        i,j,l,m,n,z, //for迴圈用
        k; //某網路格的編號
    double Qstar; //給定的設施建置評估值
    Zone  *V; //動態宣告MN長度之網格struct陣列;V[k],k=0,1,...,MN-1
    int  *VF; //動態宣告U長度之設施編號陣列;VF[l],l=0,1,...,U-1
    bool **E; //動態宣告U*MN長度的二維布林陣列E[U][MN]，用於記錄是否
              //候選點l=0,...,U-1是否與網格k=0,..,MN-1有專屬負責關係

    fstream inputfile;
    char filename[50];
    cout << "Enter filename: ";
    cin >> filename;
    inputfile.open(filename,ios::in);

    inputfile>>M>>N>>Qstar>>L>>R;
    cout<<"M="<<M<<"; "<<"N="<<N<<"; "<<"Q*="<<Qstar<<"; "<<"L="<<L<<"; "<<"R="<<R<<";"<<endl;
    V = new Zone [M*N];

    //W
    //W第一列
    printf("%6s","W");
    for(j=1;j<=N;j++){
        printf("%6d",j);
    }
    cout << endl;

    //W第二列之後
    for(i=1;i<=M;i++){
        printf("%6d",i);
        for(j=1;j<=N;j++){
            inputfile>>V[(i-1)*N+(j-1)].W;
            printf("%6.1f",V[(i-1)*N+(j-1)].W);
        }
        cout << endl;
    }

    cout << endl;

    //C
    //C第一列
    printf("%6s","C");
    for(j=1;j<=N;j++){
        printf("%6d",j);
    }
    cout <<endl;

    //C第二列之後
    for(i=1;i<=M;i++){
        printf("%6d",i);
        for(j=1;j<=N;j++){
            inputfile>>V[(i-1)*N+(j-1)].C;
            printf("%6.1f",V[(i-1)*N+(j-1)].C);
        }
        cout << endl;
    }
    //--- end --- PART 1: Declaration and read file-----------
    cout << endl;
    //---begin--- PART 2: Calculation---------------------------
    //將各網路格的座標輸入
    for(i=1;i<=M;i++){
        for(j=1;j<=N;j++){
            k = ((i-1)*N)+(j-1);
            V[k].b=(k%N)+1;
            V[k].a=((k-V[k].b+1)/N)+1;
        }
    }

    //計算Wbar
    for(i=1;i<=M;i++){
        for(j=1;j<=N;j++){
            k = (i-1)*N+(j-1);
            V[k].Wbar=0;
            for(m=1;m<=M;m++){
                for(n=1;n<=N;n++){
                    if((abs(i-m)+abs(j-n))<=R){
                        V[k].Wbar = V[k].Wbar+V[(m-1)*N+(n-1)].W;
                    }
                }
            }

        }
    }

    //計算Q和U
    for(k=0;k<(M*N);k++){
        V[k].Q = V[k].Wbar/V[k].C;
        if(V[k].Q>=Qstar){
            U = U+1;
        }
    }

    //初始化全部isQ,isF,isB為0
    for(k=0;k<(M*N);k++){
        V[k].isQ=0;
        V[k].isF=0;
        V[k].isB=0;
    }

    //創建VF陣列和E陣列
    int mark=0;
    VF = new int[U];
    E = new bool*[U];
    for(l=0;l<U;l++){
        E[l] = new bool[M*N];
        for(k=mark;k<(M*N);k++){
            if(V[k].Q>=Qstar){
                VF[l]=k;
                V[k].isQ=1;
                break;
            }
        }
        mark=k+1;
    }

    //初始化讓E每個元素皆為0
    for(l=0;l<U;l++){
        for(k=0;k<(M*N);k++){
            E[l][k] = 0;
        }
    }

    //若在可服務範圍內，則建立專屬關係
    for(l=0;l<U;l++){
        for(k=0;k<(M*N);k++){
            if(E[l][k]==1){
                continue;
            }
            else{
                if((abs(V[VF[l]].a-V[k].a)+abs(V[VF[l]].b-V[k].b))<=R){
                    E[l][k]=1;
                }
            }
        }
    }

    cout<<"U="<<U<<", "<<"VF: ";
    for(l=0;l<U;l++){
        cout<<VF[l]<<" ";
    }
    cout << endl;

    //印W_bar部分
    printf("%6s","W_bar");
    for(j=1;j<=N;j++){
        printf("%8d",j);
    }
    cout <<endl;

    for(i=1;i<=M;i++){
        printf("%6d",i);
        for(j=1;j<=N;j++){
            k = (i-1)*N+(j-1);
            printf("%8.1f",V[k].Wbar);
        }
        cout << endl;
    }
    cout << endl;

    //印Q部分
    printf("%6s","Q");
    for(j=1;j<=N;j++){
        printf("%6d",j);
    }
    cout <<endl;

    for(i=1;i<=M;i++){
        printf("%6d",i);
        for(j=1;j<=N;j++){
            k = (i-1)*N+(j-1);
            printf("%6.2f",V[k].Q);
        }
        cout << endl;
    }
    cout << endl;

    //印E部分
    printf("%6s","E");
    for(k=0;k<(M*N);k++){
        printf("%3d",k);
    }
    cout << endl;

    for(l=0;l<U;l++){
        printf("%6d",l);
        for(k=0;k<(M*N);k++){
            printf("%3d",E[l][k]);
        }
        cout << endl;
    }
    //--- end --- PART 2: Calculation---------------------------
    cout << endl;
    //---begin--- PART 3: Greedy Algorithm----------------------
    //再次初始化讓E每個元素 皆為0，因為我要的是貪婪演算法的專屬關係，不是可服務範圍的專屬關係而已
    for(l=0;l<U;l++){
        for(k=0;k<(M*N);k++){
            E[l][k] = 0;
        }
    }
    double tsd=0; //L facility total satisfied demands
    for(z=L;z>0;z--){
        cout<<"Iteration "<<L-z+1<<": select Facility ";
        int maxindex = (-1); //當前最大Wbar網格在U中的index
        int maxnumber = (-1); //當前最大Wbar網格編號
        double maxdemand = 0; //當前最大滿足需求
        double totalw = 0;  //當前總滿足需求
        for(l=0;l<U;l++){
            if(VF[l]==(-1)){ //若該候選點已用過則跳過不放進比較
                continue;
            }
            else if(V[VF[l]].Wbar>maxdemand){
                maxindex = l;
                maxnumber = VF[l];
                maxdemand = V[VF[l]].Wbar;
            }
        }
        cout<<maxnumber<<",satisfied ";
        VF[maxindex] = (-1);
        V[maxnumber].isF = 1;
        for(k=0;k<(M*N);k++){
            if(V[k].isB==1){
                continue;
            }
            else if((abs(V[maxnumber].a-V[k].a)+abs(V[maxnumber].b-V[k].b))<=R){
                E[maxindex][k]=1;
                V[k].isB=1;
                totalw = totalw + V[k].W;
            }
        }
        for(l=0;l<U;l++){
            if(VF[l]==(-1)){//若候選點已被選過則跳過
                continue;
            }
            else{
                V[VF[l]].Wbar = 0;
                for(k=0;k<(M*N);k++){
                    if(V[k].isB==1){//若該網格已有專屬設施則跳過
                        continue;
                    }
                    else if((abs(V[VF[l]].a-V[k].a)+abs(V[VF[l]].b-V[k].b)) <= R){
                        V[VF[l]].Wbar = V[VF[l]].Wbar + V[k].W;  //重新計算可服務範圍Wbar
                    }
                }
            }
        }
        cout<<totalw<<" demands for zone ";
        for(k=0;k<(M*N);k++){
            if(E[maxindex][k]==1){
                cout << k << ",";
            }
        }
        cout << endl;
        tsd = tsd + totalw;
    }
    cout << "Total satisfied demands: " << tsd;
    //--- end --- PART 3: Greedy Algorithm----------------------
    delete [] V;
    delete [] VF;
    for(i=0;i<M;i++){
        delete [] E[i];
    }
    delete [] E;
    return 0;
}
