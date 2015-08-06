#include <iostream> //to cout/cin
#include <stack> //implement stack
#include <fstream> //to read&write
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <limits>
using namespace std;

int debug=0;
float calculate(float a, float b, char c){
  float result;
  switch(c){
    case '+':
      result= a+b;
      if(debug>1)
      cout<<"did +++++++++++"<<result<<endl; 
      break;
    case '-':
      result= a-b;
      if(debug>1)
      cout<<"did ------------  "<<result<<endl;
      break;
    case '*':
      result= a*b;
      if(debug>1)
      cout<<"did *********"<<result<<endl;
      break;
    case '/':
      result= a/b;
      if(debug>1)
      cout<<"did //////////"<<a<<" / "<<b<<" = "<< result<<endl;
      break;
    default:
      cout<<"ERROR OPERATION"<<result<<endl;
      break;
  }
  return result;
}
int do_caculate(stack<float> &num_stack, stack<int> &op_stack){
  if(num_stack.empty())
    return 1;
  float tmp=num_stack.top();
  num_stack.pop();
  float tmp2=num_stack.top();
  num_stack.pop();
  num_stack.push(calculate(tmp2,tmp,op_stack.top()));
  op_stack.pop();
  return 0;
}
int do_caculate_final(stack<float> &num_stack, stack<int> &op_stack){
  if(num_stack.empty()||op_stack.empty())
    return 1;
  float tmp=num_stack.top();
  num_stack.pop();
  float tmp2=num_stack.top();
  num_stack.pop();
  num_stack.push(calculate(tmp,tmp2,op_stack.top()));
  op_stack.pop();
  return 0;
}

//when hit ")"
int reverse_calculate(stack<float> &num_stack, stack<int> &op_stack, 
  stack<float> &tmp_num_stack, stack<int> &tmp_op_stack){

  while( !op_stack.empty() && op_stack.top()!=40 ){

        tmp_op_stack.push(op_stack.top());
        op_stack.pop();

        tmp_num_stack.push(num_stack.top());
        num_stack.pop();
      }

if(op_stack.empty()){
    return 1;
  }
      op_stack.pop();

      tmp_num_stack.push(num_stack.top());
      num_stack.pop();
       
      while(!tmp_op_stack.empty() && tmp_num_stack.size()>=2){ 
        do_caculate_final(tmp_num_stack,tmp_op_stack);
      }

      num_stack.push(tmp_num_stack.top());
      tmp_num_stack.pop();
      return 0;
}
void emptystack(stack<float> &num_stack, stack<int> &op_stack){
   while(!num_stack.empty()){
        num_stack.pop();
      }
       while(!op_stack.empty()){
        op_stack.pop();
      }
}

