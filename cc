#!/usr/bin/env python3
from collections import OrderedDict
from queue import PriorityQueue
import sys,os
import argparse

#declaring and reading values of command line arguments using argparse
parser = argparse.ArgumentParser()
parser.add_argument('i', help='input file to be parsed')
parser.add_argument('-o', nargs='?',default='a.c', help='name of the output file')
args = parser.parse_args()
ifn = args.i
ofn = args.o

#dict for storing declared values of system signal and process
sys_t = {'SYSTEM':[],'SIGNAL':[],'PROCESS':[]}








#lexer
tokens = ('SYSTEM','SIGNAL','PROCESS','PROCESSDEF','START','END','PROCESSBODY','BLOCK','DECISION','IF','THEN','GOTO','ELSE','UNCONDDEC','MESSAGE','ID','COMMA','SEMICOLON','DQUOT','MCOMS','MCOMC','SCOMS')
reserved = {}
for x in tokens:
    reserved.update({x:x})

t_SYSTEM = r'SYSTEM'
t_SIGNAL = r'SIGNAL'
t_PROCESS = r'PROCESS'
t_PROCESSDEF = r'PROCESSDEF'
t_START = r'START'
t_END = r'END'
t_PROCESSBODY = r'PROCESSBODY'
t_BLOCK = r'BLOCK'
t_COMMA = r','
t_SEMICOLON = r';'

def t_DQUOT(t):
    r'\"'
    t.lexer.begin('str')
    t.type = 'ID'
    return t
def t_MCOMS(t):
    r'/\*'
    t.lexer.begin('comm')
    t.type = 'ID'
    return t
def t_SCOMS(t):
    r'//'
    t.lexer.begin('coms')
    t.type = 'ID'
    return t

def t_ID(t):
    r'[^ \t\n\,\;\"]+'
    if t.value in reserved:
        t.type = reserved[ t.value ]
    return t

t_ignore = " \t\n"

states = (
   ('comm','exclusive'),
   ('coms','exclusive'),
   ('str','exclusive'),
)

def t_comm_ID(t):
    r'[^\*/]+\*/'
    t.lexer.begin('INITIAL')
    t.value = t.value + '\n'
    return t

def t_coms_ID(t):
    r'.*\n'
    t.lexer.begin('INITIAL')
    return t

def t_str_ID(t):
    r'.*\"'
    t.lexer.begin('INITIAL')
    return t


t_coms_ignore = ''
t_comm_ignore = ''
t_str_ignore = ''
def t_comm_error(t):
    print("Unable to reognise token '%s'" % t.value)

def t_coms_error(t):
    print("Unable to reognise token '%s'" % t.value)

def t_str_error(t):
    print("Unable to reognise token '%s'" % t.value)
    
def t_error(t):
    print("Unable to reognise token '%s'" % t.value)


import ply.lex as lex
lexer = lex.lex()








#parser
cur_s = 'SYSTEM'
tar_b = ''
temp_txt =''
grph = OrderedDict()
coded = {}

def p_statment_s(t):
    '''start : sys sig pro prodef'''
    
def p_statment_sys(t):
    '''sys : SYSTEM ID idr'''
    global cur_s
    sys_t[cur_s].insert(0,t[2])
    cur_s = 'SIGNAL'
    
def p_statment_sig(t):
    '''sig : SIGNAL ID idr
    | '''
    global cur_s
    try:
        sys_t[cur_s].insert(0,t[2])
    except(IndexError):
        pass
    cur_s = 'PROCESS'

def p_statment_pro(t):
    '''pro : PROCESS ID idr
    | '''
    global cur_s
    try:
        sys_t[cur_s].insert(0,t[2])
    except(IndexError):
        pass
    
def p_statment_idr(t):
    '''idr : COMMA ID idr
    | SEMICOLON'''
    global cur_s
    try:
        sys_t[cur_s].insert(0,t[2])
    except(IndexError):
        pass

def p_statment_prodef(t):
    '''prodef : prodef PROCESSDEF ID START prodefd END
    | '''
    try:
        if(t[3] not in sys_t['PROCESS']):
            print('Error : {} is defined but is not declared as PROCESS'.format(t[3]))
            raise SyntaxError
    except(IndexError):
        pass
    except(SyntaxError):
        sys.exit()

