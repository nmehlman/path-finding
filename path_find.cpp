#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

struct Map{

    int x_dim, y_dim;
    pair<int,int> start, end;
    vector<vector<int>> map;

    Map(){ //Default constructor
        x_dim = -1;
        y_dim = -1;
        map = vector<vector<int>>();
    } 

    Map(ifstream &in){ //Constructor from file
        string line_txt;
        while(getline(in,line_txt)){
            map.push_back(vector<int>());
            istringstream line(line_txt);
            char c;
            while(line>>c){
                if(c=='.')
                    map[map.size()-1].push_back(-1);
                else if(c=='x')
                    map[map.size()-1].push_back(-2);
                else if(c=='s'){
                    map[map.size()-1].push_back(0);
                    start = make_pair<int,int>(map[map.size()-1].size()-1,map.size()-1);
                }
                else if(c=='e'){
                    map[map.size()-1].push_back(-3);
                    end = make_pair<int,int>(map[map.size()-1].size()-1,map.size()-1);
                }
            }
        }

        x_dim = map[0].size();
        y_dim = map.size();

    }

    void Print(){
      for(auto i:map){
        for(auto j:i){
            cout<<setw(2)<<j<<" ";
        }
        cout<<endl;  
      }
    }

};

struct Square{
    Map &map;
    int x,y,&value;
    bool visited = false;
    Square(Map &m):x(-1), y(-1),map(m),value(x){}
    Square(Map &m,int x_pos, int y_pos):x(x_pos),y(y_pos),map(m),value(map.map[y][x]){
        if(x>=map.x_dim || y>=map.y_dim || x<0 || y<0)
            throw runtime_error("location exceeds map bounds");
    }
    Square(Map &m,pair<int,int> p):x(p.first),y(p.second),map(m),value(map.map[y][x]){
        if(x>=map.x_dim || y>=map.y_dim || x<0 || y<0)
            throw runtime_error("location exceeds map bounds");
    }
    Square left(){
        if(x>0)
            return Square(map,x-1,y);
        else
            return *this;
    }
    Square right(){
        if(x<map.x_dim-1)
            return Square(map,x+1,y);
        else
            return *this;
    }
    Square bottom(){
        if(y<map.y_dim-1)
            return Square(map,x,y+1);
        else
            return *this;;
    }
    Square top(){
        if(y>0)
            return Square(map,x,y-1);
        else
            return *this;;
    }
    void Print(){
        cout<<"("<<x<<","<<y<<"): "<<value<<endl;
    }
    bool operator !=(Square &s)
        {
            return !((s.x == x)&&(s.y==y)&& (&s.map == &map));
        }
    
    bool operator == (Square &s)
        {
            return ((s.x == x)&&(s.y==y)&& (&s.map == &map));
        }

};

void Update(Square &s){
    
    int current_dist = s.value;

    if(s.visited == false && current_dist>=0){
        
        s.visited = true;
        
        //Update top
        Square top = s.top();
        if(top!=s && top.value==-1){
            top.value = current_dist+1;
        }

        //Update right
        Square right = s.right();
        if(right!=s && right.value==-1){
            right.value = current_dist+1;
        }

        //Update bottom
        Square bottom = s.bottom();
        if(bottom!=s && bottom.value == -1){
            bottom.value = current_dist+1;
        }

        //Update left
        Square left = s.left();
        if(left!=s && left.value == -1){
            left.value = current_dist+1;
        }

    }    
}

Square Propagate(Square &start, Square &end){
    if(start == end)
        return start;
    else{
        start.map.Print();
        cout<<endl;
        Update(start);
        Square next_top = start.top(), next_bottom = start.bottom();
        Square next_left = start.left(), next_right = start.right();
        return Propagate(next_top,end);
        return Propagate(next_right,end);
        return Propagate(next_bottom,end); 
        return Propagate(next_left,end);   
    }
}

int main(){
    ifstream infile("map.txt");
    Map map1(infile);
    Square start(map1,map1.start);
    Square end(map1,map1.end);
    Propagate(start,end);
    map1.Print();
    return 0;
}
