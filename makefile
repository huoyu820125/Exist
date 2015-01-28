#makefile文件编译指令
#如果make文件名是makefile，直接使用make就可以编译
#如果make文件名不是makefile，比如test.txt，那么使用make -f test.txt

include dependence

#————————————编译子模块————————————#
#方法一：(make -C subdir) 
#方法二：(cd subdir && make)
#加-w 可以查看执行指令时，当前工作目录信息

#————————————编译Exist————————————#
Output_lib_dir=./lib
Output_bin_dir=./bin
$(shell mkdir $(Output_lib_dir))
$(shell mkdir $(Output_bin_dir))

MDK_dir=$(MDK_ROOT)
Mother_Dir=./Mother
SSD_dir=./SolidStateDrive
Exist_dir=./Exist
CPU_dir=./CPU
Client_dir=./Client

all: 
	@echo "Create bin dir"
	tar -zxvpf bin.tar.gz

	@echo "Complie mdk"
	(make -C $(MDK_dir)/mdk_static -w)
	cp $(MDK_dir)/lib/mdk.a $(Output_lib_dir)/mdk.a
	@echo ""
	@echo "mdk Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie Mother"
	(make -C $(Mother_Dir) -w)
	@echo ""
	@echo "Mother Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie SolidStateDrive"
	(make -C $(SSD_dir) -w)
	@echo ""
	@echo "SolidStateDrive Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie Exist"
	(make -C $(Exist_dir) -w)
	@echo ""
	@echo "Exist Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie CPU"
	(make -C $(CPU_dir) -w)
	@echo ""
	@echo "CPU Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""
	@echo "Complie Client"
	(make -C $(Client_dir) -w)
	@echo ""
	@echo "Client Complie finished"
	@echo ""
	@echo ""
	@echo ""
	@echo ""


clean:
	rm $(Output_lib_dir)/*.a
	(make -C $(MDK_dir)/mdk_static -w clean)
	(make -C $(Mother_Dir) -w clean)
	(make -C $(SSD_dir) -w clean)
	(make -C $(Exist_dir) -w clean)
	(make -C $(Client_dir) -w clean)
