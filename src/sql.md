## 3.2  信息查询

### 1 小区配置信息查询

```sql
在tbCell表上创建索引
alter table tbCell add constraint pk_sector_id primary key (SECTOR_ID) --ID主键
create nonclustered index SNameIndex on tbCell(SECTOR_NAME) --Name非聚集
```



```sql
(1) 用户在界面上输入小区ID Cell_ID/Sector_ID或小区名称CellName/SectorName，以列表方式输出该小区全部信息；
按照Sector ID取数
select * from tbCell where SECTOR_ID = $SECTOR_ID
按照Sector Name取数
select * from tbCell where SECTOR_NAME = $SECTOR_NAME
(2)	程序在查询界面下来列表中程序tbCell中全部小区名称，用户在界面上通过下拉列表选择特定小区名称，以列表方式输出该小区全部信息；
首先获取所有小区名称
select SECTOR_NAME from tbCell
```

```sql
获取列数（方便遍历获取的数据）
select count(a.name) as count from syscolumns a where a.id=object_id('tbCell')
```

### 2 基站eNodeB信息查询

```sql
用户在界面上输入、或者以下拉列表方式选择基站ID eNodeBID或基站名称eNodeBName，以列表方式输出该基站全部小区信息；
首先获取所有eNodeBID 或 基站名称
eNodeBID:
select distinct ENODEBID from tbCell order by ENODEBID asc
eNodeBName:
select distinct ENODEB_NAME from tbCell order by ENODEB_NAME asc
然后按照ID或者Name显示全部信息
eNodeBID:
select * from tbCell where ENODEBID = $ENODEBID
eNodeBName:
select * from tbCell where ENODEB_NAME = $ENODEB_NAME
```

### 3 KPI指标信息查询

```sql
通过人机界面对话框选定表tbKPI中的某个特定网元，查询数据库，列出该网元从2016年7月17日到7月19日的RRC连接重建比率 (%)，并将结果用柱状图或折线图的方式呈现出来，要求查询至少3个网元。
首先获取所有网元名称
select distinct 网元名称 from tbKPI order by 网元名称 asc
然后获取指定网元的小区1的个数
select count(*) from tbKPI where 网元名称 = $网元名称 group by 起始时间
然后获取指定网元的数据
select 起始时间 , 小区1 , [RRC连接重建比率 (%)] from tbKPI where 网元名称 = $网元名称order by 起始时间 asc, 小区1 asc
上述语句意图：
同一个网元有多个小区，所以检索起始时间，小区1的号，重建比率
起始时间	         小区1	             RRC连接重建比率 (%)
07/17/2016 00:00:00	三门峡义马常村-HLHF-1	0.006
07/17/2016 00:00:00	三门峡义马常村-HLHF-2	0.005
07/17/2016 00:00:00	三门峡义马常村-HLHF-3	0.013
07/18/2016 00:00:00	三门峡义马常村-HLHF-1	0.005
07/18/2016 00:00:00	三门峡义马常村-HLHF-2	0.004
07/18/2016 00:00:00	三门峡义马常村-HLHF-3	0.007
07/19/2016 00:00:00	三门峡义马常村-HLHF-1	0.005
07/19/2016 00:00:00	三门峡义马常村-HLHF-2	0.003
07/19/2016 00:00:00	三门峡义马常村-HLHF-3	0.012
暂时理解：对每个小区1都建立一个图
```

### 4 PRB信息统计与查询

