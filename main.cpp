#include <iostream>
#include <map>
#include <vector>
using namespace std;
int CountOfValue(const vector<int>&individual){
    int count=0;
    for(auto item:individual){
        if (item==1) count++;
    }
    return count;
}

void SelectionPart2(map<int,int> &bestCopies,map<int,vector<int>> &Generation,const int &quontity,int &count2
        ,int &count3){
    int count=0;
    for(const auto & item:Generation){
        if(CountOfValue(item.second)==bestCopies[count3]){
            count++;
        }
    }
    if(count!=0){
        for(const auto & item:Generation){
            if (CountOfValue(item.second) == bestCopies[count3]) {
                if(count2<quontity/2) {
                    Generation[item.first].clear();
                    count2++;
                }
            }
        }
    }
    if(count2!=(quontity/2)){
        count3++;
        SelectionPart2(bestCopies,Generation,quontity,count2,count3);
    }
}
void SelectionPart(const int &count,const int &numberOfTheBestCopies,map<int,int> &bestCopies){
    if(count>bestCopies[numberOfTheBestCopies]){
        for(int i=1;i<numberOfTheBestCopies;i++){
            bestCopies[i]=bestCopies[i+1];
        }
        bestCopies[numberOfTheBestCopies]=count;
    }else if(numberOfTheBestCopies==1){

    }else{
        SelectionPart(count,numberOfTheBestCopies-1,bestCopies);
    }
}
unsigned int PRNG()
{
    static unsigned int seed = 4541;
    seed = (8253729 * seed + 2396403);
    return seed % 32768;
}
void MaximumValue(const map<int , vector<int>> & Generation ){
    int max=0;
    for(const auto & item:Generation){
        if(max<CountOfValue(item.second)){
            max=CountOfValue(item.second);
        }
    }
    cout<<"Maximum value of fitness-function is: "<<max<<endl;
}
void NewGeneration(map<int,vector<int>> & Generation,const int & quontity,int & countOfGenerations,int &countMax) {
    for(int i=1;i<=quontity;i++){
        if(Generation[i].empty()){
            for(int j=1;j<=30;j++){
                double now=PRNG();
                double op=(now/32768);
                if (op>=0.5){
                    Generation[i].push_back(1);
                }else{
                    Generation[i].push_back(0);
                }
            }
        }
    }
    countOfGenerations++;
    for(const auto & item:Generation){
        if (CountOfValue(item.second)>countMax){
            countMax=CountOfValue(item.second);
        }
    }
    cout<<"-----Generation "<<countOfGenerations<<" is created-----"<<endl;
    for(int i=1;i<=quontity;i++){
        for(int j=0;j<=29;j++){
            cout<<Generation[i][j];
        }
        cout<<endl;
    }
    MaximumValue(Generation);
}

void Selection(map<int,vector<int>> & Generation,const int & quontity,map<int,int>&bestCopies) {
    int count;
    int count2 = 0;
    int numberOfTheBestCopies= quontity%2==1? (quontity/2+1):quontity/2;
    for (int i = 1; i <= numberOfTheBestCopies; i++) {
        bestCopies[i] = 0;
    }
    for (const auto & item:Generation) {
        SelectionPart(CountOfValue(item.second), numberOfTheBestCopies, bestCopies);
    }
    for (auto &item:Generation) {
        count = CountOfValue(item.second);
        if (count < bestCopies[1]) {
            Generation[item.first].clear();
            count2++;
        }
    }
    if(count2!=quontity/2){
        int count3=1;
        SelectionPart2(bestCopies,Generation,quontity,count2,count3);
    }
}
void crossBreeding(map<int,vector<int>> & Generation,const int &quontity,map<int,int> & bestCopies) {
    vector<int> firstParent;
    vector<int> secondParent;
    int count = quontity % 2 == 1 ? quontity / 2 + 1 : quontity / 2;
    int max = bestCopies[count];
    int count2 = 0;
    while (count2 < quontity - count) {
        for (auto &item : Generation) {
            if (CountOfValue(item.second) == max) {
                firstParent = item.second;
            } else if (!item.second.empty()) {
                secondParent = item.second;
            }
            if (CountOfValue(firstParent)!=0 && CountOfValue(secondParent)!=0) {
                for (int j=1;j<=quontity;j++) {
                    if (Generation[j].empty()) {
                        double now = PRNG();
                        double op = (now / 32768);
                        int len = (int) (op * 30);
                        for (int i = 1; i <= len; i++) {
                            Generation[j].push_back(firstParent[i - 1]);
                        }
                        for (int i = len + 1; i <= 30; i++) {
                            Generation[j].push_back(secondParent[i - 1]);
                        }
                        count2++;
                        break;
                    }
                }
            }
        }
        count--;
        if(count<0){
            break;
        }else {
            max = bestCopies[count];
        }
    }
}
void Mutation(map<int,vector<int>> &Generation,const int  & quontity){
    for(int i=1;i<=quontity;i++){
        for(auto & item:Generation[i]){
            double now = PRNG();
            double op = (now / 32768);
            if(op>=0.9&& item==1){
                item=0;
            }else if(op>=0.8&&item==0){
                item=1;
            }
        }
    }
}

int main() {
    int quontity;
    int countmax = 0;
    cin >> quontity;
    int numberOfCycles;
    cin >> numberOfCycles;
    map<int, int> bestCopies;
    int countOfGeneration = 0;
    map<int, vector<int>> Generation;
    for (int i = 1; i <= quontity; i++) {
        Generation[i].clear();
    }
    for (int i = 1; i <= numberOfCycles; i++){
        NewGeneration(Generation, quontity, countOfGeneration, countmax);
        Selection(Generation, quontity, bestCopies);
        crossBreeding(Generation,quontity,bestCopies);
        Mutation(Generation,quontity);
    }
}
