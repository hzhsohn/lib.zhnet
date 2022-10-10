message(-> hxnet)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. HXNET_T_SRC_LIST)
#设置成静态库
ADD_LIBRARY(hxnet ${HXNET_T_SRC_LIST})
