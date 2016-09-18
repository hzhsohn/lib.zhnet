@set path="C:\Program Files\Microsoft Visual Studio 11.0\Common7\IDE\";%path%
@set path="D:\Microsoft Visual Studio 11.0\Common7\IDE\";%path%
@cls

devenv "sdk\GCHEncrypt\GCHEncrypt.vcxproj" /build debug
devenv "sdk\GCHLog\GCHLog.vcxproj" /build debug
devenv "sdk\GCSetPack\GCSetPacket.vcxproj" /build debug
devenv "sdk\GCSRTNet\GCSRTNet.vcxproj" /build debug