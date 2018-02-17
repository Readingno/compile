#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define norw  13        //13个保留字
#define txmax  100      //标识符表长度
#define al 100          //标识符长度上限

//定义符号及保留字对应的符号
typedef enum symbol
{
    IDENT,NUMBER,STRING,CHAR,          //标识符，数字，字符串
    PLUS,MINUS,TIMES,DEVIDE,BECOME,   //加，减，乘，除，赋值
    EQL,NEQ,LSS,LEQ,GTR,GEQ,           //==,!=,<,<=,>,>=
    LPAREN,RPAREN,LBRACK,RBRACK,LBRACE,
    RBRACE,COMMA,SEMICOLON,COLON,      //(，)，[，]，{，}，,，;，:
    MAINSYM,CONSTSYM,VARSYM,IFSYM,
    DOSYM,WHILESYM,SWITCHSYM,CASESYM,
    SCANSYM,PRINTSYM,RETURNSYM,        //各保留字的标识
    SPACE                              //空格
} symbol;
typedef enum idattr
{
    _INT,_CHAR,_VOID
} idattr;
typedef enum idtype
{
    _CONST,_VAR,_PROC,_PARA
} idtype;

    char *word[] = {"case","char","const","do","if","int","main","printf","return","scanf","switch","void","while"};
    symbol wsym[] = {CASESYM,VARSYM,CONSTSYM,DOSYM,IFSYM,VARSYM,MAINSYM,PRINTSYM,RETURNSYM,SCANSYM,SWITCHSYM,VARSYM,WHILESYM};
    FILE *fin;
    symbol sym;     //读取的上一个symbol
    char ch = ' ';      //读取的上一个字符
    char crtword[al];   //当前单词
    int num;            //当前数字
    int cc,ll;
    int lcount = 1;     //行计数器
    int eof = 0;        //文件读取结束判断
    char program[99999][100];    //代码字符存储
    symbol progsym[99999];     //代码类型存储
    int wdcount = 0;            //代码单词计数器
    int contract = 0;               //语法冲突标识

   // int level;          //嵌套层数
   // char *idt[];     //变量常量函数名表

