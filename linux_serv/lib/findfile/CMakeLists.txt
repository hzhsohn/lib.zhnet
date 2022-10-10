message(-> findfile库)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. FINDFILE_SRC_LIST)
#设置成静态库
ADD_LIBRARY(findfile ${FINDFILE_SRC_LIST})