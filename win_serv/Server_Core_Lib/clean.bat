rem "清除SDK部分"

del "sdk\GCHEncrypt\*.ncb"
del "sdk\GCHEncrypt\*.suo" /a
del "sdk\GCHEncrypt\*.sdf" /a
del "sdk\GCHEncrypt\*.user"
rd "sdk\GCHEncrypt\debug" /s/q
rd "sdk\GCHEncrypt\Release" /s/q

del "sdk\GCHLog\*.ncb"
del "sdk\GCHLog\*.suo" /a
del "sdk\GCHLog\*.sdf" /a
del "sdk\GCHLog\*.user"
rd "sdk\GCHLog\debug" /s/q
rd "sdk\GCHLog\Release" /s/q

del "sdk\GCSetPack\*.ncb"
del "sdk\GCSetPack\*.suo" /a
del "sdk\GCSetPack\*.sdf" /a
del "sdk\GCSetPack\*.user"
rd "sdk\GCSetPack\debug" /s/q
rd "sdk\GCSetPack\Release" /s/q

del "sdk\GCSRTNet\*.ncb"
del "sdk\GCSRTNet\*.suo" /a
del "sdk\GCSRTNet\*.sdf" /a
del "sdk\GCSRTNet\*.user"
rd "sdk\GCSRTNet\debug" /s/q
rd "sdk\GCSRTNet\Release" /s/q
rd "sdk\GCSRTNet\ipch" /s/q

del "sdk\GCHttp\*.ncb"
del "sdk\GCHttp\*.suo" /a
del "sdk\GCHttp\*.sdf" /a
del "sdk\GCHttp\*.user"
rd "sdk\GCHttp\debug" /s/q
rd "sdk\GCHttp\Release" /s/q

rem "清除例子"
del "test\Net\TestGCSRTNetClient\*.ncb"
del "test\Net\TestGCSRTNetClient\*.suo" /a
del "test\Net\TestGCSRTNetClient\*.sdf" /a
del "test\Net\TestGCSRTNetClient\*.user"
rd "test\Net\TestGCSRTNetClient\debug" /s/q
rd "test\Net\TestGCSRTNetClient\Release" /s/q
rd "test\Net\TestGCSRTNetClient\ipch" /s/q

del "test\Net\TestGCSRTNetServer\*.ncb"
del "test\Net\TestGCSRTNetServer\*.suo" /a
del "test\Net\TestGCSRTNetServer\*.sdf" /a
del "test\Net\TestGCSRTNetServer\*.user"
rd "test\Net\TestGCSRTNetServer\debug" /s/q
rd "test\Net\TestGCSRTNetServer\Release" /s/q
rd "test\Net\TestGCSRTNetServer\ipch" /s/q

del "test\Packet\testGCSetPacket\*.ncb"
del "test\Packet\testGCSetPacket\*.suo" /a
del "test\Packet\testGCSetPacket\*.sdf" /a
del "test\Packet\testGCSetPacket\CSetPacket\*.user"
rd "test\Packet\testGCSetPacket\CSetPacket\debug" /s/q
rd "test\Packet\testGCSetPacket\CSetPacket\Release" /s/q

del "test\Http\*.ncb"
del "test\Http\*.aps"
del "test\Http\*.suo" /a
del "test\Http\*.sdf" /a
del "test\Http\*.user"
rd "test\Http\debug" /s/q
rd "test\Http\Release" /s/q

rem "清理已编译项"

del "bin\debug\*.exp"
del "bin\debug\*.ilk"
del "bin\debug\*.pdb"

del "bin\release\*.exp"
del "bin\release\*.map"
del "bin\release\*.pdb"
del "bin\release\*.ilk"

rem cd ZhCTcp
rem start clean.bat