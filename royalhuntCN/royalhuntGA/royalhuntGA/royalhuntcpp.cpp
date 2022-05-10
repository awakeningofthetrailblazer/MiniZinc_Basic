#include<iostream>
#include<algorithm>
#include<fstream>
#include<vector>
#include<ctime>
#include<unordered_map>
#include<string>

#define cin fin
//#define cout fout
#define INF 1000000
#define Chromosome_Num 100//遗传过程中的群体大小


using namespace std;

const int emperor = 1;

const int breakCost = 200;

int Max_Iter = 200000;//最大遗传迭代次数

string filename = "royalhunt_4";//样例名称

ifstream fin(filename + ".txt", ios::in);
ofstream fout(filename+"out.csv",ios::out);
ofstream fBout("bestRecOut.csv", ios::app);

vector<vector<int>> chromosome;//种群
vector<int> chromo_obj;//对应目标函数值
vector<int> parentA, parentB;//用于杂交的父代
vector<int> Offspring;//子代


//参数
int N, M;
int MTN;
int hbr,temphbr=INF;
vector<int> Rank, Ability, Beauty, Speed;
vector<vector<int>> Enjoy;

bool optBetter = false;

//变量
vector<int> curRes;//初始动态解

int Obj;//目标函数值

int bestObj;//最优目标

vector<int> bestRes;//最优解

void Init();

void Genetic_Construction();

void Genetic_Crossover();

void Genetic_Mutation(vector<int>& cs);

void Genetic_Update();

void Output(vector<int> cs);

int objCal(vector<int> cs);

void optCurRes();

int main() {

	clock_t startTime, endTime;

	startTime = clock();

	Init();

	endTime = clock();
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC
		<< "s" << endl;

	Genetic_Construction();

	endTime = clock();
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC
		<< "s" << endl;


	for (int i = 0; i < Max_Iter;i++) {
		
		//cout <<endl<<"iter "<< i + 1 << endl;

		Genetic_Crossover();//染色体交叉；

		Genetic_Mutation(Offspring);//染色体变异；

		Genetic_Update();//生成下一代群体；

		//cout << "Offspring: ";
		//Output(Offspring);
		//cout << "Obj = " << Obj << ';' << endl;

		//cout << "temp_best: ";
		//Output(bestRes);
		//cout << "bestObj = " << bestObj << ';' << endl;

		fout << i << ',' << bestObj << ',' << hbr << ',' << Obj << ',' << temphbr <<','<<"-488"<< endl;
	}

	endTime = clock();
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC
		<< "s" << endl;

	cout << endl << "finished!" << endl;

	Output(bestRes);
	cout << "bestObj = " << bestObj << ';' << endl;

	fBout << filename << ',' << bestObj << ',' << temphbr << ',' 
		<< (double)(endTime - startTime) / CLOCKS_PER_SEC << ',' 
		<< Chromosome_Num << ',' << Max_Iter << endl;

	return 0;
}

void Init() {
	cin >> N >> M;
	MTN = (N + 1)*(M + 1);

	int x;
	Rank.push_back(0);
	for (int i = 1; i <= N; i++) {
		cin >> x;
		Rank.push_back(x);
	}
	Ability.push_back(0);
	for (int i = 1; i <= N; i++) {
		cin >> x;
		Ability.push_back(x);
	}

	Beauty.push_back(0);
	for (int i = 1; i <= M; i++) {
		cin >> x;
		Beauty.push_back(x);
	}

	Speed.push_back(0);
	for (int i = 1; i <= M; i++) {
		cin >> x;
		Speed.push_back(x);
	}

	vector<int> temp;
	Enjoy.push_back(temp);
	for (int i = 0; i <= N; i++) {
		temp.clear();
		temp.push_back(0);
		for (int j = 1; j <= M; j++) {
			cin >> x;
			if (x >= 0)temp.push_back(x);
			else temp.push_back(-INF);
		}
		Enjoy.push_back(temp);
	}

	cout << "finish reading data !" << endl;
}

void Genetic_Construction() {
	bestObj = -INF;
	//初始种群生成
	for (int i = 0; i <= max(M, N); i++) curRes.push_back(i);

	optCurRes();


	for (int i = 0; i < Chromosome_Num; i++) {

		//cout << "Chromosome " << i + 1 << endl;
		chromosome.push_back(curRes);
		chromo_obj.push_back(objCal(curRes));

		//Output(curRes);

		//cout << "Obj = " << chromo_obj[i] << ';' << endl;

		if (chromo_obj[i] > bestObj) {
			bestObj = chromo_obj[i];
			bestRes = chromosome[i];
		}

		Genetic_Mutation(curRes);
	}

	cout << "Constructed !" << endl;
}

void Genetic_Crossover() {
	//cout << "crossover" << endl;
	//用轮盘赌方法从群体中随机选择两个父代

	parentA = chromosome[rand() % (Chromosome_Num)];
	parentB = chromosome[rand() % (Chromosome_Num)];

	//对选取的父代进行杂交得到子代

	Offspring = parentA;
	for (int i = 1; i < parentA.size(); i++) {
		if (i == emperor) continue;
		if (parentA[i] != parentB[i]) {
			if (rand() % 2) {
				for (int j = i + 1; j < parentA.size(); j++) {
					if (parentA[i] == parentB[j]) {
						swap(Offspring[i], Offspring[j]);
						break;
					}
				}
			}
		}
	}
}

