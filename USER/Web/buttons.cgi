t <html><head><title>Button inputs</title>
t <script language=JavaScript type="text/javascript" src="xml_http.js"></script>
t <script language=JavaScript type="text/javascript">
# Define URL and refresh timeout
t var formUpdate = new periodicObj("buttons.cgx", 300);
t function periodicUpdate() {
t  if(document.getElementById("refreshChkBox").checked == true) {
t   updateMultiple(formUpdate);
t   periodicFormTime = setTimeout("periodicUpdate()", formUpdate.period); 
t  }
t  else
t   clearTimeout(periodicFormTime);
t }
t </script></head>
i pg_header.inc
t <h3 align="center"><br>Buttons ISP, SW1, SW2 on the Mini-DK2 board</h3>
t <p><font size="2">This page allows you to monitor on board buttons state.
t  Periodic screen update is based on <b>xml</b> technology. This results in smooth 
t  flicker-free screen update.<br><br>
t  Press a button on Mini-DK2 board and observe the change on the screen.</font></p>
t <form action="buttons.cgi" method="post" id="form1" name="form1">
t <table border="0" width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Item</th>
t  <th width=60%>Status</th>
t </tr>
t <tr>
t  <td><img src="pabb.gif">Buttons [ISP, SW1, SW2]:</td>
t  <td align="center">
t   <input type="checkbox" disabled id="button2">SW2
t   <input type="checkbox" disabled id="button1">SW1
t   <input type="checkbox" disabled id="button0">ISP  
t  </td>
t </tr>
t </font></table>
t <p align="center">
t  <input type="button" id="refreshBtn" value="Refresh" onclick="updateMultiple(formUpdate)">
t  Periodic:<input type="checkbox" id="refreshChkBox" onclick="periodicUpdate()">
t </p></form>
i pg_footer.inc
. End of script must be closed with period.
