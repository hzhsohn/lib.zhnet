message(-> sha1库)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. TSHA1_SRC_LIST)
#设置成静态库
ADD_LIBRARY(sha1 ${TSHA1_SRC_LIST})