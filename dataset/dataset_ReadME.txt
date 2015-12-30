folder文件夹的数据集由两则关于2016年美国总统选举民调预测的新闻构成，分别从BBC新闻和纽约时报的网站获取。

2016.aql 为文本提取文件一共提取了7个视图，分别是：
    
    1. 民主党候选人DemocratCandidate，共和党候选人RepublicanCandidate。
       鉴于参选人是固定的，采用正则表达式直接提取候选人full name或其first name.
    2. 数字Number，百分比Percent,民调比率Ratio。这三个视图主要提取、输出新闻中的民调数据。
       其中Percent的表现形式有%或者单词percent。采用pattern语句认定数字后面紧跟百分比符号或
       单词percent的数据信息为民调分析数据。
    3. 民主党候选人民调数字DemCandidateWithRatio，共和党候选人民调数字RepCandidateWithRatio。
       这两个视图由前面的两类视图关联而成。使用pattern语句认定候选人名字后面隔一定数量token如果连接民调数据，
       则关联为该候选人的民调信息。

    该样例测试了文件批处理、regex、pattern、group语句以及创建输出视图等AQL基本语法。

PerLoc.aql 对应PerLoc.input，是给定的测例。

Revenue.aql 对应Revenue.input，是给定的测例。
