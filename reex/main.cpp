#include<iostream>
#include<string.h>
#include<stack>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

const int opLen = 5;
char ops[opLen][2] = {
	{'#', '0'},
	{'+', '1'},
	{'-', '1'},
	{'*', '2'},
	{'/', '2'},
};
bool compute(const char* express, int*result);
bool ckExpress(const char* express);

int main(void){
	const int exlen = 1024;
	int result;
	bool isSucceed = false;
	char express[exlen];
	memset(express, 0, exlen);
	cout<<"express:";
	cin>>express;
	bool isGoodEx = ckExpress(express);
	if( isGoodEx==false ){
		cout<<"Bad format"<<endl;
		return 0;
	}
	isSucceed = compute(express, &result);
	if( isSucceed==true ){
		cout<<express<<" = "<<result<<endl;
	}else{
		cout<<"failed."<<endl;
	}
	
	//compute
	//output
	
	return 0;
}
bool isNum(const char c){
	if( c>='0' && c<='9' ){
		return true;
	}
	return false;
}
bool isOp(const char c){
	if( c=='(' || c==')' ){
		return true;
	}
	char* p = (char*) ops;
	while( p!=NULL && *p!='\0' ){
		if(*p==c){
			return true;	
		}
		p++;
	}

	return false;
}
int getPriority( char op ){
	int priority = -1;
	for( int i=0; i<opLen; i++ ){
		if( ops[i][0]==op ){
			return ops[i][1]-'0';
		}
	}
	return -1;
	
}
bool lCompute(char op, int n1, int n2, int *result){
	bool bSuccess = true;
	switch(op){
	case '+':
		*result = n1+n2;
		break;
	case '-':
		*result = n1-n2;
		break;
	case '*':
		*result = n1*n2;
		break;
	case '/':
		if( n2==0 ){
			perror("除数为灵");
			exit(1);
		}
		*result = n1/n2;
		break;
	default:
		bSuccess = false;
		break;
	}
	return bSuccess;;
}
bool compute(const char* express, int*result){
	stack<char> sOp;
	stack<int> sNum;
	sOp.push('#');
	char * p = (char*) express;
	while( p!=NULL && *p!='\0' ){
		if( isNum(*p) ){
			//get number
			int nNumber=0;
			while( isNum(*p) ){
				nNumber = nNumber*10 + (*p-'0');
				p++;
			}
			//push
			sNum.push(nNumber);
		}else if(isOp(*p)){
			//get operator
			if( *p=='(' ){
				sOp.push(*p);
			}else if(*p==')'){
				while( sOp.top()!='(' ){
					//pop and compute
					char cOp = sOp.top();
					sOp.pop();
					int nNumber2 = sNum.top();
					sNum.pop();
					int nNumber1 = sNum.top();
					sNum.pop();
					int nResult ;
					lCompute(cOp, nNumber1, nNumber2, &nResult);
					sNum.push(nResult);
				}
				//here is ( or empty so pop (
				sOp.pop();
			}else{
				while( !sOp.empty() && getPriority(sOp.top())>=getPriority(*p)\
						&& sOp.top()!='#' ){
					//compute and pop
					char cOp = sOp.top();
					sOp.pop();
					int nNumber2 = sNum.top();
					sNum.pop();
					int nNumber1 = sNum.top();
					sNum.pop();
					int nResult ;
					lCompute(cOp, nNumber1, nNumber2, &nResult);
					sNum.push(nResult);
				}
				sOp.push(*p);
			}
			p++;
		}
	}
	while( !sOp.empty() && sOp.top()!='#' ){
		//compute and pop
		char cOp = sOp.top();
		sOp.pop();
		int nNumber2 = sNum.top();
		sNum.pop();
		int nNumber1 = sNum.top();
		sNum.pop();
		int nResult ;
		lCompute(cOp, nNumber1, nNumber2, &nResult);
		sNum.push(nResult);
	}
	if( !sOp.empty() && sOp.top()=='#' ){
		*result = sNum.top();
		return true;
	}else{
		return false;
	}
}
bool ckExpress(const char* express){
	char *p = (char*)express;
	while( p!=NULL && *p!='\0' ){
		if( (!isNum(*p)) && (!isOp(*p)) ){
			return false;
		}
		p++;
	}
	return true;

}
