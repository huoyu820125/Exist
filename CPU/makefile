#makefile文件编译指令
#如果make文件名是makefile，直接使用make就可以编译
#如果make文件名不是makefile，比如test.txt，那么使用make -f test.txt

include ../dependence

#------------------------------------------检查操作系统32位64位--------------------------------------------------------
#SYS_BIT=$(shell getconf LONG_BIT)
#SYS_BIT=$(shell getconf WORD_BIT)
SYS_BIT=$(shell getconf LONG_BIT)
ifeq ($(SYS_BIT),32)
	CPU =  -march=i686 
else 
	CPU = 
endif

#------------------------------------------编辑器--------------------------------------------------------

#c++编译工具
CC = g++ 
#apache模块编译安装工具
APXS = apxs 

#------------------------------------------编辑器End--------------------------------------------------------

#------------------------------------------目录--------------------------------------------------------
#常用目录
#定义一些常用目录的宏，后面可以使用
#※末尾不能有空格
#范例：
#ROOT_DIR=../../../..
Root_dir=..
Mdk_dir=../$(MDK_ROOT)

#依赖文件全局查找目录
VPATH = $(OBJ_OUTPUT_DIR) $(SRC_DIR) 
SRC_DIR = 

#源文件目录
#每个目录定义一个宏，全部添加到SRC_DIR中
#※末尾不能有空格
#范例：
#Main_Dir=$(ROOT_DIR)/main
Frame_dir=$(Root_dir)/CPU/source/frame
User_dir=$(Root_dir)/CPU/source
Common_dir=$(Root_dir)/common

#输出目录
OBJ_OUTPUT_DIR=./output
OUTPUT_DIR=../lib
$(shell mkdir $(OBJ_OUTPUT_DIR))
$(shell mkdir $(OUTPUT_DIR))



#------------------------------------------目录End--------------------------------------------------------

#------------------------------------------编译选项--------------------------------------------------------

#编译选项
CFLAGS= -O -g -fPIC -Wall -D_REENTRANT -DNO_STRING_CIPHER $(CPU) -DSSD_DEVICE 

#警告级别
WARNING_LEVEL += -O3 

#头文件目录：-I 目录
INCLUDE = -I. -I../include 
#按照以下格式添加
#INCLUDE += -I目录+空格 
INCLUDE += -I$(Mdk_dir)/include 


#库目录，库文件:-L 目录 -库名
#LIB = -lnsl -lc -lm -lpthread -lstdc++ -lrt 
LIB = -lnsl -lc -lm -lpthread -lstdc++ -lrt 

#静态库：.a文件路径名
#按照以下格式添加
#LIB += .a文件路径名+空格

#------------------------------------------编译选项End--------------------------------------------------------


#------------------------------------------输出--------------------------------------------------------
#编译产生的程序文件名
OUTPUT = cpu.a

