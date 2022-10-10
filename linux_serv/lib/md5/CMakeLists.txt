message(-> md5库)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. TMD5_SRC_LIST)
#设置成静态库
ADD_LIBRARY(md5 ${TMD5_SRC_LIST})