```
首先创建tbPRBnew表
create table tbPRBnew (
	网元名称 nvarchar(50),
	日 integer,
	时 integer,
	[第0个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第1个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第2个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第3个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第4个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第5个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第6个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第7个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第8个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第9个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第10个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第11个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第12个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第13个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第14个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第15个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第16个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第17个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第18个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第19个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第20个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第21个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第22个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第23个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第24个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第25个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第26个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第27个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第28个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第29个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第30个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第31个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第32个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第33个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第34个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第35个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第36个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第37个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第38个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第39个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第40个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第41个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第42个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第43个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第44个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第45个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第46个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第47个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第48个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第49个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第50个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第51个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第52个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第53个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第54个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第55个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第56个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第57个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第58个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第59个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第60个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第61个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第62个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第63个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第64个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第65个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第66个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第67个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第68个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第69个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第70个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第71个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第72个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第73个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第74个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第75个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第76个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第77个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第78个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第79个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第80个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第81个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第82个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第83个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第84个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第85个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第86个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第87个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第88个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第89个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第90个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第91个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第92个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第93个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第94个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第95个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第96个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第97个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第98个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float,
    [第99个PRB上检测到的干扰噪声的平均值(毫瓦分贝)] float
)
```



```sql
(1) 某一网元某一小时内的PRB干扰=该小时内4个15分钟平均值的和/4，要求计算所有的网元从2016年7月17日到7月19日的每个小时的100个PRB上检测到的干扰噪声的平均值。
下属代码查询到的已经是所要求的平均值
创建存储过程计算
create procedure PRB_Hour_Pro
as 
	begin
		insert into tbPRBnew select 网元名称,DATEPART(dd,起始时间) as 日, DATEPART(hh,起始时间) as 时, avg([第0个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第0个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第2个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第2个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第3个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第3个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第4个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第4个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第5个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第5个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第6个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第6个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第7个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第7个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第8个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第8个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第9个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第9个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第10个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第10个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第11个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第11个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第12个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第12个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第13个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第13个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第14个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第14个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第15个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第15个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第16个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第16个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第17个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第17个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第18个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第18个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第19个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第19个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第20个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第20个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第21个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第21个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第22个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第22个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第23个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第23个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第24个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第24个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第25个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第25个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第26个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第26个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第27个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第27个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第28个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第28个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第29个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第29个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第30个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第30个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第31个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第31个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第32个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第32个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第33个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第33个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第34个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第34个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第35个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第35个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第36个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第36个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第37个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第37个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第38个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第38个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第39个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第39个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第40个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第40个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第41个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第41个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第42个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第42个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第43个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第43个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第44个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第44个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第45个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第45个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第46个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第46个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第47个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第47个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第48个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第48个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第49个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第49个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第50个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第50个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第51个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第51个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第52个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第52个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第53个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第53个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第54个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第54个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第55个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第55个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第56个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第56个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第57个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第57个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第58个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第58个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第59个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第59个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第61个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第61个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第62个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第62个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第63个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第63个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第64个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第64个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第65个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第65个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第66个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第66个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第67个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第67个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第68个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第68个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第69个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第69个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第70个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第70个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第71个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第71个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第72个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第72个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第73个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第73个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第74个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第74个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第75个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第75个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第76个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第76个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第77个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第77个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第78个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第78个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第79个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第79个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第80个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第80个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第81个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第81个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第82个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第82个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第83个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第83个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第84个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第84个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第85个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第85个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第86个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第86个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第87个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第87个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第88个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第88个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第89个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第89个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第90个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第90个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第91个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第91个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第92个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第92个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第93个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第93个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第94个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第94个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第95个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第95个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第96个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第96个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第97个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第97个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第98个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第98个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] ,avg([第99个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]) as [第99个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] from tbPRB group by 网元名称, DATEPART(dd,起始时间), DATEPART(hh,起始时间) order by 网元名称 asc, DATEPART(dd,起始时间) asc, DATEPART(hh,起始时间) asc
	end
执行存储过程 exec dbo.PRB_Hour_pro
数据示例：select * from tbPRbnew
网元名称	           日  时	  第0个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)	第1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)
三门峡义马汽车站-HLHF	17	0	-103.416666666667	-114.666666666667
三门峡义马汽车站-HLHF	17	1	-103.25	-115.333333333333
三门峡义马汽车站-HLHF	17	2	-104.916666666667	-116.083333333333
三门峡义马汽车站-HLHF	17	3	-105	-117.166666666667
三门峡义马汽车站-HLHF	17	4	-106.166666666667	-117
三门峡义马汽车站-HLHF	17	5	-105.166666666667	-117.25
三门峡义马汽车站-HLHF	17	6	-103.833333333333	-114.916666666667
三门峡义马汽车站-HLHF	17	7	-104.416666666667	-114.583333333333
三门峡义马汽车站-HLHF	17	8	-104.75	-113.666666666667
(2) 根据（1）中得到的小时级的PRB干扰数据表，通过人机界面对话框选定某个特定网元，查询数据库，列出其某一天从00:00:00到24：00：00每个小时内第60个PRB上检测到的干扰噪声的平均值，并将结果用柱状图或折线图的方式呈现出来。
select 网元名称, 日, 时, [第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] as [第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] from tbPRBnew where 网元名称 = $网元名称
得到的是7，8，9日三天的数据
数据示例：
网元名称	日	时	第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)
三门峡义马汽车站-HLHF	17	0	-116.416666666667
三门峡义马汽车站-HLHF	17	1	-116.583333333333
三门峡义马汽车站-HLHF	17	2	-116.833333333333
三门峡义马汽车站-HLHF	17	3	-116.75
三门峡义马汽车站-HLHF	17	4	-117
三门峡义马汽车站-HLHF	17	5	-117
三门峡义马汽车站-HLHF	17	6	-116.666666666667
```

