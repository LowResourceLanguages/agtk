#include <iostream>
#include <ag/AGAPI.h>

using namespace std;

int main()
{
  cout << "----------" << endl;
  cout << "loading..." << endl;
  Load("MyFormat", "inputfile.txt");

  cout << "----------" << endl;
  cout << "storing..." << endl;
  Store("MyFormat", "outputfile.txt", "Test:123");
}

