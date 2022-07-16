// Inês Martins Marçal 2019215917
// Noémia Gonçalves 2019219433

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

struct node
{
    
    int value;
    
    //[number of nodes, sum of values]
    array<array<int, 2>, 2> achievedValues;
    vector<int> children;
};

unordered_map<int, node> nodes;

//Best criteria in order:
//1)Minimize number of nodes
//2)Maximize sum of valuea
array<int, 2> getBest(int nodeId)
{
    
    array<int, 2> use = nodes[nodeId].achievedValues[1];
    
    array<int, 2> dontUse = nodes[nodeId].achievedValues[0];
   
    if (use[0] > dontUse[0])
    {
        return dontUse;
    }
   

    if (use[0] < dontUse[0])
    {
        return use;
    }
    
    if (use[0] == dontUse[0])
    {
        if (use[1] > dontUse[1])
        {
            return use;
        }
        
        if (use[1] < dontUse[1])
        {
            return dontUse;
        }
    }
    return use;
}

// Top Down Recursive Approach
void pyramidschemev2(int id)
{

    array<int, 2> doesntUse = {0,0}, use = {0,0}, best;
    int child;

    for (int i = 0; i < int(nodes[id].children.size()); i++)
    {
        child = nodes[id].children[i];
        pyramidschemev2(child);
    }

    if (int(nodes[id].children.size()) == 0)
    {
      
        nodes[id].achievedValues = {doesntUse, {1, nodes[id].value}};
        
    }else{
        
        for (int i = 0; i < int(nodes[id].children.size()); i++)
        {
            child = nodes[id].children[i];
        
        //If it doesn't use the node it must use all its children
        doesntUse[0] += nodes[child].achievedValues[1][0];
        doesntUse[1] += nodes[child].achievedValues[1][1];

        //If the node is used choose bestResult from each children
        best = getBest(child);
        
        use[0] += best[0];
        use[1] += best[1];
    }
    
    use[0] += 1;
    use[1] += nodes[id].value;
   
    nodes[id].achievedValues = {doesntUse, use};
    }

}

int main()
{
    string line;
    int parent = -1;
    int child;

    nodes.clear();
 
    while (cin >> parent)
    {
       
        getline(cin, line);
        stringstream ss(line);

        // End Test Case
        if (parent == -1)
        {
            
            pyramidschemev2(0);

            array<int, 2> bestResult = getBest(0);
            cout << bestResult[0] << " " << bestResult[1] << "\n";

            nodes.clear();

            continue;
        }
        
        nodes.insert({parent, node()});

        while (ss >> child)
        {
        
            nodes[parent].children.push_back(child);
        }
        
        nodes[parent].value = nodes[parent].children.back();
       
        nodes[parent].children.pop_back();
    }

    return 0;
}
