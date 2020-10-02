#define TIMING_PAGE_LEN 750
#define TIMING_PAGE "\
<html>\
<head>\
<title>Timing Setup</title>\
<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;}</style>\
</head>\
<body>\
<h1>Timing Setup</h1>\
<form>\
<table style='width:50%' border='1' cellpadding='1' cellspacing='1' summary=''>\
<tr><th><button type='submit' formmethod='post' formaction='/Tpost' formtarget='_self'>Save</button></th>\
<th>UP</th>\
<th>DOWN</th></tr>\
<tr><td><b>Window 1</b></td>\
<td><input type='number' name='W1tu' value=%00d></td>\
<td><input type='number' name='W1td' value=%00d></td></tr>\
<tr><td><b>Window 2</b></td>\
<td><input type='number' name='W2tu' value=%00d></td>\
<td><input type='number' name='W2td' value=%00d></td></tr>\
</table>\
</form>\
</body>\
</html>"