def p_statment_prodefd(t):
    '''prodefd : prodefd BLOCK prcb
    | prodefd DECISION ifc thnc elsc
    | '''

def p_statment_prcsb(t):
    '''prcb : ID START txt END'''
    global grph,tar_b,coded,temp_txt
    if (t[1] not in grph.keys()):
        grph[t[1]] = []
    grph[t[1]].append(tar_b)
    coded[t[1]] = temp_txt
    temp_txt = ''
    tar_b =''

if_id = ''
def p_statment_ifc(t):
    '''ifc : ID IF txt'''
    global tar_b
    global if_id
    if_id = t[1]
    global temp_txt
    coded[if_id] = []
    coded[if_id].append(temp_txt)
    temp_txt = ''
    
    
def p_statment_thnc(t):
    '''thnc : THEN txt'''
    global tar_b,if_id
    if (if_id not in grph.keys()):
        grph[if_id] = []
    grph[if_id].append(tar_b)
    global temp_txt
    coded[if_id].append(temp_txt)
    temp_txt = ''
    tar_b =''
    
def p_statment_elsc(t):
    '''elsc : ELSE txt
    | '''
    global tar_b,if_id
    if (if_id not in grph.keys()):
        grph[if_id] = []
    grph[if_id].append(tar_b)
    tar_b =''
    
    global temp_txt
    try:
        if(t[1] == 'ELSE'):
            coded[if_id].append(temp_txt)
    except(IndexError):
        pass
    temp_txt = ''
    tar_b =''
    pass

def p_statment_txt(t):
    '''txt : txt txtr
    | '''

def p_statment_txtr(t):
    '''txtr : ID
    | COMMA
    | SEMICOLON
    | GOTO ID discard'''
    global temp_txt,tar_b
    if(t[1] == 'GOTO'):
        tar_b = t[2]
    elif(t[1] == ';'):
        temp_txt = temp_txt + t[1]
    elif(t[1] == ','):
        temp_txt = temp_txt + t[1]
    else:
        temp_txt = temp_txt + ' ' + t[1]

def p_discard(t):
    '''discard : discard discardr
    | '''

def p_discardr(t):
    '''discardr : ID
    | COMMA
    | SEMICOLON
    | GOTO ID'''

def p_error(t):
    print("Syntax error at '%s'" % t.value)
    raise(SyntaxError)

import ply.yacc as yacc
parser = yacc.yacc()







#reading file in a variable
try:
    with open(ifn, 'r') as myfile:
        s=myfile.read()
except EOFError:
    pass





#generating output file to store result    
try:
    os.remove(ofn)
except:
    pass
ofo = open(ofn,'a')

#parsing the file
try:
    parser.parse(s)
except:
    os.remove(ofn)
    sys.exit()



#adding succeeding block indexes to blocks not having goto
keys = list(grph.keys())
for x in range(len(keys)):
    try:
        if(len(grph[keys[x]]) == 1 and grph[keys[x]][0] == ''):
            grph[keys[x]][0] = keys[x+1]
    except:
        pass





#checking which blocks will require labels by traversing graph
fref = {}
visited = []
def rec_fref(nd):
    try:
        if(grph[nd][0] != '' and grph[nd][0] not in list(fref.keys())):
            fref.update({grph[nd][0]:[nd]})
            rec_fref(grph[nd][0])
        elif(grph[nd][0] != '' and nd not in visited):
            fref[grph[nd][0]].append(nd)
            #rec_fref(grph[nd][0])
        if(grph[nd][1] != '' and grph[nd][1] not in list(fref.keys())):
            fref.update({grph[nd][1]:[nd]})
            rec_fref(grph[nd][1])
        elif(grph[nd][1] != '' and nd not in visited):
            fref[grph[nd][1]].append(nd)
            #rec_fref(grph[nd][1])
        if(grph[nd][0] == nd):
            fref[grph[nd][0]].append(nd)
        if(grph[nd][1] == nd):
            fref[grph[nd][1]].append(nd)
    except IndexError:
        pass
    except KeyError as e:
        print('KeyError:',str(e),'referenced in',fref[nd],',but is not defined' )
        os.remove(ofn)
        sys.exit()    
    visited.append(nd)

