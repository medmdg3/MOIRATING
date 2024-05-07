#include<bits/stdc++.h>
#include<cstring>
using namespace std;
#define pb(a) push_back(a)
#define mk(a,b) make_pair(a,b)
#define F first
#define S second
#define V vector
#define sz() size()
#define _map unordered_map
typedef long long ll;
typedef long double ld;
const string CF_Handles="codeforces.txt",AT_Handles="atcoder.txt",Names_Path="Names.txt";
const string Ranks_Path="Ranks/Ranking_%.txt",Contests_Path="Contests/Contest_%.txt",Reports_Path="Reports/Contest_%_Report.txt",Global_Rank_Path="Ranking/Contest_%_Ranks.txt";
string str_int(ll n){
    string ans="";
    while(n){
        ans+=n%10+'0';
        n/=10;
    }
    for(int i=0;2*i<ans.size();i++)swap(ans[i],ans[ans.size()-i-1]);
    return ans;
}
string Fix_text(string s,int n){
    string ans="",temp=str_int(n);
    for(int i=0;i<s.size();i++){
        if(s[i]=='%'){
            for(auto j:temp)ans+=j;
        }else ans+=s[i];
    }
    return ans;
}
vector<string> split(string a){
    string tem="";
    vector<string> ans;
    for(int i=0;i<=a.size();i++){
        if(i==a.size()||a[i]==' '||a[i]=='\t'||a[i]=='\n'){
            if(tem!="")ans.pb(tem);
            tem="";
        }else tem+=a[i];
    }
    return ans;
}
int int_str(string s){
    int ans=0;
    int i=0;
    if(s[0]=='-'||s[0]=='+')i++;
    int coef=1;
    if(s[0]=='-')coef=-1;
    for(;i<s.size();i++){
        ans*=10;
        ans+=s[i]-'0';
    }
    return ans*coef;
}
ld ld_str(string s){
    ld ans=0;
    int i=0;
    if(s[0]=='-'||s[0]=='+')i++;
    int coef=1;
    ld t=1;
    if(s[0]=='-')coef=-1;
    for(;i<s.size();i++){
        if(s[i]=='.'){t/=10;
        continue;}
        if(t==1)
        ans*=10;
        ans+=t*(s[i]-'0');
        if(t<1)t/=10;
    }
    return ans*coef;
}
string str_ld(ld a,int nb=10){
    string s=str_int(ll(a));
    a-=ll(a);
    if(a&&nb>0)s+='.';
    for(int i=0;a && i<nb;i++){
        a*=10;
        s+='0'+int(a);
        a-=int(a);
    }
    return s;
}
class Ranking_File{
public:
    int date;
    vector<vector<int>> Ranking;
    float weight;
    bool is_Primary;
    bool read(int n,bool All=false){
        string F_Rank=Fix_text(Ranks_Path,n);
        ifstream Ran(F_Rank.c_str());
        if(!Ran){
            cout<<"FILE DOES NOT EXIST!"<<endl;
            return false;
        }
        string line;
        getline(Ran,line);
        vector<string> element=split(line);
        unordered_map<string,int> Users;
        string Handles=element.back();
        Handles+=".txt";
        ifstream Hand(Handles.c_str());
        Ranking.clear();
        while(getline(Hand,line)){
            vector<string> temp=split(line);
            for(int i=1;i<temp.size();i++){
                Users[temp[i]]=int_str(temp[0]);
            }
        }
        Hand.close();
        if(element[0]=="PRIMARY"){
            date=int_str(element[2]);
            weight=ld_str(element[1]);
            is_Primary=true;
        }else{
            date=int_str(element[0]);
            is_Primary=false;
            weight=1;
        }
        vector<string> User_Not_Found;
        vector<bool> vis(Users.size()+1,false);
        while(getline(Ran,line)){
            element=split(line);
            Ranking.resize(Ranking.size()+1);
            for(auto i:element){
                if(Users[i]==0){
                    User_Not_Found.pb(i);
                    Users.erase(i);
                }else{
                    Ranking.back().pb(Users[i]);
                    vis[Users[i]]=true;
                }
            }
            if(Ranking.back().size()==0)Ranking.pop_back();
        }
        Ran.close();
        if(is_Primary){
            Ranking.resize(Ranking.size()+1);
            for(int i=1;i<vis.size();i++)if(!vis[i])Ranking.back().pb(i);
            if(!Ranking.back().size())Ranking.pop_back();
        }
        cout<<"Scanning complete!"<<endl;
        if(User_Not_Found.size()){
            printf("%d users were not found!\n",User_Not_Found.size());
            for(auto i:User_Not_Found){
                cout<<i<<endl;
            }
        }
        return true;
    }
    bool show(){
        return true;
        cout<<date<<" "<<weight<<" "<<is_Primary<<endl;
        for(auto i:Ranking){
            for(auto j:i)cout<<j<<" ";
            cout<<endl;
        }
        cout<<"#Done"<<endl;
    }

};
class Rating_File{
public:
    int date;
    vector<ld> Ratings;
    vector<ld> Credibility;
    vector<ld> Cte_Cred;
    vector<ll> npro;
    ld Calc_Performance(vector<ld> Rating,ld ran){
        ran-=0.5;
        ld l=Rating[0],r=Rating[0];
        for(auto i:Rating){
            l=min(l,i);
            r=max(r,i);
        }
        l-=100000;
        r+=100000;
        ld m=(l+r)/2;
        for(int j=0;j<500;j++){
            ld er=0;
            for(auto i:Rating){
                er+=ld(1)/(1+pow(10,(m-i)/400));
            }
            if(er>ran)l=m;
            else r=m;
            m=(l+r)/2;
        }
        return m;
    }
    vector<ld> Calc_Performances(vector<ld> Rating,vector<vector<int>> Ranks){
        int num=0;
        for(auto i:Ranks){
            num+=i.size();
        }
        vector<ld> X(num);
        vector<ld> RT;
        for(auto i:Ranks)for(auto j:i)RT.pb(Rating[j]);
        for(int i=0;i<num;i++){
            X[i]=Calc_Performance(RT,i+1);
        }
        cout<<endl;
        num=0;
        vector<ld> ans;
        for(auto i:Ranks){
            ld su=0;
            for(auto j:i){
                su+=X[num++];
            }
            su/=i.size();
            ans.pb(su);
        }
        cout<<endl;
        return ans;
    }
    void init(int n,ld iR=1500){
        date=0;
        vector<ld> nr(n,iR),nc(n,0),ncte(n,0);
        vector<ll> nnp(n,0);
        npro=nnp;
        Ratings=nr;
        Credibility=nc;
        Cte_Cred=ncte;
    }
    _map<int,string> Get_Name(string path=Names_Path){
        ifstream Fi(path.c_str());
        string line;
        _map<int,string> ans;
        while(getline(Fi,line)){
            vector<string> element=split(line);
            if(!element.size())continue;
            string s="";
            for(int i=1;i<element.size();i++)s+=element[i]+" ";
            ans[int_str(element[0])]=s;
        }
        return ans;
    }
    bool Apply_Ranking(Ranking_File A,int gen_Repport=0){
        if(A.date<date){
            cout<<"Conflict of dates!"<<endl;
            return false;
        }
        ld Delta=log2(2)/120;
        int dt=A.date-date;
        Delta=pow(2,-Delta*dt);
        date=A.date;
        for(int i=0;i<Credibility.size();i++){
            Credibility[i]=(Credibility[i]-Cte_Cred[i])*Delta+Cte_Cred[i];
        }
        vector<ld> t=Calc_Performances(Ratings,A.Ranking);
        if(gen_Repport){
            string path=Fix_text(Reports_Path,gen_Repport);
            FILE* Fi=fopen(path.c_str(),"w");
            _map<int,string> Name=Get_Name();
            int ran=1,ind=0;
            fprintf(Fi,"%d\n",date);
            for(auto i:A.Ranking){
                for(auto j:i){
                    ld nR=(Ratings[j]*Credibility[j]+A.weight*t[ind])/(Credibility[j]+A.weight);
                    ld nCre=Credibility[j]+A.weight;
                    fprintf(Fi,"%d\t%s\t%.0Lf\t%.2Lf\t%.0Lf\t%.0Lf\t%.0Lf\n",ran,Name[j].c_str(),Ratings[j],nCre,t[ind],nR,nR-Ratings[j]);
                }
                ind++;
                ran+=i.size();
            }
            fclose(Fi);
        }
        for(int i=0;i<t.size();i++){
            for(auto j:A.Ranking[i]){
                Ratings[j]=(Ratings[j]*Credibility[j]+A.weight*t[i])/(Credibility[j]+A.weight);
                Credibility[j]+=A.weight;
                if(A.is_Primary){Cte_Cred[j]+=A.weight;npro[j]++;}
            }
        }

        return true;
    }
    bool Apply_Ranking(int round_number,int gen_Repport=0){
        Ranking_File A;
        if(A.read(round_number)){
            A.show();
            return Apply_Ranking(A,gen_Repport);
        }
        return false;
    }
    bool Read(ifstream &File){
        if(!File)return false;
        string line;
        getline(File,line);
        vector<string> element;
        element=split(line);
        init(0);
        date=int_str(element[0]);
        while(getline(File,line)){
            element=split(line);
            int i=int_str(element[0]);
            while(i>=Ratings.size()){
                Ratings.pb(1500);
                Credibility.pb(0);
                Cte_Cred.pb(0);
                npro.pb(0);
            }
            Ratings[i]=(ld_str(element[1]));
            Credibility[i]=(ld_str(element[2]));
            Cte_Cred[i]=(ld_str(element[3]));
            npro[i]=(int_str(element[4]));
        }
        return true;
    }
    bool Read(string path){
        ifstream File(path.c_str());
        return Read(File);
    }
    bool Read(int ind){
        return Read(Fix_text(Contests_Path,ind));
    }
    ld Calc_Score(int i){
        ld Score=1-pow(0.5,Credibility[i]-Cte_Cred[i]+npro[i]);
        Score*=Ratings[i];
        return Score;
    }
    void Save(FILE* Fi){
        fprintf(Fi,"%d\n",date);
        for(int i=1;i<Ratings.size();i++){
            ld Score=Calc_Score(i);
            fprintf(Fi,"%d %.20Lf %.20Lf %.20Lf %d %.20Lf\n",i,Ratings[i],Credibility[i],Cte_Cred[i],npro[i],Score);
        }
    }
    void Save(string path){
        FILE *Fi=fopen(path.c_str(),"w");
        Save(Fi);
        fclose(Fi);
    }
    void Save(int ind){
        string path=Fix_text(Contests_Path,ind);
        Save(path);
    }
    bool genRepport(FILE* Fi){
        if(!Fi)return false;
        fprintf(Fi,"%d\n",date);
        vector<pair<ld,int>> Scores(Ratings.size()-1);
        for(int i=0;i<Scores.size();i++)Scores[i]=mk(Calc_Score(i+1),i+1);
        sort(Scores.rbegin(),Scores.rend());
        vector<ll> ran(Scores.size());
        for(int i=0;i<Scores.size();i++){
            ran[i]=i+1;
            if(i&&Scores[i].F==Scores[i-1].F)ran[i]=ran[i-1];
        }
        _map<int,string> Name=Get_Name();
        for(int i=0;i<Scores.size();i++){
            fprintf(Fi,"%d\t%s\t%.0Lf\t%.0Lf\t%.2Lf\n",ran[i],Name[Scores[i].S].c_str(),Scores[i].F,Ratings[Scores[i].S],Credibility[Scores[i].S]);
        }
        return true;
    }
    bool genRepport(string Path){
        FILE* Fi=fopen(Path.c_str(),"w");
        bool re= genRepport(Fi);
        fclose(Fi);
        return re;
    }
    bool genRepport(int n){
        string Path=Fix_text(Global_Rank_Path,n);
        return genRepport(Path);
    }
};
int find_Latest(string Text_to_Fix){
    for(int i=1;;i++){
        ifstream Fil(Fix_text(Text_to_Fix,i));
        if(!Fil)return i-1;
    }
}
bool Read_Ans(){
    string ans;
    cin>>ans;
    return (ans=="Y"||ans=="y");
}
bool Menu(){
    int a=find_Latest(Ranks_Path);
    int b=find_Latest(Contests_Path);
    Rating_File A;
    _map<int,string> Names;
    Names=A.Get_Name();
    if(b)
    A.Read(b);
    else{
        A.init(Names.size()+1,1500);
    }
    if(a>b){
        cout<<a-b<<" new contest have been detected, do you want to add them?(Y/N)"<<endl;
        if(Read_Ans()){
            while(b!=a){
                if(!A.Apply_Ranking(++b)){cout<<"An error happend while trying to add the file: \""<<Fix_text(Ranks_Path,b)<<"\", the program will end"<<endl;return false;}
                A.Save(b);
            }
        }
    }

    if(a<b){
        cout<<"Seems like some Ranks files are missing! do you want to proceed?(Y/N)"<<endl;
        if(!Read_Ans())return false;
    }
    cout<<"Do you want to regenerate all files?"<<endl;
    cout<<"(Y/N)"<<endl;
    if(Read_Ans()){
        Rating_File A;
        A.init(Names.size()+1,1500);
        A.date=-1;
        for(int i=1;i<=a;i++){
            if(A.Apply_Ranking(i,i))
            A.genRepport(i);
            else break;
        }
    }
    return false;
}
int main(){
    while(Menu());
}
