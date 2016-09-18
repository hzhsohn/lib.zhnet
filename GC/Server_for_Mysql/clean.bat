rem "清除SDK部分"

del "server\*.ncb"
del "server\server\*.ncb"
del "server\server\*.suo" /a
del "server\*.sdf" /a
del "server\server\*.user"
rd "server\server\debug" /s/q
rd "server\server\Release" /s/q

del "iocpClient\*.ncb"
del "iocpClient\*.ncb"
del "iocpClient\*.suo" /a
del "iocpClient\*.sdf" /a
del "iocpClient\*.user"
rd "iocpClient\debug" /s/q
rd "iocpClient\Release" /s/q

del "c_client\*.ncb"
del "c_client\*.ncb"
del "c_client\*.suo" /a
del "c_client\*.sdf" /a
del "c_client\*.user"
rd "c_client\debug" /s/q
rd "c_client\Release" /s/q

del "lib\zh-tcp\*.o"

rem "清理已编译项"

del "bin\debug\*.exp"
del "bin\debug\*.ilk"
del "bin\debug\*.pdb"

del "bin\release\*.exp"
del "bin\release\*.map"
del "bin\release\*.pdb"
