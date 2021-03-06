#!/bin/bash
OGLOGCOMMAND=${OGLOGCOMMAND:-"/tmp/command.log"}
OGLOGCOMMANDTMP=$OGLOGCOMMAND.tmp
echo "Content-type: text/html"
echo ""
echo "<html><head>"
echo "<meta charset='utf-8'>"
echo "<meta http-equiv='Refresh' content='5,URL=./LogCommand.sh'> <link rel='stylesheet' type='text/css' href='oglive.css' />"
echo "<title>Bash as CGI</title></head><body>"

strings -e S $OGLOGCOMMAND > $OGLOGCOMMANDTMP

echo "<table>"

echo "<tr>"


echo "<TEXTAREA NAME='trackloghead' ROWS='13' COLS='175'>"
#echo "$(head -n 10 /tmp/command.log.tmp | uniq)"
# UHU - 2013/07/05 - Se incluye el simbolo % y la palabra sent para que se muestre la salida de rsync
egrep -v '^aclfilter|^deleting|^bytes|%|sent|^Elapsed:|^Total [Tt]ime:|^-|^\\|^\/' $OGLOGCOMMANDTMP | uniq | head -n 15 
echo "</TEXTAREA>"

echo "</tr>"


echo "<tr>"

echo "<TEXTAREA NAME='tracklogtail' ROWS='2' COLS='175'>"
#echo "$(tail -n 5 /tmp/command.log.tmp | uniq)"
# UHU - 2013/07/05 - Se incluye el simbolo % y la palabra sent para que se muestre la salida de rsync
egrep '^aclfilter|^deleting|^bytes|%|sent|^Elapsed:|^Total [Tt]ime:|^-|^\\|^\/' $OGLOGCOMMANDTMP | uniq | tail -n 2 
echo "</TEXTAREA>"


echo "</tr>"

echo "</table>"


echo "</body></html>"