#目标文件
#范例
#目标文件列表 = $(notdir $(patsubst %.cpp,%.o,$(wildcard [cpp源文件目录]/*.cpp))) 
#cpp分布在多少个目录中,就定义多少个[目标文件列表]
#范例：
#OBJ列表1 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/mdk/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
#OBJ列表2 = $(notdir $(patsubst %.cpp,%.o,$(wildcard mdk目录/source/frame/netserver/*.cpp))) 
Frame_obj = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Frame_dir)/*.cpp))) 
User_obj = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(User_dir)/*.cpp))) 
Common_obj = $(notdir $(patsubst %.cpp,%.o,$(wildcard $(Common_dir)/*.cpp))) 


#依赖的目标文件
DEPENDENCE = 
#将目标文件中定义的列表宏全部添加到DEPENDENCE中，每个项目都并以空格结束
#范例：
#DEPENDENCE += $(OBJ列表1) 
DEPENDENCE += $(Frame_obj) 
DEPENDENCE += $(User_obj) 
DEPENDENCE += common.o MD5Helper.o Protocol.o 


#连接项目，需要的所有其它源文件或目标文件(带目录)
OBJ = 
#将目标文件中定义的列表宏，按照如下格式的指令，全部添加到OBJ中，每个项目都以空格结束
#指令：$(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表)) 
#范例：
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表1)) 
#OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(OBJ列表2)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(Frame_obj)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, $(User_obj)) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, common.o) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, MD5Helper.o) 
OBJ += $(addprefix $(OBJ_OUTPUT_DIR)/, Protocol.o) 

 
#------------------------------------------输出End--------------------------------------------------------

#-------------------------------------------编译指令-----------------------------------------------------
#生成EXE
#范例：
#$(OUTPUT_DIR)/$(OUTPUT): $(DEPENDENCE)										依赖关系
#	@echo "Complie (OUTPUT_DIR)/$(OUTPUT)"
#	$(CC) -o $@ $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)		gcc编译指令
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""


#-----------------------------------------编译生成.A静态库---------------------------------------------------
#$(OUTPUT_DIR)/$(OUTPUT):$(DEPENDENCE)
#	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
#	@echo ""
#	ar -r $@ $(OBJ)
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

$(OUTPUT_DIR)/$(OUTPUT):$(DEPENDENCE)
	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
	@echo ""
	ar -r $@ $(OBJ)  
	@echo ""
	cp $(Mdk_dir)/lib/mdk.a $(OUTPUT_DIR)/mdk.a
	@echo ""
	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

#-----------------------------------------编译生成SO动态库---------------------------------------------------
#范例：
#$(OUTPUT_DIR)/$(OUTPUT): $(DEPENDENCE)											依赖关系
#	@echo "Complie $(OUTPUT_DIR)/$(OUTPUT)"
#	$(CC) -o $@ -shared $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(OBJ)$(LIB)		gcc编译指令
#	@echo ""
#	@echo "$(OUTPUT_DIR)/$(OUTPUT) complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""


#------------------------------------------编译Object----------------------------------------------------
#范例：单个cpp编译为object编译
#$(OBJ_OUTPUT_DIR)/GameSerFrm.o: main/GameSerFrm.cpp main/GameSerFrm.h main/GameSerCPU.h com/ComDef.h com/XXSocket.h tool/DBTool.h	依赖关系
#	@echo "Complie GameSerFrm.o"
#	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)main/GameSerFrm.cpp											gcc编译指令
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/GameSerFrm.o complie finished"
#	@echo ""
#	@echo ""

#范例：目录批量编译，所有cpp目录下的cpp生成object
#$(OBJ列表1):%.o:[OBJ列表1的cpp源文件目录]/%.cpp [OBJ列表1的cpp源文件目录]/%.h
#	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
#	@echo ""
#	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) [OBJ列表1的cpp源文件目录]/$*.cpp
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

#$(OBJ列表2):%.o:[OBJ列表2的cpp源文件目录]/%.cpp [OBJ列表2的cpp源文件目录]/%.h
#	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
#	@echo ""
#	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE) [OBJ列表2的cpp源文件目录]/$*.cpp
#	@echo ""
#	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
#	@echo ""
#	@echo ""
#	@echo ""
#	@echo ""

$(Frame_obj):%.o:$(Frame_dir)/%.cpp $(Root_dir)/include/Exist/frame/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(Frame_dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

$(User_obj):%.o:$(User_dir)/%.cpp $(Root_dir)/include/Exist/%.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(User_dir)/$*.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	
common.o:$(Common_dir)/common.cpp $(Common_dir)/common.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(Common_dir)/common.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

MD5Helper.o:$(Common_dir)/MD5Helper.cpp $(Common_dir)/MD5Helper.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(Common_dir)/MD5Helper.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""

Protocol.o:$(Common_dir)/Protocol.cpp $(Common_dir)/Protocol.h
	@echo "Complie $(OBJ_OUTPUT_DIR)/$*.o"
	@echo ""
	$(CC) -c -o $(OBJ_OUTPUT_DIR)/$*.o $(CFLAGS)$(WARNING_LEVEL)$(INCLUDE)$(Common_dir)/Protocol.cpp
	@echo ""
	@echo "$(OBJ_OUTPUT_DIR)/$*.o complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""


install:
#------------------------------------------安装----------------------------------------------------
#------------------------------------------安装apache模块----------------------------------------------------
#范例：apxs -a -i -n 模块名 so文件名
#apxs -a -i -n test $(OBJ_OUTPUT_DIR)/$(OUTPUT)
	apxs -a -i -n pfadurl $(OBJ_OUTPUT_DIR)/$(OUTPUT)





#------------------------------------------清理重新编译----------------------------------------------------
clean:
	-rm -f $(OUTPUT_DIR)/$(OUTPUT) $(OUTPUT_DIR)/SolidStateDrive.cfg $(OBJ_OUTPUT_DIR)/*.o

	
.PHONY: clean


#------------------------------------------apache控制----------------------------------------------------

start:
	$(APACHE_BIN)/apachectl start

stop:
	$(APACHE_BIN)/apachectl stop

ps:
	ps -ae|grep http