void getch()
{
    ch = fgetc(fin);
    if (ch==EOF)
    {
        fclose(fin);
        eof = 1;
    }
    if (ch == '\n')
    {
        lcount ++;
    }
}
void getsym()
{
    int i,j;
    int k;        //单词字数计数器
    memset(crtword, 0, sizeof(crtword));
    num = 0;
    k = 0;
    while (ch==' ' || ch=='\t' || ch=='\n')
    {
        switch (ch){
            case ' ':{strcpy(program[wdcount]," ");break;}
            case '\t':{strcpy(program[wdcount],"\t");break;}
            case '\n':{strcpy(program[wdcount],"\n");break;}
        }
        progsym[wdcount] = SPACE;
        wdcount ++;
        getch();
    }
    if ((ch>='a' && ch<='z')||(ch>='A' && ch<='Z'))
    {
        do
        {
            if (k<al)
            {
                crtword[k] = ch;
                k ++;
            }
            getch();
        }
        while ((ch>='a' && ch<='z')||(ch>='0' && ch<='9')||(ch>='A' && ch<='Z'));
        strcpy(program[wdcount], crtword);
        for (i=0; i<k; i++)
            if (crtword[i]>='A' && crtword[i]<='Z')
                crtword[i] -= 32;
        for (i=0; i<norw; i++)
        {
            if (!strcmp(crtword, word[i]))
            {
                sym = wsym[i];
                break;
            }
        }
        if (i==norw) sym = IDENT;
    }
    else if (ch>='0' && ch<='9')
    {
        sym = NUMBER;
        do
        {
            num = 10*num+(ch-'0');
            crtword[k] = ch;
            k ++;
            getch();
        }
        while (ch>='0' && ch<='9');
        strcpy(program[wdcount], crtword);
    }
    else if (ch=='\"')
    {
        sym = STRING;
        do
        {
            if (k<al)
            {
                crtword[k] = ch;
                k ++;
            }
            getch();
        }
        while (ch!='\"');
        crtword[k] = ch;
        strcpy(program[wdcount], crtword);
        getch();
    }
    else if (ch=='\'')
    {
        sym = CHAR;
        do
        {
            if (k<al)
            {
                crtword[k] = ch;
                k ++;
            }
            getch();
        }
        while (ch!='\'');
        crtword[k] = ch;
        strcpy(program[wdcount], crtword);
        getch();
    }
    else if (ch=='+')
    {
        sym = PLUS;
        strcpy(program[wdcount],"+");
        getch();
    }
    else if (ch=='-')
    {
        sym = MINUS;
        strcpy(program[wdcount],"-");
        getch();
    }
    else if (ch=='*')
    {
        sym = TIMES;
        strcpy(program[wdcount],"*");
        getch();
    }
    else if (ch=='/')
    {
        sym = DEVIDE;
        strcpy(program[wdcount],"/");
        getch();
    }
    else if (ch=='(')
    {
        sym = LPAREN;
        strcpy(program[wdcount],"(");
        getch();
    }
    else if (ch==')')
    {
        sym = RPAREN;
        strcpy(program[wdcount],")");
        getch();
    }
    else if (ch=='[')
    {
        sym = LBRACK;
        strcpy(program[wdcount],"[");
        getch();
    }
    else if (ch==']')
    {
        sym = RBRACK;
        strcpy(program[wdcount],"]");
        getch();
    }
    else if (ch=='{')
    {
        sym = LBRACE;
        strcpy(program[wdcount],"{");
        getch();
    }
    else if (ch=='}')
    {
        sym = RBRACE;
        strcpy(program[wdcount],"}");
        getch();
    }
    else if (ch==',')
    {
        sym = COMMA;
        strcpy(program[wdcount],",");
        getch();
    }
    else if (ch==';')
    {
        sym = SEMICOLON;
        strcpy(program[wdcount],";");
        getch();
    }
    else if (ch==':')
    {
        sym = COLON;
        strcpy(program[wdcount],":");
        getch();
    }
    else if (ch=='=')
    {
        getch();
        if (ch=='=')
        {
            sym = EQL;
            strcpy(program[wdcount],"==");
            getch();
        }
        else
        {
            sym = BECOME;
            strcpy(program[wdcount],"=");
        }
    }
    else if (ch=='!')
    {
        getch();
        if (ch=='=')
        {
            sym = NEQ;
            strcpy(program[wdcount],"!=");
            getch();
        }
    }
    else if (ch=='<')
    {
        getch();
        if (ch=='=')
        {
            sym = LEQ;
            strcpy(program[wdcount],"<=");
            getch();
        }
        else
        {
            sym = LSS;
            strcpy(program[wdcount],"<");
        }
    }
    else if (ch=='>')
    {
        getch();
        if (ch=='=')
        {
            sym = GEQ;
            strcpy(program[wdcount],">=");
            getch();
        }
        else
        {
            sym = GTR;
            strcpy(program[wdcount],">");
        }
    }
    else
    {
        printf("error! #%d#\n",lcount);
        getch();
    }
    //printf("%d %s\n",wdcount, program[wdcount]);
    progsym[wdcount] = sym;
    wdcount ++;
}
char* symtochar(symbol sym)
{
    switch (sym)
    {
        case IDENT: return "IDENT";
        case NUMBER: return "NUMBER";
        case STRING: return "STRING";
        case CHAR: return "CHAR";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case TIMES: return "TIMWS";
        case DEVIDE: return "DEVIDE";
        case BECOME: return "BECOME";
        case EQL: return "EQL";
        case NEQ: return "NEQ";
        case LSS: return "LSS";
        case LEQ: return "LEQ";
        case GTR: return "GTR";
        case GEQ: return "GEQ";
        case LPAREN: return "LPAREN";
        case RPAREN: return "RPAREN";
        case LBRACK: return "LBRACK";
        case RBRACK: return "RBRACK";
        case LBRACE: return "LBRACE";
        case RBRACE: return "RBRACE";
        case COMMA: return "COMMA";
        case SEMICOLON: return "SEMICOLON";
        case COLON: return "COLON";
        case MAINSYM: return "MAINSYM";
        case CONSTSYM: return "CONSTSYM";
        case VARSYM: return "VARSYM";
        case IFSYM: return "IFSYM";
        case DOSYM: return "DOSYM";
        case WHILESYM: return "WHILESYM";
        case SWITCHSYM: return "SWITCHSYM";
        case CASESYM: return "CASESYM";
        case SCANSYM: return "SCANSYM";
        case PRINTSYM: return "PRINTSYM";
        case RETURNSYM: return "RETRNSYM";
        case SPACE: return "SPACE";
    }
}

