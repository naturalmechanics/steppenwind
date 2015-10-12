 #!/usr/bin/bash
SWITCH="\e["
NORMAL="${SWITCH}0m"
RED="${SWITCH}31m"
MAGENTA="${SWITCH}35m"
YELLOW="${SWITCH}33m"
GREEN="${SWITCH}32m"

 
 echo ""
 echo -e "${GREEN}acquire ; ${RED}analyze ; ${MAGENTA}simulate ; ${RED}model ; ${RED}experiment ; ${RED}compute ; ${GREEN}map ; ${YELLOW}verify ; ${GREEN}transfer ; ${YELLOW}visualize ; ${GREEN}web ; ${GREEN}plan ; ${GREEN}maintain ; ${YELLOW}work ; ${GREEN}help ${NORMAL}" 
 printf %s "--> "