void printstack(stack<float> &num_stack, stack<int> &op_stack){
  stack<float> tmp;
  stack<int> tmp2;
  while(!num_stack.empty()){
        tmp.push(num_stack.top());
        num_stack.pop();
      }
       while(!op_stack.empty()){
        tmp2.push(op_stack.top());
        op_stack.pop();
      }

      cout<<"Number Stack:"<<endl;
      while(!tmp.empty()){
        cout<<tmp.top()<<" ";
        num_stack.push(tmp.top());
        tmp.pop();
      }
       cout<<endl;
       cout<<"Operation Stack:"<<endl;
       while(!tmp2.empty()){
        cout<<(char)tmp2.top()<<" ";
        op_stack.push(tmp2.top());
        tmp2.pop();
      }
       cout<<endl;
}
int main (int argc, char* argv[])
{
 
 if(argv[1]==NULL){
  cout<<"Usage: ./a.out xxx_input.txt\nOutput file: output.txt"<<endl;
  exit(1);
 }
  if(argv[2]!=NULL && strcmp(argv[2],"d")==0)
    debug=1;
 else if(argv[2]!=NULL && strcmp(argv[2],"dd")==0)
    debug=2;


  //read from file
  ifstream input(argv[1]);
  string str;

  //write to output file
  ofstream output;
  output.open("output.txt");
  
  stack<int> op_stack;
  stack<float> num_stack;

  stack<float> tmp_num_stack;
  stack<int> tmp_op_stack;

  int er_flag=0;

  while (getline(input,str)){
    if(str.empty())
      continue;
    er_flag=0;
    emptystack(num_stack,op_stack);

    cout<<"#case: "<<str<<endl;
    int n=str.size();
    int num_flag=1; // 1 when it's not a number
    int op_flag=0; //  level of operation
    int check_op=1; // not an */+-
    int n_pr=0; //to match # of "(" and # of ")"

    int j; //curser: set location of begin of a num

    int first_flag=0;

    for(int i=0;i<n;i++){
      // cout<<"this is loop"<<i<<endl;
      if(str[i]==32)
        continue;

      if(debug>0)
      printstack(num_stack,op_stack);
      // if(first_flag==0){
      //   if(!(str[0]<=57 && str[0]>=48) && str[0]!=40){
      //    er_flag=1;
      //     cout<<"EEEERRROOOORRR"<<endl;
      //     output<<"ERROR"<<endl;
      //     break;
      //   }
      //   first_flag=1;
      // }
       
      if (str[i]<=57 && str[i]>=48){ //if is a number
        if(num_flag==1){
          num_flag=0; // 0 when it's a number
          j=i;
        }
      } else{
        
        if(num_flag==0){
           num_stack.push(stof(str.substr(j,i-j)));
           num_flag=1;
           check_op=1;
        } 


        if(op_flag==3|| op_flag==4){
          op_flag=0;
        }   

         if(str[i]==40){ // "("
            op_flag=4;
            n_pr++;
            op_stack.push(str[i]);
            continue;
          }
          if(str[i]==41){ // ")"
            if(check_op==0){
              er_flag=1;
              break;
            }
            op_flag=3;
            n_pr--;
            //calculate result in paraphisis...

            reverse_calculate(num_stack,op_stack,
              tmp_num_stack,tmp_op_stack);

             if(debug>0)
            printstack(num_stack,op_stack);
            // cout<<"this is er_flag"<<er_flag<<endl;
    
            continue;
          }

           //only to check two */+- cannot be together
        if(str[i]==47||str[i]==42||str[i]==43||str[i]==45){
          if(op_flag==4){
            er_flag=1;
            break;
          }
          if(check_op==0){
            er_flag=1;
            break; //if operation sign messed up
          }
          check_op=0;
        }else check_op=1;

        if(op_flag==2){
            er_flag = do_caculate(num_stack,op_stack);
            op_flag=0;
         }

         if(str[i]==47||str[i]==42){ // * or /
             op_stack.push(str[i]);
            op_flag=2;
            continue;
          }

         if(str[i]==43||str[i]==45){ // + or -
          op_flag=1;
          op_stack.push(str[i]);
          continue;
        }
        
          er_flag=1;      
          break;
      }// end of else
         
    }
      if(n_pr!=0){
        er_flag=1;
      }

      if(er_flag){
        cout<<"EROORRR"<<endl;
        output<<"ERROR"<<endl;
        continue;

      }
       
     float next=stof(str.substr(j,n-j));
     num_stack.push(next);


      if(op_flag==2){
            er_flag = do_caculate(num_stack,op_stack);
       }

      stack<float> tmp_num_stack;
      stack<int> tmp_op_stack;

      while(!num_stack.empty()){
        tmp_num_stack.push(num_stack.top());
        num_stack.pop();
      }

       while(!op_stack.empty()){
        tmp_op_stack.push(op_stack.top());
        op_stack.pop();
      }
      
      while(!tmp_op_stack.empty() && tmp_num_stack.size()>=2){ 
        er_flag = do_caculate_final(tmp_num_stack,tmp_op_stack);
      }

      // end of operation
      if(er_flag){
        cout<<"EROORRR"<<endl;
         output<<"ERROR"<<endl;
        continue;
      }

      if(debug>0)
      printstack(num_stack,op_stack);
    
      if(debug>1)
       cout<<"There're "<<num_stack.size()<<" numbers left on stack"<<endl;
       cout<<"Result is equals to "<<tmp_num_stack.top()<<endl;  
    
      float result=tmp_num_stack.top();

      if(tmp_op_stack.size()==0){
        if(debug>1)
        cout<<"All operations are done"<<endl;
      }

      output<<setprecision(10)<<(float)result<<endl;
  }

  input.close();
  output.close();

  return 0;
}