int _program();            //程序
int _const_dec();          //常量说明
int _var_dec();            //变量说明
int _return_fun_def();     //有返回值函数定义
int _void_fun_def();       //无返回值函数定义
int _main_fun_def();       //主函数
int _const_def();          //常量定义
int _var_def();            //变量定义
int _type_ident();         //类型标识符
int _dec_head();           //声明头部
int _para_list();          //参数表
int _comp_state();         //复合语句
int _state_list();         //语句列
int _state();              //语句
int _if_state();           //条件语句
int _condition();          //条件
int _while_state();        //循环语句
int _switch_state();       //情况语句
int _case_list();          //情况表
int _case_sub_state();     //情况子语句
int _return_fun_call();    //有返回值函数调用
int _void_fun_call();      //无返回值函数调用
int _value_para_list();    //值参数表
int _assign_state();       //赋值语句
int _read_state();         //读语句
int _write_state();        //写语句
int _return_state();       //返回语句
int _express();            //表达式
int _term();               //项
int _factor();             //因子
int _const();              //常量
int _ident();              //标识符
int _int();                //整数
int _unsigned_int();       //无符号整数
int _num();                //数字
int nonzero_num();         //非零数字
int _char();               //字符
int _string();             //字符串
int _letter();             //字母
int _relat_op();           //关系运算符
int _add_op();             //加法运算符
int _mult_op();            //乘法运算符

int travel = -1;                 //遍历标识

struct idtable
{
    char name[100];
    idtype idt;
    idattr ida;
    int array;
    int block;
    int addr;
} idtable[];                  //符号表
int crtblock = 0;               //现语句块
int blocknum = 0;               //语句块数量
int addr = 0;
int idtindex = 0;               //符号表索引


int labelct = 0;                //标签计数
int interindex = 0;             //四元式表索引

typedef enum interop
{
    _PLUS,_MINUS,_TIMES,_DIVADE,_BECOME,
    _EQLJ,_NEQJ,_LSSJ,_LEQJ,_GTRJ,_GEQJ,_JUMP,
    _STORE,_LOAD,_LABEL,_PRINT,_SCAN
} interop;
struct inter
{
    interop op;
    char x[100];
    char y[100];
    char result[100];
} inter[];

typedef enum nodetype
{
    _optype, _datatype
} nodetype;
typedef enum optype
{
    _plus, _minus, _times, _divide, _array, _funccall
}
typedef enum datatype
{
    _int, _char, _ident
}
typedef struct exnode
{
    nodetype ntype;
    optype otype;
    datatype dtype;
    int con;
    char idname[100];
    int childnum;
    exnode father;
    exnode child[];
} exnode;

exnode operstack[100];
struct symstack
{
    optype otype;
    char funcname[100];
} symstack[100];
int optop=0, symtop=0;

void next_word()
{
    travel ++;
    while(progsym[travel] == SPACE)
    {
        printf("%s", program[travel]);
        travel ++;
    }
    if (travel == wdcount+1) exit(0);
}

int last_word()
{
    int temp = travel-1;
    while(progsym[temp] == SPACE)
    {
        temp --;
    }
    return temp;
}

int _program()              //＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
{
    next_word();
    _const_dec();
    _var_dec();
    while(1)
    {
        if (!_return_fun_def() && !_void_fun_def()) break;
    }
    _main_fun_def();
    printf("    #This is a program!#\n");
    return 1;
}

