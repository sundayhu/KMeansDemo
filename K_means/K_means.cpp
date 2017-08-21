// K_means.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <time.h>
#include <iostream>  
#include <sstream>  
#include <fstream>  
#include <vector>  
#include <math.h>  
#include <stdlib.h>  
#define k 7//�ص���Ŀ  
using namespace std;
//���Ԫ���������Ϣ  
typedef vector<double> Tuple;//�洢ÿ�����ݼ�¼  

int dataNum;//���ݼ������ݼ�¼��Ŀ  
int dimNum;//ÿ����¼��ά��  

//�ַ����ָ��
std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//��չ�ַ����Է������
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}
//��stringת��Ϊ��������
template <class Type>
Type stringToNum(const string& str)
{
	istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}
////��������Ԫ����ŷ�������  
//double getDistXY(const Tuple& t1, const Tuple& t2)
//{
//	double sum = 0;
//	for (int i = 1; i <= dimNum; ++i)
//	{
//		sum += (t1[i] - t2[i]) * (t1[i] - t2[i]);
//	}
//	return sqrt(sum);
//}
//��ȡ����Ԫ��֮����������ƶ�[0,1]
double getDistXY(const Tuple& t1, const Tuple& t2)
{
	double d = 0.0, a = 0.0, b = 0.0;
	double result;
	for (int i = 1; i <= dimNum; i++)
	{
		d += t1[i] * t2[i];
		a += t1[i] * t1[i];
		b += t2[i] * t2[i];
	}
	if (d == 0 && (sqrt(a) * sqrt(b)) == 0)
	{
		result = 0.0;
	}
	else
	{
		result = d / (sqrt(a) * sqrt(b));
	}
	//cout << result << endl;
	return result;
}
//�������ģ�������ǰԪ�������ĸ���  
int clusterOfTuple(Tuple means[], const Tuple& tuple){
	double dist = getDistXY(means[0], tuple);//�������ƶ�
	double tmp;
	int label = 0;//��ʾ������һ����  
	for (int i = 1; i<k; i++){
		tmp = getDistXY(means[i], tuple);
		if (tmp>dist) { dist = tmp; label = i; }
	}
	return label;
}
//��ø����ؼ���ƽ�����  �ص����ĵ��������ƶ�֮��
double getVar(vector<Tuple> clusters[], Tuple means[]){
	double var = 0;
	for (int i = 0; i < k; i++)
	{
		vector<Tuple> t = clusters[i];
		for (int j = 0; j< t.size(); j++)
		{
			var += getDistXY(t[j], means[i]);
		}
	} 
	return var;

}
//��õ�ǰ�صľ�ֵ�����ģ�  
Tuple getMeans(const vector<Tuple>& cluster){

	int num = cluster.size();
	Tuple t(dimNum + 1, 0);
	for (int i = 0; i < num; i++)
	{
		for (int j = 1; j <= dimNum; ++j)
		{
			t[j] += cluster[i][j];
		}
	}
	for (int j = 1; j <= dimNum; ++j)
		t[j] /= num;
	return t;
	//cout<<"sum:"<<sum<<endl;  
}

void print(const vector<Tuple> clusters[])
{
	for (int lable = 0; lable<k; lable++)
	{	
		vector<Tuple> t = clusters[lable];
		cout << "��" << lable + 1 << "���ع��е�Ԫ�����Ϊ��" << t.size() << " ";
		cout << "���ǵı�ŷֱ��ǣ�" << endl;
		for (int i = 0; i<t.size(); i++)
		{
			cout << t[i][0] << " ";
			/*cout << i + 1 << ".(";
			for (int j = 0; j <= dimNum; ++j)
			{
				cout << t[i][j] << ", ";
			}
			cout << ")\n";*/
		}
		cout << endl;
		cout << endl;
	}
}

void KMeans(vector<Tuple>& tuples){
	vector<Tuple> clusters[k];//k����  
	Tuple means[k];//k�����ĵ�  
	int i = 0;
	//һ��ʼ���ѡȡk����¼��ֵ��Ϊk���ص����ģ���ֵ��  
	srand((unsigned int)time(NULL));
	for (i = 0; i < k;){
		int iToSelect = rand() % tuples.size();
		//int iToSelect = tuples.size() / k*i+1;
		if (means[iToSelect].size() == 0)
		{
			for (int j = 0; j <= dimNum; ++j)
			{
				means[i].push_back(tuples[iToSelect][j]);
			}
			++i;
		}
	}
	int lable = 0;
	//�������ĸ��ظ�ֵ  
	for (i = 0; i != tuples.size(); ++i){
		lable = clusterOfTuple(means, tuples[i]);//���������ĸ��صı��
		clusters[lable].push_back(tuples[i]);
	}
	double oldVar = -1;
	double newVar = getVar(clusters, means);//��ȡ�������ƽ����
	cout << "��ʼ�ĵ��������ƽ����Ϊ��" << newVar << endl<<endl;
	//��ʼ���� Խ��Խ�������Ϊ1
	int t = 0;
	while (abs(newVar-oldVar)>0.0001&&t<20) //���¾ɺ���ֵ����0.0001��׼����ֵ���������Ա仯ʱ���㷨��ֹ  
	{
		cout << "�� " << ++t << " �ε�����ʼ��" << endl;
		for (i = 0; i < k; i++) //����ÿ���ص����ĵ�  
		{
			means[i] = getMeans(clusters[i]);
		}
		oldVar = newVar;
		newVar = getVar(clusters, means); //�����µ�׼����ֵ  
		for (i = 0; i < k; i++) //���ÿ����  
		{
			clusters[i].clear();
		}
		//�����µ����Ļ���µĴ�  
		for (i = 0; i != tuples.size(); ++i){
			lable = clusterOfTuple(means, tuples[i]);
			clusters[lable].push_back(tuples[i]);
		}
		cout << "�˴ε���֮����������ƽ����Ϊ��" << newVar << endl;
	}
	cout << endl;
	cout << "�� k = " << k << "ʱ��";
	cout << "The result is:\n" << endl;
	print(clusters);
}
int main(){
	string fname;
	/*cout << "�����������ݵ��ļ����� ";
	cin >> fname;
	cout << endl << " ����������: ά�� ������Ŀ" << endl;
	cout << endl << " ά��dimNum: ";
	cin >> dimNum;
	cout << endl << " ������ĿdataNum: ";
	cin >> dataNum;*/
	fname = "tf_idf.txt";
	dimNum = 1826;
	dataNum = 143;
	ifstream infile(fname);
	if (!infile){
		cout << "���ܴ�������ļ�" << fname << endl;
		return 0;
	}
	vector<Tuple> tuples;
	//���ļ����ж�������  
	for (int i = 0; i<dataNum && !infile.eof(); ++i)
	{
		string str;
		getline(infile, str);
		//istringstream istr(str);
		vector<string> temp;
		temp = split(str, " ");
		Tuple tuple(dimNum + 1, 0);//��һ��λ�ô�ż�¼��ţ���2��dimNum+1��λ�ô��ʵ��Ԫ��  
		tuple[0] = i + 1;
		for (int j = 1; j <= dimNum; j++)
		{
			tuple[j] = stringToNum<double>(temp[j - 1]);
			//istr >> tuple[j];
		}
		//cout << tuple.size() << endl;
		tuples.push_back(tuple);
	}
	//cout << tuples.size() << endl;
	cout << endl << "��ʼ����........" << endl<<endl;
	KMeans(tuples);
	return 0;
}