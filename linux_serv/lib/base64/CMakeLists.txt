message(-> base64库)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. BASE64_T_SRC_LIST)
#设置成静态库
ADD_LIBRARY(base64 ${BASE64_T_SRC_LIST})