## 3.3 主邻小区C2I干扰分析

```sql
单独执行创建tbC2INew
create table tbC2INew (
	ServingSector nvarchar(50),
	InterferingSector nvarchar(50),
	mean float,
	std float,
	Prb9 float,
	Prb6 float
)
导入标准正态分布概率表 NormStatistics.xlsx 为表 tbNormStatistics（这个表按照0.00，0.01，0.02的方式纵向排列）
```



```sql
(1) 主邻小区RSRP差值（即C2I）的均值C2I_mean、标准差std计算
根据导入的MRO测量报告数据表tbMROData表中主小区和邻小区的参考信号接收功率RSRP，计算主邻小区RSRP差值，该联系是多对多联系。
计算每对主邻小区<SCell, NCell>的RSRP差值C2I，并计算出C2I的均值和标准差。
创建存储过程
create procedure Calc_mean_std_pro
as 
	begin
		select ServingSector, InterferingSector, avg(LteScRSRP - LteNcRSRP) as mean, stdev(LteScRSRP - LteNcRSRP) as std into tbMROMean_Std from tbMROData group by ServingSector, InterferingSector
	end
调用
exec dbo.Calc_mean_std_pro
(2) 主邻小区RSRP差值小于9的概率PrbC2I9计算 
根据每对主邻小区RSRP差值的均值和标准差，可以得到正态分布，根据正态分布求出主邻小区RSRP差值小于9的概率Prb9=Pr{C2I<9db}。
(3) 主邻小区RSRP差值绝对值小于6的概率PrbABS6的计算 
    根据每对主邻小区RSRP差值的均值和标准差，可以得到正态分布，根据正态分布求出主邻小区RSRP差值绝对值小于6的概率（PrbABS6）。
创建存储过程
create procedure RSRP_Prb_pro
as 
	begin
		truncate table tbC2INew --冲表
		insert into tbC2INew select ServingSector, InterferingSector, mean, std, 0, 0 from tbMROMean_Std --默认概率为0
		update tbC2INew set Prb9 = (
				case
				when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std < 5 and (9 - tbC2INew.mean)/tbC2INew.std >= 0 then (select prb from tbNormStatistics where tbNormStatistics.id = round((9 - tbC2INew.mean)/tbC2INew.std, 2, 0) * 100)-- 0~4.99，查表
				when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std < 0 then 1 - (select prb from tbNormStatistics where tbNormStatistics.id = abs(round((9 - tbC2INew.mean)/tbC2INew.std, 2, 0)) * 100)-- <0，1 - 绝对值，查表
				when tbC2INew.Prb9 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (9 - tbC2INew.mean)/tbC2INew.std >= 5 then 1 -->5， 1
				end
			), Prb6 = (
				case
				when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std < 5 and (6 - tbC2INew.mean)/tbC2INew.std >= 0 then (select prb from tbNormStatistics where tbNormStatistics.id = round((6 - tbC2INew.mean)/tbC2INew.std, 2, 0) * 100)-- 0~4.99，查表
				when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std < 0 then 1 - (select prb from tbNormStatistics where tbNormStatistics.id = abs(round((6 - tbC2INew.mean)/tbC2INew.std, 2, 0)) * 100)-- <0，1 - 绝对值，查表
				when tbC2INew.Prb6 = 0 and tbC2INew.std is not null and tbC2INew.std != 0 and (6 - tbC2INew.mean)/tbC2INew.std >= 5 then 1 -->5， 1
				end
			)
		select * from tbC2INew where Prb9 is not null and Prb6 is not null --由于std有null和0，这些行不能算出正态分布
	end
调用
exec dbo.RSRP_Prb_pro
```

