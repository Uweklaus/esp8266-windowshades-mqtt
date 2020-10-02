#define CURRENT_PAGE_LEN 760
#define CURRENT_PAGE "\
<html>\
<head>\
<title>Set Current Position</title>\
<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;}</style>\
</head>\
<body>\
<h1>Set Current Position</h1>\
<form>\
<table style='width:50%' border='1' cellpadding='1' cellspacing='1' summary=''>\
<tr>\
<th><button type='submit' formmethod='post' formaction='/Cpost' formtarget='_self'>Save</button></th>\
<th>Current Position</th>\
</tr>\
<tr>\
<td align='center'><b>Window 1</b></td>\
<td align='center'><input maxlength='3' value='%00d' name='Cur1' type='number'></td>\
</tr>\
<tr>\
<td align='center'><b>Window 2</b></td>\
<td align='center'><input maxlength='3' value='%00d' name='Cur2' type='number'></td>\
</tr>\
</table>\
</form>\
</body>\
</html>"
