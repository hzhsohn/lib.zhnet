@set path="C:\Program Files\Microsoft Visual Studio 11.0\Common7\IDE\";%path%
@set path="D:\Microsoft Visual Studio 11.0\Common7\IDE\";%path%
@cls


rem "net----------------------------------------------------"

devenv "test\Net\TestGCSRTNetClient\TestGCSRTNetClient.vcxproj" /build debug
devenv "test\Net\TestGCSRTNetServer\TestGCSRTNetServer.vcxproj" /build debug

rem "packet---------------------------------------------------"

devenv "test\Packet\testGCSetPacket\CSetPacket\CSetPacket.vcxproj" /build debug