## 3.4查询重叠覆盖干扰三元组

```sql
提前创建好tbC2I3
create table tbC2I3 (
	a varchar(50),
	b varchar(50),
	c varchar (50),
	PRIMARY KEY (a,b,c)
)
提前创建一个函数用于获得两个邻区之间的Prb6（drop function PrbGE70删除这个函数）
create function Prb6GE70(@x varchar(50), @y varchar(50))
returns float
as
	begin
	declare @rslt float
	set @rslt = 0
	select @rslt = Prb6 from tbC2INew where ServingSector = @x and InterferingSector = @y and Prb6 >= 0.7
	return @rslt
	end
```

```sql
根据表tbC2Inew，找出所有的小区三元组<a,b,c>（其中a,b,c互为邻小区），生成新表tbC2I3，其中有三个属性，分别是三个小区的小区ID。
with sa (ServingSector, InterferingSector) as (
	select ServingSector, InterferingSector from tbC2INew
	), --a的小区
 sb (ServingSector, InterferingSector) as (
	select ServingSector, InterferingSector from tbC2INew
	), --b的小区
 sc (ServingSector, InterferingSector) as (
	select ServingSector, InterferingSector from tbC2INew
	) --c的小区
insert into tbC2I3 select sa.ServingSector as a, sb.ServingSector as b, sc.ServingSector as c from  sa, sb, sc where sa.ServingSector != sb.ServingSector and sb.ServingSector!=sc.ServingSector and sc.ServingSector != sa.ServingSector --不能两两成环
	and  sa.InterferingSector=sb.ServingSector and sb.InterferingSector=sc.ServingSector and sc.InterferingSector=sa.ServingSector --三个成环
	and ((select dbo.Prb6GE70(sa.ServingSector, sb.ServingSector)) != 0 or (select dbo.Prb6GE70(sb.ServingSector, sa.ServingSector)) != 0 ) --概率条件(1)
	and ((select dbo.Prb6GE70(sa.ServingSector, sc.ServingSector)) != 0 or (select dbo.Prb6GE70(sc.ServingSector, sa.ServingSector)) != 0 ) --概率条件(2)
	and ((select dbo.Prb6GE70(sc.ServingSector, sb.ServingSector)) != 0 or (select dbo.Prb6GE70(sb.ServingSector, sc.ServingSector)) != 0 ) --概率条件(3)
select * from tbC2I3
```

## 触发器

### tbCell

