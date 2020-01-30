#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
using namespace std;
typedef double db;
const int N = 1000;
const db EPS = 1e-6;
const db MAX_DB = 1e18;

db a[N][N * 2];         //coefficient matrix
db ans[N * 2];          //answer array

int type[N * 2];        //type of restriction <= or >= or =
db CTwoPhase[N * 2];    

int n, m;               //n:变量数量 m:约束个数
int id[N * 2];          //标记每一个约束对应的基变量
int id_res[N * 2];

int k = 1;                  //表示解的情况

int totVariableSlack;   //真正变量+松弛变量的数量
int totVariable;        //真正变量的数量
int manMadeVar;         //添加人工变量的数量

void pivot(int out, int in, int isFirstPhase) {
    int outVar = id[out];
    id[out] = in;//out是出基的那一行
    
    db value = a[out][in];//入基变量的值
    //入基变量系数归一
    for(int i = 0; i <= totVariableSlack; i++)
        a[out][i] /= value;

    if(isFirstPhase) {  //如果是first phase
        value = CTwoPhase[in]; 
        for(int j = 0; j <= totVariableSlack; j++)
            CTwoPhase[j] -= value * a[out][j];

        for(int i = 1; i <= m; i++) {
            if(i != out) {
                value = a[i][in];
                for(int j = 0; j <= totVariableSlack; j++)
                    a[i][j] -= value * a[out][j];
            }
        }
    } else {//如果是second phase
        for(int i = 0; i <= m; i++) {
            if(i != out) {
                value = a[i][in];
                for(int j = 0; j <= totVariableSlack; j++) 
                    a[i][j] -= value * a[out][j];
            }
        }
    }

}

