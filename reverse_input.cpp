#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main()
{
  vector<pair<double, double>> vertices;
  ifstream file;
  file.open("input_py.txt");
  int n;
  file >> n;
  for (int i = 0; i < n; i++)
  {
    double x, y;
    file >> x >> y;
    vertices.push_back({x, y});
  }
  file.close();
  //write to the same file
  file.open("input_py.txt", ios::out | ios::trunc);
  file << n << endl;
  for (int i = 0; i < n; i++)
  {
    file << vertices[i].first << " " << vertices[i].second << endl;
  }
  file.close();
}