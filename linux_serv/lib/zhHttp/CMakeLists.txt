message(-> zhHttp库)

#所有的路径变量都不允许重名
AUX_SOURCE_DIRECTORY(. ZHTTP_SRC_LIST)
#设置成静态库
ADD_LIBRARY(zhHttp ${ZHTTP_SRC_LIST})