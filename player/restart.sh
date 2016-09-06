kill $(ps aux | grep /renderer/vDome | awk '{print $2}')
sleep 1
open ./renderer/vDome.app
