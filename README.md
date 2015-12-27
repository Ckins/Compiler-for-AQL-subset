# README

---

## examples
We have prepared some examples for you:

+ PerLoc
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
