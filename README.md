# README
## AQL(Annotation Query Language) Subset Compiler

### What Is The AQL Subset?
This project is not a complete Compiler of AQL, but it realizes the major part of it. The BNF grammar is listed below:
```
aql_stmt → create_stmt ; | output_stmt ;  
create_stmt → create view ID as view_stmt  
view_stmt → select_stmt | extract_stmt  
output_stmt → output view ID alias  
alias → as ID | ε  
elect_stmt → select select_list from from_list  
select_list → select_item | select_list , select_item  
select_item → ID . ID alias  
from_list → from_item | from_list , from_item  
from_item → ID ID  
extract_stmt → extract extract_spec from from_list  
extract_spec → regex_spec | pattern_spec  
regex_spec → regex REG on column name_spec  
column → ID . ID  
name_spec → as ID | return group_spec  
group_spec → single_group | group_spec and single_group  
single_group → group NUM as ID  
pattern_spec → pattern pattern_expr name_spec  
pattern_expr → pattern_pkg | pattern_expr pattern_pkg  
pattern_pkg → atom | atom { NUM , NUM } | pattern_group  
atom→ < column > | < Token > | REG  
pattern_group → ( pattern_expr )  
```
### Lexer
Scan the original language and output the terminator to gramma parser.

### Parser
Create a gramma tree and execute the program.

### Tokenizer
When executing the program, we need to get the tokens of the document to be executed, it would help us to divide token from the original document.

## environment
System: Ubuntu14.04LTS 64-bit   
Compiler: GNU g++ version 4.8.4

## examples
We have prepared some examples for you:
+ America_2016_election_news
```
aql_master/src/# make clean
aql_master/src/# make
aql_master/src/# make test1
```
+ Revenue
```
aql_master/src/# make clean
aql_master/src/# make
aql_master/src/# make test2
```
+ PerLoc
```
aql_master/src/# make clean
aql_master/src/# make
aql_master/src/# make test3
```

+ For More information, pls view:
```
/dataset/dataset_ReadME.txt
```

## extras

+ If you want to use your own data, you should put the ".aql" file (for instance, "your.aql") and the article (for instance, "your.text") into the "aql_master/dataset" catalog. Then:
```
aql_master/src/# make clean
aql_master/src/# make
aql_master/src/# ./AQL your.aql your.text
```

+ If your have more than one articles, you can put all of the articles into a folder (for instance, "articles", and certainly, it should be under "aql_master/dataset/"), and each article should end with ".input" (for instance, "a.input", "b.input"). Then:
```
aql_master/src/# make clean
aql_master/src/# make
aql_master/src/# ./AQL your.aql articles
```

## P.S.
each output will be seperated by 
> "==========================================================="
