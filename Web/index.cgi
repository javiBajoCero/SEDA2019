t <!DOCTYPE html>
t     <html>
t       <head>
t         <meta content="text/html; charset=UTF-8" http-equiv="content-type">
t         <meta http-equiv="refresh" content="20; url=http://192.168.1.120/index.cgi">
t         <title>Control del robot espia</title>
t       </head>
t       <body> 
t         <form id="formulario" action="/index.cgi" method="get"> 
t           <table style="width: 200px" border="0">
t             <tbody>
t               <tr align="center">
t                 <td border="1" style="width: 100%">
t                   <div style="font-weight:bold">
t                     Control del robot espia
t                   </div>
t                 </td>
t               </tr>
t               <tr align="center">
t                 <td border="1" style="width: 100%">
t                   <div style="font-weight:bold">
#t                     <button name="enviar" value="OFF"  type="submit">send</button>
#t						<p>Comandos</p>
t                  </div>
t                 </td>
t               </tr>
t               <tr align="center">
t                 <td border="1" style="width: 100%">
#t                   <div style="font-weight:bold">
#t                     <button name="leds" value="OFF"  type="submit">Apaga LEDs</button>
#t                   </div>
t                 </td>
t               </tr>
t               <tr align="center">
t                 <td style="width: 100%">
#c a 1               <input name="led_1" value="ON" onclick="submit()" type="checkbox" %s >
#t                   LED 1 
c a 2               <input value="..." type="text" name="comandos" >
t                   <br>
t                   <div style="font-weight:bold">
t                     <button name="enviar" value="OFF"  type="submit">send</button>
t                   </div>
t                   <input name="Doomy" value="Doomy" type="hidden">
t                 </td>
t               </tr>
t             </tbody>
t           </table>
t           <table style="width: 200px" border="0">
t             <tbody>
t               <tr align="center">
#c a 3             <td border="1" bgcolor="%s" style="width: 45%%">
t                   <div style="color:white;font-weight:bold">Led 1</div>
t                 </td>
t                 <td style="width: 10%">    <br>
t                 </td>
#c a 4             <td border="1" bgcolor="%s"  style="width: 45%%">
t                   <div style="color:white;font-weight:bold">Led 2</div>
t                 </td>
t               </tr>
t             </tbody>
t           </table>
t         </form>
t         <br>
t       </body>
t     </html>
.