int _const_dec()            //const＜常量定义＞;{ const＜常量定义＞;}
{
    if (progsym[travel] == CONSTSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    _const_def();
    if (progsym[travel] == SEMICOLON)
    {
        printf("%s", program[travel]);
        next_word();
    }
    while (1)
    {
        if (progsym[travel] == CONSTSYM)
        {
            printf("%s", program[travel]);
            next_word();
        }
        else break;
        _const_def();
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    printf("    #This is a constant declaration!#\n");
    return 1;
}

int _var_dec()              //＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
{
    if (!_var_def()) return 0;
    if (progsym[travel] == SEMICOLON)
    {
        printf("%s", program[travel]);
        next_word();
    }
    while(1)
    {
        if (!_var_def()) break;
        if (progsym[travel] == LPAREN)
            return 0;
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    printf("    #This is a variable declaration!#\n");
    return 1;
}

int _return_fun_def()       //＜有返回值函数定义＞  ::=  ＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’
{
    if(!_dec_head() && progsym[travel] != LPAREN) return 0;
    //
    strcpy(idtable[idtindex].name, program[last_word()]);
    idtable[idtindex].idt = _PROC;
    idtable[idtindex].array = 0;
    idtable[idtindex].block = crtblock;
    idtable[idtindex].addr = addr;
    addr += 8;
    check_idtab(idtable[idtindex]);
    idtindex ++;
    blocknum ++;
    crtblock += blocknum;
    //写符号表
    inter[interindex].op = _LABEL;
    strcpy(inter[interindex].result, strcat("func_",program[last_word()]));
    interindex ++;
    //LABEL func_a
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _para_list();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _comp_state();
    if (progsym[travel] == RBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    crtblock -= blocknum;
    //
    inter[interindex].op = _JUMP;
    strcpy(inter[interindex].result, "return");
    interindex ++;
    //JUMP return
    printf("    #This is a return value function definition!#\n");
    return 1;
}

int _void_fun_def()         //＜无返回值函数定义＞  ::= void＜标识符＞‘(’＜参数表＞‘)’‘{’＜复合语句＞‘}’
{
    if (progsym[travel]==VARSYM && strcmp(program[travel],"void")==0)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel]==MAINSYM) return 0;
    _ident();
    //
    strcpy(idtable[idtindex].name, program[last_word()]);
    idtable[idtindex].idt = _PROC;
    idtable[idtindex].ida = _VOID;
    idtable[idtindex].array = 0;
    idtable[idtindex].block = crtblock;
    idtable[idtindex].addr = addr;
    addr += 8;
    check_idtab(idtable[idtindex]);
    idtindex ++;
    blocknum ++;
    crtblock += blocknum;
    //写符号表
    inter[interindex].op = _LABEL;
    strcpy(inter[interindex].result, strcat("func_",program[last_word()]));
    interindex ++;
    //LABEL func_a
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _para_list();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _comp_state();
    if (progsym[travel] == RBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    crtblock -= blocknum;
    //
    inter[interindex].op = _JUMP;
    strcpy(inter[interindex].result, "return");
    interindex ++;
    //JUMP return
    printf("    #This is a void function definition!#\n");
    return 1;
}

int _main_fun_def()         //＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
{
    if (progsym[travel]==VARSYM && strcmp(program[travel],"void"))
    {
        printf("%s", program[travel]);
        next_word();
    }
    else if (progsym[travel]!=MAINSYM) return 0;
    if (progsym[travel]==MAINSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    blocknum ++;
    crtblock += blocknum;
    _comp_state();
    if (progsym[travel] == RBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    crtblock -= blocknum;
    printf("    #This is a main function definition!#\n");
    return 1;
}

int _const_def()            //＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
{
    if (progsym[travel] == VARSYM)
        if (strcmp(program[travel],"int")==0)
        {
            printf("%s", program[travel]);
            next_word();
            _ident();
            //
            strcpy(idtable[idtindex].name, program[last_word()]);
            idtable[idtindex].idt = _CONST;
            idtable[idtindex].ida = _INT;
            idtable[idtindex].array = 0;
            idtable[idtindex].block = crtblock;
            idtable[idtindex].addr = addr;
            check_idtab(idtable[idtindex]);
            idtindex ++;
            addr += 8;
            //写符号表
            if (progsym[travel] == BECOME)
            {
                printf("%s", program[travel]);
                next_word();
            }
            _int();
            while (1)
            {
                if (progsym[travel] == COMMA)
                {
                    printf("%s", program[travel]);
                    next_word();
                }
                else break;
                _ident();
                //
                strcpy(idtable[idtindex].name, program[last_word()]);
                idtable[idtindex].idt = _CONST;
                idtable[idtindex].ida = _INT;
                idtable[idtindex].array = 0;
                idtable[idtindex].block = crtblock;
                idtable[idtindex].addr = addr;
                check_idtab(idtable[idtindex]);
                idtindex ++;
                addr += 8;
                //写符号表
                if (progsym[travel] == BECOME)
                {
                    printf("%s", program[travel]);
                    next_word();
                }
                _int();
            }
        }
        else if (strcmp(program[travel],"char")==0)
        {
            printf("%s", program[travel]);
            next_word();
            _ident();
            //
            strcpy(idtable[idtindex].name, program[last_word()]);
            idtable[idtindex].idt = _CONST;
            idtable[idtindex].ida = _CHAR;
            idtable[idtindex].array = 0;
            idtable[idtindex].block = crtblock;
            idtable[idtindex].addr = addr;
            check_idtab(idtable[idtindex]);
            idtindex ++;
            addr += 8;
            //写符号表
            if (progsym[travel] == BECOME)
            {
                printf("%s", program[travel]);
                next_word();
            }
            _char();
            while (1)
            {
                if (progsym[travel] == COMMA)
                {
                    printf("%s", program[travel]);
                    next_word();
                }
                else break;
                _ident();
                //
                strcpy(idtable[idtindex].name, program[last_word()]);
                idtable[idtindex].idt = _CONST;
                idtable[idtindex].ida = _CHAR;
                idtable[idtindex].array = 0;
                idtable[idtindex].block = crtblock;
                idtable[idtindex].addr = addr;
                check_idtab(idtable[idtindex]);
                idtindex ++;
                addr += 8;
                //写符号表
                if (progsym[travel] == BECOME)
                {
                    printf("%s", program[travel]);
                    next_word();
                }
                _char();
            }
        }
        else return 0;
    else return 0;
    return 1;
}

int _var_def()          //＜变量定义＞  ::= ＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
{
    if (strcmp(program[travel],"int") == 0) idtable[idtindex].ida = _INT;
    if (strcmp(program[travel],"char") == 0) idtable[idtindex].ida = _CHAR;
    if (!_type_ident()) return 0;
    _ident();
    if (progsym[travel]==LPAREN) return 1;
    //
    strcpy(idtable[idtindex].name, program[last_word()]);
    idtable[idtindex].idt = _VAR;
    idtable[idtindex].array = 0;
    idtable[idtindex].block = crtblock;
    idtable[idtindex].addr = addr;
    //写符号表
    if (progsym[travel] == LBRACK)
    {
        printf("%s", program[travel]);
        next_word();
        _unsigned_int();
        addr += string_to_num(program[last_word()])*8;
        idtable[idtindex].array = string_to_num(program[last_word()]);
        if (progsym[travel] == RBRACK)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    else
    {
        addr += 8;
    }
    check_idtab(idtable[idtindex]);
    idtindex ++;
    while (1)
    {
        if (progsym[travel] == COMMA)
        {
            printf("%s", program[travel]);
            next_word();
        }
        else break;
        idtable[idtindex].ida = idtable[idtindex-1].ida;
        idtable[idtindex].idt = _VAR;
        idtable[idtindex].array = 0;
        idtable[idtindex].block = crtblock;
        idtable[idtindex].addr = addr;
        _ident();
        strcpy(idtable[idtindex].name, program[last_word()]);
        if (progsym[travel] == LBRACK)
        {
            printf("%s", program[travel]);
            next_word();
            _unsigned_int();
            addr += 8;
            idtable[idtindex].array = string_to_num(program[last_word()]);
            if (progsym[travel] == RBRACK)
            {
                printf("%s", program[travel]);
                next_word();
            }
        }
        else
        {
            addr += 8;
        }
        check_idtab(idtable[idtindex]);
        idtindex ++;
    }
    return 1;
}

int _type_ident()           //＜类型标识符＞      ::=  int | char
{
    if (progsym[travel]==VARSYM )
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _dec_head()         //＜声明头部＞   ::=  int＜标识符＞|char＜标识符＞
{
    if (progsym[travel]==VARSYM )
    {
        if (strcmp(program[travel],"void") == 0) return 0;
        printf("%s", program[travel]);
        if (strcmp(program[travel],"int") == 0) idtable[idtindex].ida = _INT;
        if (strcmp(program[travel],"char") == 0) idtable[idtindex].ida = _CHAR;
        next_word();
    }
    else return 0;
    _ident();
    strcpy(idtable[idtindex].name, program[last_word()]);
    return 1;
}

int _para_list()            //＜参数表＞    ::=  ＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}| ＜空＞
{
    if (strcmp(program[travel],"int") == 0) idtable[idtindex].ida = _INT;
    if (strcmp(program[travel],"char") == 0) idtable[idtindex].ida = _CHAR;
    if (!_type_ident()) return 0;
    _ident();
    strcpy(idtable[idtindex].name, program[last_word()]);
    idtable[idtindex].idt = _PARA;
    idtable[idtindex].array = 0;
    idtable[idtindex].block = crtblock;
    idtable[idtindex].addr = addr;
    addr += 8;
    check_idtab(idtable[idtindex]);
    idtindex ++;
    while (1)
    {
        if (progsym[travel] == COMMA)
        {
            printf("%s", program[travel]);
            next_word();
        }
        else break;
        if (strcmp(program[travel],"int") == 0) idtable[idtindex].ida = _INT;
        if (strcmp(program[travel],"char") == 0) idtable[idtindex].ida = _CHAR;
        _type_ident();
        _ident();
        strcpy(idtable[idtindex].name, program[last_word()]);
        idtable[idtindex].idt = _PARA;
        idtable[idtindex].array = 0;
        idtable[idtindex].block = crtblock;
        idtable[idtindex].addr = addr;
        addr += 8;
        check_idtab(idtable[idtindex]);
        idtindex ++;
    }
    return 1;
}

int _comp_state()           //＜复合语句＞   ::=  ［＜常量说明＞］［＜变量说明＞］＜语句列＞
{
    _const_dec();
    _var_dec();
    if(!_state_list()) return 0;
    return 1;
}

int _state_list()           //＜语句列＞   ::=｛＜语句＞｝
{
    while (1)
    {
        if (!_state()) break;
    }
    return 1;
}

int _state()                //＜语句＞    ::= ＜条件语句＞｜＜循环语句＞｜<情况语句>|‘{’＜语句列＞‘}’｜
                            //＜有返回值函数调用语句＞;  |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
{
    if (_if_state()) return 1;
    if (_while_state()) return 1;
    if (_switch_state()) return 1;
    if (progsym[travel] == LBRACE)
    {
        printf("%s", program[travel]);
        next_word();
        _state_list();
        if (progsym[travel] == RBRACE)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_return_fun_call())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_void_fun_call())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_assign_state())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_read_state())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_write_state())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (_return_state())
    {
        if (progsym[travel] == SEMICOLON)
        {
            printf("%s", program[travel]);
            next_word();
        }
        return 1;
    }
    if (progsym[travel] == SEMICOLON)
    {
        printf("%s", program[travel]);
        next_word();
        return 1;
    }
    return 0;
}

int _if_state()         //＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞
{
    if (progsym[travel] == IFSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _condition();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _state();
    printf("    #This is a if statement!#\n");
    return 1;
}

int _condition()            //＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
{
    if (!_express()) return 0;
    if (_relat_op())
    {
        _express();
    }
    return 1;
}

int _while_state()          //＜循环语句＞   ::=  do＜语句＞while ‘(’＜条件＞‘)’
{
    if (progsym[travel] == DOSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    _state();
    if (progsym[travel] == WHILESYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _condition();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a do-while statement!#\n");
    return 1;
}

int _switch_state()         //＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞ ‘}’
{
    if (progsym[travel] == SWITCHSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _express();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (progsym[travel] == LBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _case_list();
    if (progsym[travel] == RBRACE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a switch statement!#\n");
    return 1;
}

int _case_list()            //＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
{
    if (!_case_sub_state()) return 0;
    while (1)
    {
        if (!_case_sub_state()) break;
    }
    return 1;
}

int _case_sub_state()           //＜情况子语句＞  ::=  case＜常量＞：＜语句＞
{

    if (progsym[travel] == CASESYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    _const();
    if (progsym[travel] == COLON)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _state();
    return 1;
}

int _return_fun_call()          //＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
{
    if (!_ident() && progsym[travel]!=LPAREN) return 0;
    if (progsym[travel]==BECOME || progsym[travel]==LBRACK) return 0;
    check_id(program[last_word()]);
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _value_para_list();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a function call!#\n");
    return 1;
}

int _void_fun_call()            //＜无返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’
{
    if (!_ident()) return 0;
    if (progsym[travel]==BECOME || progsym[travel]==LBRACK) return 0;
    check_id(program[last_word()]);
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _value_para_list();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a function call!#\n");
    return 1;
}

int _value_para_list()          //＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}｜＜空＞
{
    if (!_express()) return 0;
    while (1)
    {
        if (progsym[travel] == COMMA)
        {
            printf("%s", program[travel]);
            next_word();
        }
        else break;
        _express();
    }
    return 0;
}

int _assign_state()         //＜赋值语句＞   ::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
{
    if (!_ident() && progsym[travel]!=BECOME && progsym[travel]!=LBRACK) return 0;
    check_id(program[last_word()]);
    if (progsym[travel] == LBRACK)
    {
        printf("%s", program[travel]);
        next_word();
        _express();
        if (progsym[travel] == RBRACK)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    if (progsym[travel] == BECOME)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _express();
    printf("    #This is a assignment statement!#\n");
    return 1;
}

int _read_state()           //＜读语句＞    ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
{
    if (progsym[travel] == SCANSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    _ident();
    check_id(program[last_word()]);
    while (1)
    {
        if (progsym[travel] == COMMA)
        {
            printf("%s", program[travel]);
            next_word();
        }
        else break;
        _ident();
        check_id(program[last_word()]);
    }
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a read statement!#\n");
    return 1;
}

int _write_state()          //＜写语句＞    ::=  printf‘(’＜字符串＞,＜表达式＞‘)’|printf ‘(’＜字符串＞‘)’|printf ‘(’＜表达式＞‘)’
{
    if (progsym[travel] == PRINTSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    if (_string())
    {
        printf("%s", program[travel]);
        next_word();
        if (progsym[travel] == COMMA)
        {
            printf("%s", program[travel]);
            next_word();
        }
        _express();
    }
    else _express();
    if (progsym[travel] == RPAREN)
    {
        printf("%s", program[travel]);
        next_word();
    }
    printf("    #This is a write statement!#\n");
    return 1;
}

int _return_state()         //＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
{
    if (progsym[travel] == RETURNSYM)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
        _express();
        if (progsym[travel] == RPAREN)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    printf("    #This is a return statement!#\n");
    return 1;
}

int _express()          //＜表达式＞    ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}
{
    _add_op();
    if (!_term()) return 0;
    while (1)
    {
        if (!_add_op()) break;
        _term();
    }
    return 1;
}

int _term()         //＜项＞     ::= ＜因子＞{＜乘法运算符＞＜因子＞}
{
    if (!_factor()) return 0;
    while (1)
    {
        if (!_mult_op()) break;
        _factor();
    }
    return 1;
}

int _factor()           //＜因子＞    ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
{
    if (_ident())
    {
        check_id(program[last_word()]);
        if (progsym[travel] == LBRACK)
        {
            printf("%s", program[travel]);
            next_word();
            _express();
            if (progsym[travel] == RBRACK)
            {
                printf("%s", program[travel]);
                next_word();
            }
            return 1;
        }
        if (progsym[travel] == LPAREN)
        {
            _return_fun_call();
            return 1;
        }
        return 1;
    }
    if (_int()) return 1;
    if (_char()) return 1;
    if (progsym[travel] == LPAREN)
    {
        printf("%s", program[travel]);
        next_word();
        _express();
        if (progsym[travel] == RPAREN)
        {
            printf("%s", program[travel]);
            next_word();
        }
    }
    return 1;

}

int _const()            //＜常量＞   ::=  ＜整数＞|＜字符＞
{
    if (_int())
    {
        return 1;
    }
    if (_char())
    {
        return 1;
    }
    return 0;
}

int _ident()            //＜标识符＞    ::=  ＜字母＞｛＜字母＞｜＜数字＞｝
{
    if (progsym[travel] == IDENT)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _int()          //＜整数＞        ::= ［＋｜－］＜无符号整数＞｜０
{
    int neg = 1;
    if (progsym[travel]==PLUS || progsym[travel]==MINUS)
    {
        if (progsym[travel]==MINUS) neg = -1;
        printf("%s", program[travel]);
        next_word();
    }
    if (!_unsigned_int())
    {
        operstack[optop].ntype = _datatype;
        operstack[optop].dtype = _int;
        operstack[optop].con = program[last_word()]*neg;
        optop ++;
        return 0;
    }
    return 1;
}

int _unsigned_int()         //＜无符号整数＞  ::= ＜非零数字＞｛＜数字＞｝
{
    if (progsym[travel] == NUMBER)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _char()         //＜字符＞    ::=  '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
{
    if (progsym[travel] == CHAR)
    {
        operstack[optop].ntype = _datatype;
        operstack[optop].dtype = _char;
        operstack[optop].con = program[last_word()];
        optop ++;
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _string()           //＜字符串＞   ::=  "｛十进制编码为32,33,35-126的ASCII字符｝"
{
    if (progsym[travel] == STRING)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _relat_op()             //＜关系运算符＞  ::=  <｜<=｜>｜>=｜!=｜==
{
    if (progsym[travel]==LSS || progsym[travel]==LEQ || progsym[travel]==GTR || progsym[travel]==GEQ || progsym[travel]==NEQ || progsym[travel]==EQL)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _add_op()           //＜加法运算符＞ ::= +｜-
{
    if (progsym[travel]==PLUS || progsym[travel]==MINUS)
    {
        if (progsym[travel]==PLUS)
        {
            operstack[optop].ntype = _optype;
            operstack[optop].otype = _plus;
            operstack[optop].childnum = 2;
            optop ++;
        }

        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _mult_op()          //＜乘法运算符＞  ::= *｜/
{
    if (progsym[travel]==TIMES || progsym[travel]==DEVIDE)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

void check_idtab(struct idtable idtab)
{
    for (int temp=0; temp<idtindex; temp++)
    {
        if (strcmp(idtab.name, idtable[temp].name)==0 && (idtable[temp].block==0 || idtab.block==idtable[temp].block))
        {
            printf ("\n program error !");
            exit (0);
        }
    }
}

void check_id(char name[100])
{
    int t=0;
    for (int temp=0; temp<idtindex; temp++)
    {
        if (strcmp(name, idtable[temp].name)==0 && (idtable[temp].block==0 || crtblock==idtable[temp].block))
        {
            t=1;
        }
    }
    if (t==0)
    {
            printf ("\n program error !");
            exit (0);
    }
}


int main()
{
    fin = fopen("15061198_test.txt","r");
    int ct = 0;
    while(1)
    {
        if (eof)
            break;
        getsym();
        ct ++;
    }
	int i;
    _program();
    for (int temp=0; temp<idtindex; temp++)
    {
        printf("%s  *  ", idtable[temp].name);
        switch (idtable[temp].ida)
        {
            case _INT: printf("int  *  "); break;
            case _CHAR: printf("char  *  "); break;
            case _VOID: printf("void  *  "); break;
        }
        switch (idtable[temp].idt)
        {
            case _VAR: printf("var  *  "); break;
            case _CONST: printf("const  *  "); break;
            case _PROC: printf("proc  *  "); break;
            case _PARA: printf("para  *  "); break;
        }
        printf("%d  *  ", idtable[temp].array);
        printf("%d\n", idtable[temp].block);
    }
}

int string_to_num(char *s)
{
    int n;
    for(n=0;*s;s++)
    {
        n=n*10+*s-'0';
    }
    return n;
}

void error(int errno)
{

}