void Genetic_Mutation(vector<int>& cs) {
	//cout << "mutation" << endl;
	//在一定的概率下，将子代的两个位置互换；

	//cout << cs[emperor] << endl;

	int se, se1, se2, h1, h2;
	se = rand() % N + 1;
	
	if (se!=emperor && cs[se] <= M){ //&& 
		//Enjoy[emperor][cs[emperor]] < Enjoy[se][cs[se]]) {//同时检测皇帝约束
		swap(cs[emperor], cs[se]);
	}

	//cout << "test1" << endl;

	//硬约束

	se1 = rand() % max(M, N) + 1;
	se2 = rand() % N + 1;
	h1 = cs[se1];
	h2 = cs[se2];

	if (se2 == emperor) return;
	if (h2 > M) return;

	//cout <<"se: "<< se1 << ' ' << se2 << endl;
	//cout <<"h: "<< h1 << ' ' << h2 << endl;

	if (se1 <= N && h1 <= M && se2 <= N && h2 <= M) {
		if ((Rank[se1] - Rank[se2]) * (Beauty[h1] - Beauty[h2]) < 0 ||
			(Speed[h1] - Speed[h2]) * (Ability[se1] - Ability[se2]) < 0)
			swap(cs[se1], cs[se2]);
	}
	else if (se1 > N && h1 <= M && se2 <= N && h2 <= M) {//有马无人
		//if(Speed[h1] < Speed[h2]) 
			swap(cs[se1], cs[se2]);
	}
	else if (se1 <= N && h1 > M && se2 <= N && h2 <= M) {//有人无马
		//if (Rank[se1] > Rank[se2]) 
			swap(cs[se1], cs[se2]);
	}

	//cout << "se: " << se1 << ' ' << se2 << endl;
	//cout << "h: " << cs[se1] << ' ' << cs[se2] << endl;
}

void Genetic_Update() {
	

	//更新群体
	Obj = objCal(Offspring);

	//cout << "update" << endl;

	int MinValue = INF, MinSign;

	for (int i = 0; i < Chromosome_Num; i++) {
		if (chromo_obj[i] < MinValue) {

			MinValue = chromo_obj[i];

			MinSign = i;
		}
	}

	if (MinValue < Obj) {
		chromosome[MinSign] = Offspring;
		if (optBetter && hbr) return;
		if (bestObj < Obj || hbr==0) {
			if(hbr==0) temphbr = hbr;
			bestObj = Obj;
			bestRes = Offspring;
		}
	}
}

void Output(vector<int> cs) {
	for (int i = 1; i <= N;i++) {
		if (i == 1) cout << "horse = [";
		else cout << ", ";
		if (cs[i] <= M)
			cout << cs[i];
		else cout << '0';
		if (i == N) cout << "];" << endl;
	}
}

int objCal(vector<int> cs) {
	int res = 0;

	for (int i = 1; i <= N; i++) {
		if (cs[i] && cs[i] <= M) {
			res += Enjoy[i][cs[i]];
		}
	}

	

	/*强约束惩罚*/
	/*
	• 皇上需要⽐其他⼈都更享受这次狩猎。

	• 除⾮马的数量不够，否则所有⼈都应该骑马。

	• 如果⼀个要员⽐另外⼀个要员阶级更⾼，那么就应该符合以下任意⼀种情况：
		（a）等级较⾼的要员骑的马应该⽐等级较低的要员骑的马更骏伟或两者同等俊伟；
		（b）等级较低的要员没有骑马；
		（c）两个⼈都没有骑马。
	*/

	int br = 0;
	hbr = 0;

	for (int i = 1; i <= N; i++) {
		if (i == emperor || cs[i] > M) continue;
		if (cs[emperor] > M) hbr++;
		else if (Enjoy[emperor][cs[emperor]] <= Enjoy[i][cs[i]]) hbr++;
	}

	//cout << "test4" << endl;

	for (int i = 1; i <= N; i++) {
		if (cs[i] > M) continue;
		for (int j = 1; j <= N; j++) {
			if (cs[j] > M) {
				if (Rank[i] < Rank[j]) hbr++;
			}
			else {
				if ((Rank[i] - Rank[j]) * (Beauty[cs[i]] - Beauty[cs[j]]) < 0) hbr++;
			}
		}
	}

	


	/*弱约束惩罚*/
	/*
	• 如果⼀匹马⽐另外⼀匹马更快，那么就应该符合以下任意⼀种情况：
		（a）骑快马的⼈的骑马技术不应该⽐骑慢马的⼈的更差；
		（b）没有⼈骑快马；
		（c）两匹马都没有⼈骑。
	*/

	vector<int> court;
	for (int i = 0; i <= max(M, N); i++) court.push_back(0);
	for (int i = 1; i <= max(M, N); i++) court[cs[i]] = i;

	

	for (int i = 1; i <= M; i++) {
		if (court[i] > N) continue;
		for (int j = 1; j <= M; j++) {
			if (Speed[i] > Speed[j] && (court[j]>N || Ability[court[i]] < Ability[court[j]])) {
				br++;
			}
		}
	}

	res -= br * breakCost;

	res -= hbr * breakCost * 10;

	if (!hbr) optBetter = true;

	//cout << "hbr = " << hbr << endl;

	return res;
}

void optCurRes() {
	int a, b, maxenjoy = -INF,temp;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (maxenjoy < Enjoy[i][j]) {
				maxenjoy = Enjoy[i][j];
				a = i;
				b = j;
			}
		}
	}

	swap(curRes[emperor], curRes[a]);
}