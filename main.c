#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define norw  13        //13��������
#define txmax  100      //��ʶ������
#define al 100          //��ʶ����������

//������ż������ֶ�Ӧ�ķ���
typedef enum symbol
{
    IDENT,NUMBER,STRING,CHAR,          //��ʶ�������֣��ַ���
    PLUS,MINUS,TIMES,DEVIDE,BECOME,   //�ӣ������ˣ�������ֵ
    EQL,NEQ,LSS,LEQ,GTR,GEQ,           //==,!=,<,<=,>,>=
    LPAREN,RPAREN,LBRACK,RBRACK,LBRACE,
    RBRACE,COMMA,SEMICOLON,COLON,      //(��)��[��]��{��}��,��;��:
    MAINSYM,CONSTSYM,VARSYM,IFSYM,
    DOSYM,WHILESYM,SWITCHSYM,CASESYM,
    SCANSYM,PRINTSYM,RETURNSYM,        //�������ֵı�ʶ
    SPACE                              //�ո�
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
    symbol sym;     //��ȡ����һ��symbol
    char ch = ' ';      //��ȡ����һ���ַ�
    char crtword[al];   //��ǰ����
    int num;            //��ǰ����
    int cc,ll;
    int lcount = 1;     //�м�����
    int eof = 0;        //�ļ���ȡ�����ж�
    char program[99999][100];    //�����ַ��洢
    symbol progsym[99999];     //�������ʹ洢
    int wdcount = 0;            //���뵥�ʼ�����
    int contract = 0;               //�﷨��ͻ��ʶ

   // int level;          //Ƕ�ײ���
   // char *idt[];     //����������������

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
    int k;        //��������������
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

int _program();            //����
int _const_dec();          //����˵��
int _var_dec();            //����˵��
int _return_fun_def();     //�з���ֵ��������
int _void_fun_def();       //�޷���ֵ��������
int _main_fun_def();       //������
int _const_def();          //��������
int _var_def();            //��������
int _type_ident();         //���ͱ�ʶ��
int _dec_head();           //����ͷ��
int _para_list();          //������
int _comp_state();         //�������
int _state_list();         //�����
int _state();              //���
int _if_state();           //�������
int _condition();          //����
int _while_state();        //ѭ�����
int _switch_state();       //������
int _case_list();          //�����
int _case_sub_state();     //��������
int _return_fun_call();    //�з���ֵ��������
int _void_fun_call();      //�޷���ֵ��������
int _value_para_list();    //ֵ������
int _assign_state();       //��ֵ���
int _read_state();         //�����
int _write_state();        //д���
int _return_state();       //�������
int _express();            //���ʽ
int _term();               //��
int _factor();             //����
int _const();              //����
int _ident();              //��ʶ��
int _int();                //����
int _unsigned_int();       //�޷�������
int _num();                //����
int nonzero_num();         //��������
int _char();               //�ַ�
int _string();             //�ַ���
int _letter();             //��ĸ
int _relat_op();           //��ϵ�����
int _add_op();             //�ӷ������
int _mult_op();            //�˷������

int travel = -1;                 //������ʶ

struct idtable
{
    char name[100];
    idtype idt;
    idattr ida;
    int array;
    int block;
    int addr;
} idtable[];                  //���ű�
int crtblock = 0;               //������
int blocknum = 0;               //��������
int addr = 0;
int idtindex = 0;               //���ű�����


int labelct = 0;                //��ǩ����
int interindex = 0;             //��Ԫʽ������

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

int _program()              //������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
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

int _const_dec()            //const���������壾;{ const���������壾;}
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

int _var_dec()              //������˵����  ::= ���������壾;{���������壾;}
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

int _return_fun_def()       //���з���ֵ�������壾  ::=  ������ͷ������(������������)�� ��{����������䣾��}��
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
    //д���ű�
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

int _void_fun_def()         //���޷���ֵ�������壾  ::= void����ʶ������(������������)����{����������䣾��}��
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
    //д���ű�
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

int _main_fun_def()         //����������    ::= void main��(����)�� ��{����������䣾��}��
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

int _const_def()            //���������壾   ::=   int����ʶ��������������{,����ʶ��������������} | char����ʶ���������ַ���{,����ʶ���������ַ���}
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
            //д���ű�
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
                //д���ű�
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
            //д���ű�
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
                //д���ű�
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

int _var_def()          //���������壾  ::= �����ͱ�ʶ����(����ʶ����|����ʶ������[�����޷�����������]��){,(����ʶ����|����ʶ������[�����޷�����������]��) }
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
    //д���ű�
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

int _type_ident()           //�����ͱ�ʶ����      ::=  int | char
{
    if (progsym[travel]==VARSYM )
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _dec_head()         //������ͷ����   ::=  int����ʶ����|char����ʶ����
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

int _para_list()            //��������    ::=  �����ͱ�ʶ��������ʶ����{,�����ͱ�ʶ��������ʶ����}| ���գ�
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

int _comp_state()           //��������䣾   ::=  �ۣ�����˵�����ݣۣ�����˵�����ݣ�����У�
{
    _const_dec();
    _var_dec();
    if(!_state_list()) return 0;
    return 1;
}

int _state_list()           //������У�   ::=������䣾��
{
    while (1)
    {
        if (!_state()) break;
    }
    return 1;
}

int _state()                //����䣾    ::= ��������䣾����ѭ����䣾��<������>|��{��������У���}����
                            //���з���ֵ����������䣾;  |���޷���ֵ����������䣾;������ֵ��䣾;��������䣾;����д��䣾;�����գ�;����������䣾;
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

int _if_state()         //��������䣾  ::=  if ��(������������)������䣾
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

int _condition()            //��������    ::=  �����ʽ������ϵ������������ʽ���������ʽ�� //���ʽΪ0����Ϊ�٣�����Ϊ��
{
    if (!_express()) return 0;
    if (_relat_op())
    {
        _express();
    }
    return 1;
}

int _while_state()          //��ѭ����䣾   ::=  do����䣾while ��(������������)��
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

int _switch_state()         //�������䣾  ::=  switch ��(�������ʽ����)�� ��{��������� ��}��
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

int _case_list()            //�������   ::=  ���������䣾{���������䣾}
{
    if (!_case_sub_state()) return 0;
    while (1)
    {
        if (!_case_sub_state()) break;
    }
    return 1;
}

int _case_sub_state()           //���������䣾  ::=  case��������������䣾
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

int _return_fun_call()          //���з���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
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

int _void_fun_call()            //���޷���ֵ����������䣾 ::= ����ʶ������(����ֵ��������)��
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

int _value_para_list()          //��ֵ������   ::= �����ʽ��{,�����ʽ��}�����գ�
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

int _assign_state()         //����ֵ��䣾   ::=  ����ʶ�����������ʽ��|����ʶ������[�������ʽ����]��=�����ʽ��
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

int _read_state()           //������䣾    ::=  scanf ��(������ʶ����{,����ʶ����}��)��
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

int _write_state()          //��д��䣾    ::=  printf��(�����ַ�����,�����ʽ����)��|printf ��(�����ַ�������)��|printf ��(�������ʽ����)��
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

int _return_state()         //��������䣾   ::=  return[��(�������ʽ����)��]
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

int _express()          //�����ʽ��    ::= �ۣ������ݣ��{���ӷ�����������}
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

int _term()         //���     ::= �����ӣ�{���˷�������������ӣ�}
{
    if (!_factor()) return 0;
    while (1)
    {
        if (!_mult_op()) break;
        _factor();
    }
    return 1;
}

int _factor()           //�����ӣ�    ::= ����ʶ����������ʶ������[�������ʽ����]������������|���ַ��������з���ֵ����������䣾|��(�������ʽ����)��
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

int _const()            //��������   ::=  ��������|���ַ���
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

int _ident()            //����ʶ����    ::=  ����ĸ��������ĸ���������֣���
{
    if (progsym[travel] == IDENT)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _int()          //��������        ::= �ۣ������ݣ��޷�������������
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

int _unsigned_int()         //���޷���������  ::= ���������֣��������֣���
{
    if (progsym[travel] == NUMBER)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _char()         //���ַ���    ::=  '���ӷ��������'��'���˷��������'��'����ĸ��'��'�����֣�'
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

int _string()           //���ַ�����   ::=  "��ʮ���Ʊ���Ϊ32,33,35-126��ASCII�ַ���"
{
    if (progsym[travel] == STRING)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _relat_op()             //����ϵ�������  ::=  <��<=��>��>=��!=��==
{
    if (progsym[travel]==LSS || progsym[travel]==LEQ || progsym[travel]==GTR || progsym[travel]==GEQ || progsym[travel]==NEQ || progsym[travel]==EQL)
    {
        printf("%s", program[travel]);
        next_word();
    }
    else return 0;
    return 1;
}

int _add_op()           //���ӷ�������� ::= +��-
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

int _mult_op()          //���˷��������  ::= *��/
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
