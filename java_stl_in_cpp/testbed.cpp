#include <iostream>
#include <cstring>
#include <string>
#include "ArrayList.h"
#include "HashMap.h"
#include "LinkedList.h"
#include "Utility.h"
using namespace std;


class Hashint{
	public:
		static int hashcode(int obj){
			return obj;
		}
};

int main(){
    using namespace std;
   /* ArrayList<string> a;
    a.add("ArrayList");
    for (ArrayList<double>::Iterator i(a.iterator()); i.hasNext(); )
		cout << i.next();
	LinkedList<string>b;
	b.add("LinkedList");*/
	/*    LinkedList<int> jjj;
    cout<<jjj.size()<<'\n';
    {
    LinkedList<int> iii;
    cout<<iii.size()<<'\n';
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(300);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
    iii.add(100);
    iii.add(200);
    iii.add(300);
    iii.add(400);
	cout <<"*";
    iii.get(5)=1000000;
	cout <<"*";
    iii.set(6,40044);
	cout<<"*";
	jjj = iii.subList(5,30);
	cout <<"*";
    //iii.clear();
    }
    cout<<jjj.size()<<'\n';
    for(LinkedList<int>::ConstIterator i(jjj.constIterator());i.hasNext();){
        cout<<i.next()<<'\n';
    }
    cout<<" rr ----------\n\n\n";
//    cout<<jjj.removeIndex(5);
//	cout<<jjj.indexOf(40044);

    for(LinkedList<int>::Iterator i(jjj.iterator());i.hasNext();){
        cout<<i.next()<<' '<<jjj.size()<<'\n';
        i.remove();
    }
	string s("Abc");
	ArrayList<char >e;
	for (int i='a';i<='z';i++)
	{
		e.add(i);
		for (ArrayList<char>::Iterator iter(e.iterator());iter.hasNext();)
		{
		cout << iter.next();
		}
		cout <<endl;
	}*/

//	HashSet <int,Hashint> H;

//
	HashMap <int, double ,Hashint> H;
	cout <<"1";
	H.put(3,0.5);
	cout <<"2";
	for (HashMap<int, double,Hashint>::Iterator i(H.iterator()); i.hasNext(); )
	{
		Entry<int, double> tmp(i.next());
		cout <<tmp.key<<" "<<tmp.value<<endl;
	}
	while (1);
    return 0;
}
