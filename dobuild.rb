require 'p4'
p4 = P4.new()
p4.connect()
client = p4.fetch_client()
p4.run_sync("-f", "//backroom/buildtools/...")
require client['Root']+'/backroom/buildtools/buildtools'
p4.disconnect
$versionFile = ['.\CFtpServer\CFtpServer.rc']
$project = "backroom/Tools/FtpServerDll"

$toCopy = [
  %w(.\CFtpServer\CFtpServer.h _export),
  %w(.\Release\CFtpServer.lib _export\vs12),
  %w(.\Release\CFtpServer.pdb _export\vs12),
  %w(.\Release\CFtpServer.dll _export\vs12)
]
$buildCmd = [	"#{$devenv12} /rebuild Release CFtpServer.sln"
]
$docCmd = [] 
$setupCmd = [] 

startBuild()