```sql
create trigger tbCell_trigger on tbCell for insert
as
	declare @sid nvarchar(255), @city nvarchar(255), @sname nvarchar(255), @eid float, @ename nvarchar(255), @ear int, @pci int, @pss int, @sss int,
	 @tac int, @vendor nvarchar(255), @long float, @lati float, @sty nvarchar(255), @azi float, @hei float, @ele float, @mec float, @tot float;
	select @sid=SECTOR_ID, @city=CITY, @sname=SECTOR_NAME, @eid= ENODEBID, @ename=ENODEB_NAME, @ear=EARFCN, @pci=PCI, @pss=PSS, @sss=SSS, @tac=TAC, 
	@vendor=VENDOR, @long=LONGITUDE, @lati=LATITUDE, @sty=STYLE, @azi=AZIMUTH, @hei=HEIGHT, @ele=ELECTTILT, @mec=MECHTILT, @tot=TOTLETILT from inserted;
	if (exists(select SECTOR_ID from tbCell where SECTOR_ID = @sid))
		update tbCell set CITY = @city, SECTOR_NAME=@sname, ENODEBID=@eid, ENODEB_NAME=@ename, 
			EARFCN=@ear, PCI=@pci, PSS=@pss, SSS=@sss, TAC=@tac, VENDOR=@vendor,
			LONGITUDE=@long, LATITUDE=@lati, STYLE=@sty, AZIMUTH=@azi, HEIGHT=@hei,
			ELECTTILT=@ele, MECHTILT=@mec, TOTLETILT=@tot;
	else
		insert into tbCell select * from inserted;
```

### tbKPI

```sql
create trigger tbKPI_trigger on tbKPI for insert
as
	declare @begin nvarchar(255), @circuit float, @metaname nvarchar(255), @nbr nvarchar(255), @nbr1 nvarchar(255), @r1 float, @r2 float,
			@r3 float, @e1 float, @e2 float, @e3 float, @e4 float, @nbrtimes float, @e5 float, @wireless float, @e6 float, @u1 float, @u2 float,
			@wirefail float, @e7 float, @e8 float, @e9 float, @e10 float, @e11 float, @e12 float, @e13 float, @e14 float, @e15 nvarchar(255), @e16 float, @zsp1 float, @zsp2 nvarchar(255),
			@suc float, @p1 float, @p2 float, @r4 float, @r5 float, @tg1 float, @tg2 float, @tg3 float, @tg4 float, @last1 float, @last2 float;
	select @begin=起始时间, @circuit=周期, @metaname=网元名称, @nbr=小区, @nbr1=小区1, @r1=[RRC连接建立完成次数 (无)], @r2=[RRC连接请求次数（包括重发） (无)], @r3=[RRC建立成功率qf (%)], @e1=[E-RAB建立成功总次数 (无)], @e2=[E-RAB建立尝试总次数 (无)], @e3=[E-RAB建立成功率2 (%)], @e4=[eNodeB触发的E-RAB异常释放总次数 (无)], @nbrtimes=[小区切换出E-RAB异常释放总次数 (无)], @e5=[E-RAB掉线率(新) (%)], @wireless=[无线接通率ay (%)], @e6=[eNodeB发起的S1 RESET导致的UE Context释放次数 (无)],
			@u1=[UE Context异常释放次数 (无)], @u2=[UE Context建立成功总次数 (无)], @wirefail=[无线掉线率 (%)], @e7=[eNodeB内异频切换出成功次数 (无)], @e8=[eNodeB内异频切换出尝试次数 (无)], @e9=[eNodeB内同频切换出成功次数 (无)], @e10=[eNodeB内同频切换出尝试次数 (无)], @e11=[eNodeB间异频切换出成功次数 (无)], @e12=[eNodeB间异频切换出尝试次数 (无)], @e13=[eNodeB间同频切换出成功次数 (无)], @e14=[eNodeB间同频切换出尝试次数 (无)], @e15=[eNB内切换成功率 (%)], @e16=[eNB间切换成功率 (%)], @zsp1=[同频切换成功率zsp (%)], @zsp2=[异频切换成功率zsp (%)], @suc=[切换成功率 (%)], @p1=[小区PDCP层所接收到的上行数据的总吞吐量 (比特)], @p2=[小区PDCP层所发送的下行数据的总吞吐量 (比特)], @r4=[RRC重建请求次数 (无)], @r5=[RRC连接重建比率 (%)], 
			@tg1=[通过重建回源小区的eNodeB间同频切换出执行成功次数 (无)], @tg2=[通过重建回源小区的eNodeB间异频切换出执行成功次数 (无)], @tg3=[通过重建回源小区的eNodeB内同频切换出执行成功次数 (无)], @tg4=[通过重建回源小区的eNodeB内异频切换出执行成功次数 (无)], @last1=[eNB内切换出成功次数 (次)], @last2=[eNB内切换出请求次数 (次)] from inserted;
	if (exists(select 小区 from tbKPI where 小区 = @nbr))
		update tbKPI set 起始时间=@begin, 周期=@circuit, 网元名称=@metaname, 小区=@nbr, 小区1=@nbr1, [RRC连接建立完成次数 (无)]=@r1, [RRC连接请求次数（包括重发） (无)]=@r2, [RRC建立成功率qf (%)]=@r3, [E-RAB建立成功总次数 (无)]=@e1, [E-RAB建立尝试总次数 (无)]=@e2, [E-RAB建立成功率2 (%)]=@e3, [eNodeB触发的E-RAB异常释放总次数 (无)]=@e4, [小区切换出E-RAB异常释放总次数 (无)]=@nbrtimes, [E-RAB掉线率(新) (%)]=@e5, [无线接通率ay (%)]=@wireless, [eNodeB发起的S1 RESET导致的UE Context释放次数 (无)]=@e6,
			[UE Context异常释放次数 (无)]=@u1, [UE Context建立成功总次数 (无)]=@u2, [无线掉线率 (%)]=@wirefail, [eNodeB内异频切换出成功次数 (无)]=@e7, [eNodeB内异频切换出尝试次数 (无)]=@e8, [eNodeB内同频切换出成功次数 (无)]=@e9, [eNodeB内同频切换出尝试次数 (无)]=@e10, [eNodeB间异频切换出成功次数 (无)]=@e11, [eNodeB间异频切换出尝试次数 (无)]=@e12, [eNodeB间同频切换出成功次数 (无)]=@e13, [eNodeB间同频切换出尝试次数 (无)]=@e14, [eNB内切换成功率 (%)]=@e15, [eNB间切换成功率 (%)]=@e16, [同频切换成功率zsp (%)]=@zsp1, [异频切换成功率zsp (%)]=@zsp2, [切换成功率 (%)]=@suc, [小区PDCP层所接收到的上行数据的总吞吐量 (比特)]=@p1, [小区PDCP层所发送的下行数据的总吞吐量 (比特)]=@p2, [RRC重建请求次数 (无)]=@r4, [RRC连接重建比率 (%)]=@r5, 
			[通过重建回源小区的eNodeB间同频切换出执行成功次数 (无)]=@tg1, [通过重建回源小区的eNodeB间异频切换出执行成功次数 (无)]=@tg2, [通过重建回源小区的eNodeB内同频切换出执行成功次数 (无)]=@tg3, [通过重建回源小区的eNodeB内异频切换出执行成功次数 (无)]=@tg4, [eNB内切换出成功次数 (次)]=@last1, [eNB内切换出请求次数 (次)]=@last2;
	else
		insert into tbKPI select * from inserted;
```