try:
    rec_fref(keys[0])
except IndexError:
    pass


#use if above method gives error
#for x in keys:
    #for y in grph[x]:
        #if(y != '' and y not in list(fref.keys())):
            #fref.update({y:[x]})
        #elif(y in list(fref.keys())):
            #fref[y].append(x)





#traversing graph and recursively writing result to the output file
visited = []
frefk = list(fref.keys())
n_ind = 1
import traceback
def rec_prnt(nd):
    try:
        global n_ind
        visited.append(nd)
        if(nd in frefk and (len(fref[nd]) > 1 or nd in fref[nd])):
            ofo.write('\n'+'\t'*n_ind+nd+':')
        if(len(grph[nd]) == 1):
            curc = coded[nd].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
            if(curc != ''):
                curc = coded[nd].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                ofo.write('\n'+'\t'*n_ind+curc)
            if(grph[nd][0] != '' and grph[nd][0] not in visited):
                rec_prnt(grph[nd][0])
            elif(grph[nd][0] != ''):
                ofo.write('\n'+'\t'*n_ind+'goto '+grph[nd][0]+';')
        if(len(grph[nd]) == 2):
            ele_f = False
            try:
                if(coded[nd][2] or coded[nd][2] == ''):
                    ele_f = True
            except:
                ele_f = False
                
            if(not ele_f):
                curc = coded[nd][0].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                ofo.write('\n'+'\t'*n_ind+'if('+curc+'){')
                n_ind += 1
                try:
                    curc = coded[nd][1].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                    if(curc != ''):
                        curc = coded[nd][1].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                        ofo.write('\n'+'\t'*n_ind+curc)
                except:
                    pass
                if(grph[nd][0] != '' and grph[nd][0] not in visited):
                    rec_prnt(grph[nd][0])
                elif(grph[nd][0] != ''):
                    ofo.write('\n'+'\t'*n_ind+'goto '+grph[nd][0]+';')
                n_ind -= 1
                ofo.write('\n'+'\t'*n_ind+'}')
                
            else:
                curc = coded[nd][0].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                ofo.write('\n'+'\t'*n_ind+'if('+curc+'){')
                n_ind += 1
                try:
                    curc = coded[nd][1].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                    if(curc != ''):
                        curc = coded[nd][1].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                        ofo.write('\n'+'\t'*n_ind + curc)
                except:                
                    pass
                if(grph[nd][0] != '' and grph[nd][0] not in visited):
                    rec_prnt(grph[nd][0])
                elif(grph[nd][0] != ''):
                    ofo.write('\n'+'\t'*n_ind+'goto '+grph[nd][0]+';')
                n_ind -= 1
                ofo.write('\n'+'\t'*n_ind+'}\n'+'\t'*n_ind+'else{')
                n_ind += 1
                try:
                    curc = coded[nd][2].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                    if(curc != ''):
                        curc = coded[nd][2].strip('\n').strip(' ').replace('\n','\n'+'\t'*n_ind)
                        ofo.write('\n'+'\t'*n_ind+curc)
                except:
                    pass
                if(grph[nd][1] != ''  and grph[nd][1] not in visited):
                    rec_prnt(grph[nd][1])
                elif(grph[nd][0] != ''):
                    ofo.write('\n'+'\t'*n_ind+'goto '+grph[nd][1]+';')
                n_ind -= 1
                ofo.write('\n'+'\t'*n_ind+'}')
    except KeyError as e:
        try:
            print('KeyError:',str(e),'referenced in',str(fref[str(e)[1:-1]]),',but is not defined' )
        except:
            traceback.print_tb(e.__traceback__)
            print('KeyError:',str(e))





#printing header and calling recursive print to write final output in output file
hdr = '#include<stdio.h>\nint main(){'
ftr = '\nreturn 0;\n}'
ofo.write(hdr)
#for x in keys:
    #if(x not in visited):
        #rec_prnt(x)
try:
    rec_prnt(keys[0])
except(IndexError):
    pass
ofo.write(ftr)
ofo.close()
