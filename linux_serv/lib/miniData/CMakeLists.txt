message(-> miniData)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. MINIDATA_T_SRC_LIST)
#设置成静态库
ADD_LIBRARY(miniData ${MINIDATA_T_SRC_LIST})