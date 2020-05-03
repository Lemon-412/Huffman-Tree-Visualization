#include <iostream>
#include <string>
#include <queue>
#include <cassert>
using namespace std;

struct Huffman_Tree_Node{
    int value,pos[2],id;
    Huffman_Tree_Node *left_child,*right_child;
    Huffman_Tree_Node(int v=0,Huffman_Tree_Node *l=NULL,Huffman_Tree_Node *r=NULL):value(v),left_child(l),right_child(r){}
    inline void set_position(int x,int y) {pos[0]=x;pos[1]=y;}
    inline void set_id(int x) {id=x;}
};

class Huffman_Tree{
protected:
    int depth,id;
    Huffman_Tree_Node *root;

    void clear(Huffman_Tree_Node *p){
        if (p==NULL) return;
        clear(p->left_child);
        clear(p->right_child);
        delete p;p=NULL;
    }
    void set_position(Huffman_Tree_Node *p,int x,int y,int dx){
        if (p==NULL) return;
        p->set_position(x,y);
        set_position(p->left_child,x-dx/2,y+1,dx/2);
        set_position(p->right_child,x+dx/2,y+1,dx/2);
    }

public:
    ~Huffman_Tree(){clear();}
    Huffman_Tree(int value,int c){
        root=new Huffman_Tree_Node(value);
        root->set_id(c);
        depth=1;id=c;
    }
    Huffman_Tree(Huffman_Tree *l,Huffman_Tree *r,int c){
        assert(l!=NULL);assert(r!=NULL);
        root=new Huffman_Tree_Node(l->get_value()+r->get_value(),l->root,r->root);
        depth=max(l->depth,r->depth)+1;id=c;
    }
    inline int get_depth() const{return depth;}
    inline int get_value() const{
        assert(root!=NULL);
        return root->value;
    }
    inline Huffman_Tree_Node* get_root() const{
        assert(root!=NULL);
        return root;
    }
    void clear(){
        clear(root);
        root=NULL;
        depth=-1;
    }
    void set_position(int base){
        assert(root!=NULL);
        set_position(root,base+(1<<(depth-1)),1,1<<(depth-1));
    }
    bool operator<(const Huffman_Tree& qmh) const{
        if (get_value()==qmh.get_value()) return id<qmh.id;
        return get_value()>qmh.get_value();
    }
};

struct cmp{
    bool operator()(const Huffman_Tree* a,const Huffman_Tree* b) const{
        return (*a)<(*b);
    }
};

typedef priority_queue<Huffman_Tree*,vector<Huffman_Tree*>,cmp> Huffman_Tree_Priority_Queue;



/**********************************************************************************************************/


const bool logging=true;
const bool output=false;
Huffman_Tree_Priority_Queue que;

void show_position(Huffman_Tree_Node *cur,Huffman_Tree_Node *parent=NULL){
//    cout<<"Node: ["<<cur->pos[0]<<","<<cur->pos[1]<<"] value="<<cur->value<<endl;
    cout<<"1 "<<cur->pos[0]*20<<' '<<cur->pos[1]*75<<' '<<cur->value<<endl;
    if (parent!=NULL){
//        cout<<"Line: ["<<parent->pos[0]<<","<<parent->pos[1]<<"]<->["<<cur->pos[0]<<","<<cur->pos[1]<<"]"<<endl;
        cout<<"2 "<<parent->pos[0]*20<<' '<<parent->pos[1]*75<<' '<<cur->pos[0]*20<<' '<<cur->pos[1]*75<<endl;
    }
    if (cur->left_child!=NULL){
        show_position(cur->left_child,cur);
        show_position(cur->right_child,cur);
    }
}

void display(){
    if (!logging) return;
    Huffman_Tree_Priority_Queue q=que;
    int x=1;
//    cout<<"round"<<endl;
    cout<<"-1"<<endl;
    while (!q.empty()){
        Huffman_Tree *p=q.top();q.pop();
        p->set_position(x);
        show_position(p->get_root());
        x+=(1<<p->get_depth())+1;
    }
}

Huffman_Tree* merge(int &cnt){
    Huffman_Tree *l,*r,*p;
    while (true){
        display();
        l=que.top();que.pop();
        if (que.empty()) return l;
        r=que.top();que.pop();
        p=new Huffman_Tree(l,r,cnt++);
        que.push(p);
    }
}

int num[100];
string ans[100];

void dfs(Huffman_Tree_Node* p,string& cur){
    if (p->left_child==NULL){
        assert(p->right_child==NULL);
        ans[p->id]=cur;
    }
    else{
        assert(p->right_child!=NULL);
        cur.push_back('1');
        dfs(p->left_child,cur);
        cur.erase(--cur.end());
        cur.push_back('0');
        dfs(p->right_child,cur);
        cur.erase(--cur.end());
    }
}

void get_ans(Huffman_Tree* t,int kase,int n){
    if (!output) return;
    cout<<"Case "<<kase<<endl;
    string cur="";
    dfs(t->get_root(),cur);
    for (int i=0;i<n;++i){
        cout<<num[i]<<' '<<ans[i]<<endl;
    }
    cout<<endl;
}

int main(){
    freopen("data.in","r",stdin);
    freopen("data.out","w",stdout);
    int T,n,kase=0;
    cin>>T;
    while (T--){
        int cnt=0;
        cin>>n;
        for (int i=0;i<n;++i){
            cin>>num[i];
            Huffman_Tree* p=new Huffman_Tree(num[i],cnt++);
            que.push(p);
        }
        Huffman_Tree* result=merge(cnt);
        get_ans(result,++kase,n);
        result->clear();
    }
    cout<<"-2"<<endl;
    return 0;
}