### tbPRB

```sql
create trigger tbPRB_trigger on tbPRB for insert
as
	declare @begin nvarchar(255), @circuit float, @metaname nvarchar(255), @nbr nvarchar(255), @nbrname nvarchar(255), @prb0 float, @prb1 float, @prb2 float, @prb3 float, @prb4 float, @prb5 float, @prb6 float, @prb7 float, @prb8 float, @prb9 float, @prb10 float, @prb11 float, @prb12 float, @prb13 float, @prb14 float, @prb15 float, @prb16 float, @prb17 float, @prb18 float, @prb19 float, @prb20 float, @prb21 float, @prb22 float, @prb23 float, @prb24 float, @prb25 float, @prb26 float, @prb27 float, @prb28 float, @prb29 float, @prb30 float, @prb31 float, @prb32 float, @prb33 float, @prb34 float, @prb35 float, @prb36 float, @prb37 float, @prb38 float, @prb39 float, @prb40 float, @prb41 float, @prb42 float, @prb43 float, @prb44 float, @prb45 float, @prb46 float, @prb47 float, @prb48 float, @prb49 float, @prb50 float, @prb51 float, @prb52 float, @prb53 float, @prb54 float, @prb55 float, @prb56 float, @prb57 float, @prb58 float, @prb59 float, @prb60 float, @prb61 float, @prb62 float, @prb63 float, @prb64 float, @prb65 float, @prb66 float, @prb67 float, @prb68 float, @prb69 float, @prb70 float, @prb71 float, @prb72 float, @prb73 float, @prb74 float, @prb75 float, @prb76 float, @prb77 float, @prb78 float, @prb79 float, @prb80 float, @prb81 float, @prb82 float, @prb83 float, @prb84 float, @prb85 float, @prb86 float, @prb87 float, @prb88 float, @prb89 float, @prb90 float, @prb91 float, @prb92 float, @prb93 float, @prb94 float, @prb95 float, @prb96 float, @prb97 float, @prb98 float, @prb99 float;	
	select 
		@begin = 起始时间,@circuit=周期,@metaname=网元名称,@nbr=小区,@nbrname=小区名,@prb0=[第0个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb1=[第1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb2=[第2个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb3=[第3个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb4=[第4个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb5=[第5个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb6=[第6个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb7=[第7个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb8=[第8个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb9=[第9个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb10=[第10个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb11=[第11个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb12=[第12个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb13=[第13个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb14=[第14个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb15=[第15个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb16=[第16个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb17=[第17个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb18=[第18个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb19=[第19个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb20=[第20个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb21=[第21个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb22=[第22个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb23=[第23个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb24=[第24个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb25=[第25个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb26=[第26个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb27=[第27个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb28=[第28个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb29=[第29个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb30=[第30个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb31=[第31个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb32=[第32个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb33=[第33个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb34=[第34个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb35=[第35个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb36=[第36个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb37=[第37个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb38=[第38个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb39=[第39个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb40=[第40个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb41=[第41个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb42=[第42个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb43=[第43个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb44=[第44个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb45=[第45个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb46=[第46个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb47=[第47个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb48=[第48个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb49=[第49个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb50=[第50个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb51=[第51个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb52=[第52个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb53=[第53个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb54=[第54个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb55=[第55个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb56=[第56个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb57=[第57个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb58=[第58个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb59=[第59个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb60=[第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb61=[第61个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb62=[第62个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb63=[第63个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb64=[第64个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb65=[第65个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb66=[第66个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb67=[第67个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb68=[第68个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb69=[第69个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb70=[第70个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb71=[第71个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb72=[第72个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb73=[第73个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb74=[第74个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb75=[第75个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb76=[第76个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb77=[第77个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb78=[第78个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb79=[第79个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb80=[第80个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb81=[第81个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb82=[第82个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb83=[第83个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb84=[第84个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb85=[第85个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb86=[第86个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb87=[第87个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb88=[第88个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb89=[第89个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb90=[第90个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb91=[第91个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb92=[第92个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb93=[第93个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb94=[第94个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb95=[第95个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb96=[第96个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb97=[第97个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb98=[第98个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)], @prb99=[第99个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)] from inserted;
	if (exists(select 小区 from tbPRB where 小区 = @nbr))
		update tbPRB set 起始时间=@begin, 周期=@circuit, 网元名称=@metaname, 小区=@nbr, 小区名=@nbrname,  [第0个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb0, [第1个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb1, [第2个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb2, [第3个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb3, [第4个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb4, [第5个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb5, [第6个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb6, [第7个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb7, [第8个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb8, [第9个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb9, [第10个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb10, [第11个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb11, [第12个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb12, [第13个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb13, [第14个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb14, [第15个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb15, [第16个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb16, [第17个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb17, [第18个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb18, [第19个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb19, [第20个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb20, [第21个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb21, [第22个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb22, [第23个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb23, [第24个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb24, [第25个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb25, [第26个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb26, [第27个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb27, [第28个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb28, [第29个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb29, [第30个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb30, [第31个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb31, [第32个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb32, [第33个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb33, [第34个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb34, [第35个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb35, [第36个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb36, [第37个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb37, [第38个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb38, [第39个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb39, [第40个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb40, [第41个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb41, [第42个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb42, [第43个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb43, [第44个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb44, [第45个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb45, [第46个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb46, [第47个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb47, [第48个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb48, [第49个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb49, [第50个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb50, [第51个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb51, [第52个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb52, [第53个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb53, [第54个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb54, [第55个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb55, [第56个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb56, [第57个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb57, [第58个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb58, [第59个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb59, [第60个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb60, [第61个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb61, [第62个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb62, [第63个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb63, [第64个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb64, [第65个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb65, [第66个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb66, [第67个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb67, [第68个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb68, [第69个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb69, [第70个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb70, [第71个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb71, [第72个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb72, [第73个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb73, [第74个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb74, [第75个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb75, [第76个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb76, [第77个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb77, [第78个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb78, [第79个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb79, [第80个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb80, [第81个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb81, [第82个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb82, [第83个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb83, [第84个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb84, [第85个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb85, [第86个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb86, [第87个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb87, [第88个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb88, [第89个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb89, [第90个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb90, [第91个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb91, [第92个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb92, [第93个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb93, [第94个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb94, [第95个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb95, [第96个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb96, [第97个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb97, [第98个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb98, [第99个PRB上检测到的干扰噪声的平均值 (毫瓦分贝)]=@prb99;
	
	else
		insert into tbPRB select * from inserted;
```