int main() {
    clock_t startTime, endTime;
    int method = 1;
    int haseq = 0;
    startTime = clock();

    cin >> n >> m;
    int i, j, l, e;
   
   totVariableSlack = n;
   totVariable = n;
    for(j = 1; j <= n; ++j) {
        cin >> a[0][j];
        a[0][j] *= -1; //change min to max
    }

    for(i = 1; i <= m; ++i) {
        for(j = 1; j<= n; ++j)
            cin >> a[i][j];
        cin >> a[i][0];
        cin >> type[i];
    }

    /**
     * e = 1表示x>=0
     * e = 0表示无约束
     * e = -1表示x<=0
     **/
    for (int i = 1; i <= n; ++i) {
        //全部处理成x>=0的形式
        int e;
        cin >> e;
        if (e == -1) {
            for (int j = 0; j <= m; ++j)
                a[j][i] *= -1;
            id_res[i] = -1;
        }
        else if (e == 0){
            //x无约束处理成x=x1-x2, x1>=0, x2>=0
            ++totVariable; ++totVariableSlack;
            for (int j = 0; j <= m; ++j)
                a[j][totVariable] = -a[j][i];
            id_res[i] = totVariable;//记录添加的变量的位置，最后用来恢复
        }
    }
    //根据type,也就是约束类型,添加松弛变量
    for(int i = 1; i <= m; ++i) {
        if(type[i] == -1) {
            totVariableSlack++;
            a[i][totVariableSlack] = 1;
        } 
        else if(type[i] == 1){
            totVariableSlack++;
            a[i][totVariableSlack] = -1;
        } 
        else haseq = 1; //存在等式约束
        id[i] = totVariableSlack;
    }
    //把所有bi都变成>=0
    for(int i = 1; i <= m; ++i) {
        if(a[i][0] < 0)
            for(int t = 0; t <= totVariableSlack; t++)
                a[i][t] = -a[i][t];
    }

    int flag = 0;
    for(int i = 1; i <= totVariableSlack; i++) {
        if(a[0][i] > 0) {
            flag = 1;
            break;
        }
    }
    //flag=0，说明检验数都<=0,满足对偶的约束，用对偶单纯形法
    // cout << flag << method << haseq << endl;
    if(flag == 0 && method == 1 && haseq == 0) {
        for(i = 1; i <= m; i++)
            if(a[i][totVariable + i] == -1)
                for(int j = 0;  j < totVariableSlack + 1; j++) 
                    a[i][j] *= -1;
        int suss = 0;
        while(true) {
            db min = -EPS;
            int out = 0;
            for(int i = 1; i <= m; i++) {
                if(a[i][0] < min) {
                    min = a[i][0];
                    out = i;
                }
            }
            if(out == 0) {
                suss = 1;
                break;
            }
            int in = 0;
            min = -EPS;
            for(int i = 1; i <= totVariableSlack; i++) {
                cout << a[out][i] <<endl;
                if(a[out][i] < min && (in == 0 || a[0][i]/a[out][i] < a[0][in]/a[out][in]))
                    in = i;
            }
            if(in == 0) break;
            pivot(out, in, 0);
        }
        if(suss == 0) {
            k = -1;
            // cout << "No solution"<<endl;
        }
        else {
            cout << "k = " << k << endl;
            cout << "result = " << a[0][0] << endl;
            for (i = 1; i <= m; ++i)
                ans[id[i]] = a[i][0];
            cout << "===========================" << endl;
            for (i = 1; i <= n; ++i) {
                if (id_res[i] == -1)
                    ans[i] *= -1;
                else if (id_res[i] > 0)
                    ans[i] -= ans[id_res[i]];
                cout <<  "x[" << i << "]:" << ans[i] << endl;
            }
            cout << "===========================" << endl;
            
        } 
        return 0; 
    }
    
/**
 * 两阶段法
 **/
    int OriginalVariableSlack = totVariableSlack;
    for(int i = 1; i <= m; ++i) {
        a[i][++totVariableSlack] = 1;
        ++manMadeVar;
        CTwoPhase[totVariableSlack] = 1;    //目标函数
        id[i] = totVariableSlack;           //人工变量为基变量
    }

    //将基变量对应的检验数都变成0
    for(j = 1; j <= m; ++j)
        for(int i = 0; i <= totVariableSlack; i++)
            CTwoPhase[i] -= a[j][i];

    while(true) {
        //找到一个不符合条件到检验数

        int chooseIn;//选择的入基变量
        for(i = 1; i <= totVariableSlack; ++i)
            if(CTwoPhase[i] < -EPS)
                break;

        if((chooseIn = i) > OriginalVariableSlack)
            break;//找到人工变量里面了，不行！！！
        
        db min = MAX_DB;
        db ratio = 0;
        int out = 0;
        //选择出基变量
        for(i = 1; i <= m; ++i) {
            if(a[i][chooseIn] > EPS && (ratio = (a[i][0] / a[i][chooseIn])) < min) {
                min = ratio;
                out = i;//第i行对应第基变量出基
            }
        }
        if (!out) {
            k = -1;
            cout << "k = " << k << endl;
            endTime = clock();
            cout << "\nTotle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
            return 0;
        }
        pivot(out, chooseIn, 1);
    }

    if(CTwoPhase[0] > EPS || CTwoPhase[0] < -EPS) {
        k = -1;
        cout << "k = " << k << endl;
        endTime = clock();
        cout << "\nTotle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
        // cout << "No solution" << endl;
        return 0;
    }
    
    //减掉人工变量 
    totVariableSlack -= manMadeVar;

    //将基变量对应的检验数都变成0
    for(int i = 1; i <= m; i++) {
        double value = a[0][id[i]];
        if(a[0][id[i]] != 0) {
            for(int j = 0; j <= totVariableSlack; j++) {
                a[0][j] -= value * a[i][j];
            }
        }
    }


    while(true) {
        //找到一个不符合条件的检验数
        int chooseIn = -1;//选择的入基变量
        for(i = 1; i <= totVariableSlack; ++i)
            if(a[0][i] > EPS)
                break;
        chooseIn = i;
        if(chooseIn > totVariableSlack) break;
        
        db min = MAX_DB;
        db ratio = 0;
        int out = 0;
        //选择出基变量
        for(i = 1; i <= m; ++i) {
            if(a[i][chooseIn] > EPS && (ratio = (a[i][0] / a[i][chooseIn])) < min) {
                min = ratio;
                out = i;//第i行对应第基变量出基
            }
        }
        if (!out) { //如果无法找到出基变量
            k = -1;
            cout << "k = " << k << endl;
            //cout << "no base out " << 0;
            endTime = clock();
            cout << "\nTotle Time : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
            return 0;
        }
        //找到了出基变量，进行旋转
        pivot(out, chooseIn, 0);
    }
    cout << "k = " << k << endl;
    cout << setiosflags(ios::fixed);
    cout << "result = " << a[0][0] << endl;

    for (i = 1; i <= m; ++i)
        ans[id[i]] = a[i][0];

    cout << "===========================" << endl;
    for (i = 1; i <= totVariable; ++i) {
        if (id_res[i] == -1)
            ans[i] *= -1;
        else if (id_res[i] > 0)
            ans[i] -= ans[id_res[i]];
        cout <<  "x[" << i << "]:" << ans[i] << endl;
    }
    cout << "==========================" << endl;

    endTime = clock();
    cout << "\nTotle Time : " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
