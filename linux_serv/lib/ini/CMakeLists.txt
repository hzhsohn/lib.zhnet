message(-> ini)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. ZHINI_T_SRC_LIST)
#设置成静态库
ADD_LIBRARY(zhini ${ZHINI_T_SRC_LIST})
