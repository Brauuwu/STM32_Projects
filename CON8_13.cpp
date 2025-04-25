#include<bits/stdc++.h>
using namespace std;

vector<char> v = {'0', '1', '2', '3', '4', '5'};

int main(){
	int t; cin >> t;
	while(t--){
		int l, r;
		cin >> l >> r;
		queue<string> q;
		for(char x:v){
			if(x!='0') q.push(string(1,x));
		}
		int cnt = 0;
		while(!q.empty()){
			string s = q.front(); q.pop();
			int val = stoi(s);
			if(val >= l && val <= r) cnt++;
			else if(val > r) continue;
			bool visited[6] = {false};
			for(char c:s) visited[c-'0'] = true;
			for(auto x:v){
				if(!visited[x-'0']){
					visited[x-'0'] = true;
					q.push(s + x);
				}
			}
		}
		cout << cnt << endl;
	}
    return 0;
}
