message(-> handy)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. THANDY_SRC_LIST)
#设置成静态库
ADD_LIBRARY(handy ${THANDY_SRC_LIST})