### tbMROData

```sql
create trigger tbMROData_trigger on tbMROData for insert
as
	declare @ts nvarchar(30), @ss nvarchar(50), @is nvarchar(50), @lsr float, @lnr float, @lne int, @lnp smallint;
	select @ts=[TimeStamp], @ss= ServingSector, @is=InterferingSector, @lsr=LteScRSRP, @lnr=LteNcRSRP, @lne=LteNcEarfcn, @lnp=LteNcPci from inserted;
	if (exists(select [TimeStamp], ServingSector, InterferingSector from tbMROData where [TimeStamp] = @ts and ServingSector=@ss and InterferingSector=@is))
		update tbMROData set LteScRSRP=@lsr, LteNcRSRP=@lnr, LteNcEarfcn=@lne, LteNcPci=@lnp;
	else
		insert into tbMROData select * from inserted;
```

## 索引补充

```sql
tbKPI表：
create nonclustered index meta_name_index on tbKPI(网元名称)
tbPRB表：
create nonclustered index meta_name_index on tbPRB(起始时间)
tbPRBnew
create nonclustered index meta_name_index on tbPRBnew(网元名称)
create nonclustered index date_index on tbPRBnew(日)
create nonclustered index hour_index on tbPRBnew(时)
tbMROData
create clustered index SI_index on tbMROData(ServingSector, InterferingSector)
tbC2INew
create clustered index SI_index_new on tbC2INew(ServingSector, InterferingSector)
create nonclustered index prb9_index on tbC2INew(Prb9)
create nonclustered index prb6_index on tbC2INew(Prb6)
```

## 物理设计

```sql
创建数据库时指定主文件，文件组，辅文件，日志文件
create database LTE_DB
on primary //主文件
(
	name = 'LTE_DB_Primary',
	filename = 'D:\lte_db.mdf',
	size = 5mb,
	maxsize = 10mb,
	filegrowth = 1mb),
filegroup Group_FG1 //文件组1，包括两个文件
	(	name = 'LTE_FG1_Data1',
		filename = 'D:\lte_fg1_data1.ndf',
		size = 1mb,
		maxsize = 10mb,
		filegrowth = 1mb),
	(	name = 'LTE_FG1_Data2',
		filename = 'D:\lte_fg1_data2.ndf',
		size = 1mb,
		maxsize = 10mb,
		filegrowth = 1mb)
log on //日志文件
(
	name = 'LTE_log',
	filename = 'D:\lte_log.ldf',
	size = 1mb,
	maxsize = 10mb,
	filegrowth = 1mb
);
修改文件组
alter database LTE_DB
modify filegroup LTE_FG1 default
```



