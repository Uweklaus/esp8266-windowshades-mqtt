#define HOME_PAGE_LEN 1650
#define HOME_PAGE "\
<html>\
<head>\
<meta content='text/html; charset=ISO-8859-1' http-equiv='content-type'>\
<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>\
<title>ESP8266 Window Shades</title>\
</head>\
<body style='width: 600px;'>\
<h1>ESP8266 Window Shades</h1>\
<p>Uptime: %02d:%02d:%02d</p>\
<p>Hostname: %s</p>\
<p> SSID: %s</p>\
<p>IP-Address %00d.%00d.%00d.%00d</p>\
<p>MQTT-Server %s:%d %s</p>\
<br>\
<table style='width: 75%;' summary='' border='1' cellpadding='1' cellspacing='1'>\
<tbody>\
<tr>\
<th>POSITION</th>\
<th>Current</th>\
<th>Target</th>\
<th>State</th>\
</tr>\
<tr>\
<td align='center'><b>Window 1</b></td>\
<td align='center'>%00d</td>\
<td align='center'>%00d</td>\
<td align='center'>%00d</td>\
</tr>\
<tr>\
<td align='center'><b>Window 2</b></td>\
<td align='center'>%00d</td>\
<td align='center'>%00d</td>\
<td align='center'>%00d</td>\
</tr>\
</tbody>\
</table>\
<br>\
<form>\
<button name='pins' formmethod='get' formaction='/setupPin' formtarget='_self'>Setup Pins</button>\
<button type='submit' formmethod='get' formaction='/setupTime' formtarget='_self'>Setup Timing</button>\
<button type='submit' formmethod='get' formaction='/setCurrent' formtarget='_self'>Set Current Position</button><br>\
<button type='submit' formmethod='get' formaction='/root' formtarget='_self'>Set SSID</button>\
<button type='submit' formmethod='get' formaction='/standard' formtarget='_self'>Set Standard Values</button>\
<button type='submit' formmethod='get' formaction='/save' formtarget='_self'>Save Config</button>\
</form>\
</body>\
</html>"
