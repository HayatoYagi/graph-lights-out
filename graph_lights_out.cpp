#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>
#include <map>

using namespace std;

int n; // 頂点数
int m; // 辺数
vector<vector<int>> adj; // 隣接リスト. 各adj[i]は昇順にソート済とする

/**
 * 多重集合Aから集合Bに含まれるないものを取り除く
 * 
 * @param a 多重集合A. 昇順にソート済とする
 * @param b 集合B. 昇順にソート済とする
 * @return 多重集合Aのうち集合Bに含まれるもの. 昇順にソート済になる
 */
vector<int> submultiset(const vector<int>& a, const vector<int>& b){
  vector<int> res;
  int ai = 0, bi = 0;
  while(ai < a.size() && bi < b.size()){
    if(a[ai] == b[bi]){
      res.push_back(a[ai]);
      ++ai;
    }else if(a[ai] > b[bi]){
      ++bi;
    }else{
      ++ai;
    }
  }
  return res;
}

// memoization
map<set<int>,vector<int>> cache;

/**
 * 各部分グラフに対して再帰的にグラフ反転の操作列を求める
 * 
 * @param vs {0,1,...,n-1}の部分集合. この頂点集合から誘導される部分グラフに対してランプ反転を行う
 * @return 操作する頂点番号のリスト
 * 
 * 空間計算量を抑えるため, 引数は隣接リストではなく部分グラフの頂点集合とした.
 */
vector<int> solve(set<int> vs){
  if(cache.count(vs))return cache[vs];

  vector<int> res(vs.begin(),vs.end()); // すべてのvで(B)だった時の出力
  bool degsAreAllEven = true;

  for(int v : vs){
    // 部分グラフでのvの近傍と次数を求める
    vector<int> neighbors;
    set_intersection(vs.begin(),vs.end(),
                    adj[v].begin(),adj[v].end(),
                    inserter(neighbors,neighbors.end()));
    int d = neighbors.size(); // 部分グラフにおけるvの次数

    if(d % 2 == 0)continue;
    degsAreAllEven = false;

    // vを除いたグラフに対し再帰的に[solve]を呼び出す
    set<int> vs_copy = vs;
    vs_copy.erase(v);
    vector<int> s = solve(vs_copy);

    // sによりvも反転するなら(A), sを出力して終了
    vector<int> sortedS = s;
    sort(sortedS.begin(), sortedS.end());
    if(submultiset(sortedS,neighbors).size() % 2){
      cout << "vs:";
      for(auto v : vs)cout << " " << v;
      cout << endl;
      for(int v : s)cout << " " << v;
      cout << endl;
      return cache[vs] = s;
    }
    
    // resの後ろにsを連結する
    res.insert(res.end(),s.begin(),s.end());
  }

  cout << "vs:";
  for(auto v : vs)cout << " " << v;
  cout << endl;
  if(degsAreAllEven){
    // 全頂点の次数が偶数なら全頂点のボタンを押す
    for(int v : vs)cout << " " << v;
    cout << endl;
    return cache[vs] = vector<int>(vs.begin(),vs.end());
  }else{
    // 全てのvで(B)だったら
    for(auto v : res)cout << " " << v;
    cout << endl;
    return cache[vs] = res;
  }
}

int main(){
  /**
   * 入力の仕方の例
   * 
   * 1行目に頂点数nと辺数mを空白区切りで入力する
   * 2行目からのm行に各辺の両端点の頂点番号v,u (0 <= v,u < n)を入力する
   * 
   * 入力例1
6 7
0 1
0 5
1 5
1 2
1 4
2 4
3 4
   * 
   * 入力例2 （演習2の3x4のタイル）
12 17
0 1
1 2
2 3
4 5
5 6
6 7
8 9
9 10
10 11
0 4
4 8
1 5
5 9
2 6
6 10
3 7
7 11
   *
   */
  cin >> n >> m;
  adj.resize(n);
  for(int i = 0; i < m; ++ i){
    int v,u;
    cin >> v >> u;
    adj[v].push_back(u);
    adj[u].push_back(v);
  }
  for(int i = 0; i < n; ++i){
    sort(adj[i].begin(), adj[i].end());
  }

  // {0,1,...,n-1}に対して[solve]を呼び出す
  vector<int> vs(n);
  iota(vs.begin(),vs.end(),0);
  vector<int> ans = solve(set<int>(vs.begin(),vs.end()));

  // 操作列の出力
  cout << "output:";
  for(int v : ans){
    cout << " " << v;
  }
  cout << endl;
  cout << "操作列の長さ: " << ans.size() << endl;

  // 偶数回押すものは無視した操作列（最初からこちらのみ計算すれば計算量を落とせるが割愛）
  vector<int> count(n);
  for(int v : ans){
    count[v]++;
  }
  cout << "deducted:";
  for(int i = 0; i < n; ++i){
    if(count[i] % 2)cout << " " << i;
  }
  cout << endl;
  return 0;
}
